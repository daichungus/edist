#define _XOPEN_SOURCE 600

#include <pthread.h>
#include <unistd.h>
#include <emmintrin.h>
#include <immintrin.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <stdio.h>
#include "edit_distance.h"
#include "tile_calculator.h"

#define cost(a, b) (a == b) ? 0 : 1
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min3(a, b, c) min(min(a, b), c)

// Calculate optimal tile size
static inline size_t opt_tilesize(size_t max_len, size_t num_threads) {
    const size_t L1_CACHE_LIMIT = 2048; // Actual L1 limit: 2730 
    size_t sat_limit = max_len / num_threads;
    if (sat_limit < 1) {
        sat_limit = 1;
    }
    size_t optimal = L1_CACHE_LIMIT;
    if (optimal > sat_limit) {
        optimal = sat_limit;
    }
    if (optimal < 64) {
        optimal = 64;
    }
    return optimal;
}

// Arguments for the threads
struct ThreadArgs {
    pthread_barrier_t *barrier;
    
    const char *str1;
    const char *str2;
    
    int *g_top_row;
    int *g_left_col;
    int **vdp;

    size_t tile_size;
    size_t len1;
    size_t len2;

    size_t num_tiles_i;
    size_t num_tiles_j;

    size_t thread_id;
    size_t num_threads;
};

// Calculate the minimum of 3 vectors
static inline __m256i avx_min3(__m256i a, __m256i b, __m256i c) {
    return _mm256_min_epi32(_mm256_min_epi32(a, b), c);
}

// Inter-tile operation
void compute_tile_avx(const char *str1p, const char *str2p, 
                        size_t h, size_t w, 
                        size_t str1_global_offset, size_t str2_global_offset, 
                        size_t len1, size_t len2, 
                        int topleft, int *row_edgep, int *col_edgep, int *corner_outp) {

    // Allocate diagonal buffers
    size_t maxwave = h + w - 2;
    __attribute__((aligned(32))) int buffer_prevprev[maxwave + 8];
    __attribute__((aligned(32))) int buffer_prev[maxwave + 8];
    __attribute__((aligned(32))) int buffer_curr[maxwave + 8];

    int *d_prevprev = &buffer_prevprev[1];
    int *d_prev = &buffer_prev[1];
    int *d_curr = &buffer_curr[1];

    // Initialize vector of 1's
    const __m256i v_ones = _mm256_set1_epi32(1);

    for (size_t wave = 0; wave <= maxwave; wave++) {

        if (wave < w) {
            d_prev[-1] = row_edgep[wave];
            if (wave == 0) {
                d_prevprev[-1] = topleft;
            } else {
                d_prevprev[-1] = row_edgep[wave - 1];
            }
        }

        if (wave < h) {
            d_prev[wave] = col_edgep[wave];
        }
        
        size_t i_min = (wave < w) ? 0 : (wave - w + 1);
        size_t i_max = (wave < h) ? wave : (h - 1);

        size_t k = i_min;

        for (; k + 7 <= i_max; k += 8) {
            // Load string 1 and string 2 chunks
            size_t global_str1_base = str1_global_offset + k;
            size_t global_str2_base = str2_global_offset + (wave - k);

            // Check if elements are within bounds
            if (global_str1_base + 8 <= len1 && 
                global_str2_base >= 7 && 
                global_str2_base - 7 >= str2_global_offset) {
                
                long long str1_chunk;
                memcpy(&str1_chunk, &str1p[k], sizeof(long long));

                __m128i s1_vec = _mm_cvtsi64_si128(str1_chunk);
                __m256i char1_vec = _mm256_cvtepu8_epi32(s1_vec);

                __m256i char2_vec = _mm256_set_epi32(
                    (unsigned char)str2p[wave - (k + 7)], 
                    (unsigned char)str2p[wave - (k + 6)], 
                    (unsigned char)str2p[wave - (k + 5)], 
                    (unsigned char)str2p[wave - (k + 4)], 
                    (unsigned char)str2p[wave - (k + 3)], 
                    (unsigned char)str2p[wave - (k + 2)], 
                    (unsigned char)str2p[wave - (k + 1)], 
                    (unsigned char)str2p[wave - (k)]
                );

                __m256i vec_ins = _mm256_loadu_si256((__m256i*)&d_prev[k]);
                __m256i vec_del = _mm256_loadu_si256((__m256i*)&d_prev[k - 1]);
                __m256i vec_sub = _mm256_loadu_si256((__m256i*)&d_prevprev[k - 1]);

                // Compare characters and calculate cost
                __m256i v_match = _mm256_cmpeq_epi32(char1_vec, char2_vec);
                __m256i v_cost_sub = _mm256_add_epi32(vec_sub, v_ones);
                v_cost_sub = _mm256_add_epi32(v_cost_sub, v_match);

                __m256i v_cost_ins = _mm256_add_epi32(vec_ins, v_ones);
                __m256i v_cost_del = _mm256_add_epi32(vec_del, v_ones);
                __m256i v_result = avx_min3(v_cost_del, v_cost_ins, v_cost_sub);

                _mm256_storeu_si256((__m256i*)&d_curr[k], v_result);

            } else {
                // Fall back to scalar loop
                break;
            }
            
        }

        // Scalar loop for bounds
        for (; k <= i_max; k++) {
            int prev_del = d_prev[k-1];
            int prev_ins = d_prev[k];
            int prev_sub = d_prevprev[k-1];

            size_t global_str1_idx = str1_global_offset + k;
            size_t global_str2_idx = str2_global_offset + wave - k;

            int c = 0;
            if (global_str1_idx < len1 && global_str2_idx < len2) {
                c = cost(str1p[k], str2p[wave - k]);
            } else {
                c = 1; // Out-of-bounds mismatch cost
            }

            int val = min3(prev_del + 1, prev_ins + 1, prev_sub + c);
            d_curr[k] = val;
        }

        // Export edge values
        size_t start = i_min;
        size_t end = i_max;

        if (end == h - 1) {
            size_t col_index = wave - (h - 1);
            if (col_index < w) {
                row_edgep[col_index] = d_curr[h - 1];    
            }
        }

        if (start == wave - w + 1 && wave >= w - 1) {
            size_t row_index = start;
            if (row_index < h) {
                col_edgep[row_index] = d_curr[row_index];
            }
        }

        if (wave == maxwave) {
            *corner_outp = d_curr[h-1];
        }

        // Swap buffer pointers
        int *temp = d_prevprev;
        d_prevprev = d_prev;
        d_prev = d_curr;
        d_curr = temp;
    }
}

void *worker_thread(void *arg) {
    struct ThreadArgs *args = (struct ThreadArgs *)arg;

    // Extract values from ThreadArgs
    const char *str1 = args->str1;
    const char *str2 = args->str2;
    int *g_top_row = args->g_top_row;
    int *g_left_col = args->g_left_col;
    int **vdp = args->vdp;
    size_t len1 = args->len1;
    size_t len2 = args->len2;
    size_t tile_size = args->tile_size;
    size_t num_tiles_i = args->num_tiles_i;
    size_t num_tiles_j = args->num_tiles_j;
    size_t num_threads = args->num_threads;
    size_t thread_id = args->thread_id;

    size_t total_waves = num_tiles_i + num_tiles_j - 1;

    // Diagonal wavefront
    for (size_t wave = 0; wave < total_waves; wave++) {

        size_t ti_min = (wave < num_tiles_j) ? 0 : (wave - num_tiles_j + 1); // This was "the one thing"
        size_t ti_max = (wave < num_tiles_i) ? wave : (num_tiles_i - 1);

        // Iterate through the tile
        for (size_t ti = ti_min + thread_id; ti <= ti_max; ti += num_threads) {

            size_t tj = wave - ti;
            size_t tile_idx = ti * num_tiles_j + tj;

            if (vdp[tile_idx] == NULL) {
                vdp[tile_idx] = (int *)malloc(2 * tile_size * sizeof(int));
                if (vdp[tile_idx] == NULL) {
                    return NULL; // malloc failed
                }
            }

            // Determine the tile indices and dimensions
            size_t i_start = ti * tile_size + 1;
            size_t j_start = tj * tile_size + 1;

            size_t i_end = min(i_start + tile_size - 1, len1);
            size_t j_end = min(j_start + tile_size - 1, len2);

            size_t tile_height = i_end - i_start + 1;
            size_t tile_width = j_end - j_start + 1;

            // Value of upper left tile's bottom right corner
            int current_corner;
            if (ti == 0 && tj == 0) {
                current_corner = 0;
            } else if (ti == 0) {
                current_corner = g_top_row[j_start - 1];
            } else if (tj == 0) {
                current_corner = g_left_col[i_start - 1];
            } else {
                size_t diag_tile_idx = (ti - 1) * num_tiles_j + (tj - 1);
                current_corner = vdp[diag_tile_idx][tile_size - 1];
            }

            int *row_edge_for_tile = (int *)malloc((tile_size + 1) * sizeof(int));
            int *col_edge_for_tile = (int *)malloc((tile_size + 1) * sizeof(int));

            // Copy top row
            if (ti == 0) {
                for (size_t k = 0; k < tile_width; k++) {
                    row_edge_for_tile[k] = g_top_row[j_start + k];
                }
            } else {
                size_t up_tile_idx = (ti - 1) * num_tiles_j + tj;
                for (size_t k = 0; k < tile_width; k++) {
                    row_edge_for_tile[k] = vdp[up_tile_idx][k];  // Bottom of up-tile
                }
            }

            // Copy left column
            if (tj == 0) {
                for (size_t k = 0; k < tile_height; k++) {
                    col_edge_for_tile[k] = g_left_col[i_start + k];
                }
            } else {
                size_t left_tile_idx = ti * num_tiles_j + (tj - 1);
                for (size_t k = 0; k < tile_height; k++) {
                    col_edge_for_tile[k] = vdp[left_tile_idx][tile_size + k];  // Right of left-tile
                }
            }

            // Bottom right corner
            // int next_corner = g_top_row[j_start + tile_width - 1];

            // Intra-tile computation
            compute_tile_avx(
                str1, 
                str2,
                tile_height, 
                tile_width, 
                i_start - 1,
                j_start - 1,
                len1,
                len2,
                current_corner, 
                row_edge_for_tile, 
                col_edge_for_tile, 
                &vdp[tile_idx][tile_size - 1]
            );

            
            // Copy output boundaries to vdp
            for (size_t k = 0; k < tile_width; k++) {
                vdp[tile_idx][k] = row_edge_for_tile[k];
            }
            for (size_t k = 0; k < tile_height; k++) {
                vdp[tile_idx][tile_size + k] = col_edge_for_tile[k];
            }

            // If this is the bottom-right tile, store the answer
            if (ti == num_tiles_i - 1 && tj == num_tiles_j - 1) {
                vdp[tile_idx][tile_size - 1] = row_edge_for_tile[tile_width - 1];
            }

            free(row_edge_for_tile);
            free(col_edge_for_tile);
        }

        pthread_barrier_wait(args->barrier);

    }

    return NULL;
}

// Calculate edit distance with specific tile size and number of threads
int edit_distance_base(const char *str1, const char *str2, size_t len1, size_t len2, size_t tile_size, size_t num_threads) {
    
    // Determine number of tiles needed
    size_t num_tiles_i = (len1 + tile_size - 1) / tile_size;
    size_t num_tiles_j = (len2 + tile_size - 1) / tile_size;

    // Allocate buffers for tile edges and corner values
    // size_t max_len = max(len1, len2);
    // int *row_edge = (int *)malloc((max_len + 1) * sizeof(int));
    // int *col_edge = (int *)malloc((max_len + 1) * sizeof(int));
    int **vdp = (int **)malloc(num_tiles_i * num_tiles_j * sizeof(int *));

    // Global boundary arrays
    int *g_top_row = (int *)malloc((len2 + 1) * sizeof(int));
    int *g_left_col = (int *)malloc((len1 + 1) * sizeof(int));

    if (!vdp || !g_top_row || !g_left_col) {
        free(vdp);
        free(g_top_row);
        free(g_left_col);
        return -1;
    }

    // Initialize row and column edges
    for (size_t k = 0; k <= len2; k++) {
        g_top_row[k] = k;
    }
    for (size_t k = 0; k <= len1; k++) {
        g_left_col[k] = k;
    }

    for (size_t i = 0; i < num_tiles_i * num_tiles_j; i++) {
        vdp[i] = NULL;
    }

    // Limit the number of threads
    size_t min_dimension = min(num_tiles_i, num_tiles_j);
    
    if (num_threads > min_dimension) {
        num_threads = min_dimension;
    }

    printf("Tile size: %zu\n", tile_size);
    printf("Number of threads: %zu\n", num_threads);

    pthread_t threads[num_threads];
    struct ThreadArgs t_args[num_threads];

    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, num_threads);
    
    // Spawn worker threads
    for (size_t i = 0; i < num_threads; i++) {
        t_args[i].barrier = &barrier;

        t_args[i].str1 = str1;
        t_args[i].str2 = str2;

        t_args[i].g_top_row = g_top_row;
        t_args[i].g_left_col = g_left_col;
        t_args[i].vdp = vdp;

        t_args[i].tile_size = tile_size;
        t_args[i].len1 = len1;
        t_args[i].len2 = len2;

        t_args[i].num_tiles_i = num_tiles_i;
        t_args[i].num_tiles_j = num_tiles_j;

        t_args[i].thread_id = i;
        t_args[i].num_threads = num_threads;

        pthread_create(&threads[i], NULL, worker_thread, &t_args[i]);
    }

    for (size_t i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);

    int result = vdp[num_tiles_i * num_tiles_j - 1][tile_size - 1];

    for (size_t i = 0; i < num_tiles_i * num_tiles_j; i++) {
        if (vdp[i] != NULL) {
            free(vdp[i]);
        }
    }

    free(vdp);
    free(g_top_row);
    free(g_left_col);
    
    return result;
}

// Wrapper
int edit_distance(const char *str1, const char *str2, size_t len1, size_t len2) {
    // Special cases
    if (len1 == 0 || len2 == 0) {
        return len1 + len2;
    }
    if (len1 == 1) {
        int found = (memchr(str2, str1[0], len2) != NULL);
        return len2 - found;
    }
    if (len2 == 1) {
        int found = (memchr(str1, str2[0], len1) != NULL);
        return len1 - found;
    }

    // Set up threads and barrier
    long nproc = sysconf(_SC_NPROCESSORS_ONLN);
    if (nproc < 1) {
        nproc = 1;
    }

    size_t max_len = max(len1, len2);
    size_t tile_size;
    if (len1 < 16384 || len2) {
        tile_size = opt_tilesize(max_len, (size_t)nproc);
    } else {
        tile_size = calculate_tile((size_t)nproc);
    }

    size_t num_tiles_i = (len1 + tile_size - 1) / tile_size;
    size_t num_tiles_j = (len2 + tile_size - 1) / tile_size;
    size_t num_threads = (size_t)nproc;
    size_t min_tiles = min(num_tiles_i, num_tiles_j);
    if (num_threads > min_tiles) num_threads = min_tiles;

    return edit_distance_base(str1, str2, len1, len2, tile_size, num_threads);
}