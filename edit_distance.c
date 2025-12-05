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

#define cost(a, b) (a == b) ? 0 : 1
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define min3(a, b, c) min(min(a, b), c)

// Calculate optimal tile size
static inline size_t opt_tilesize(size_t len, size_t num_threads) {
    const size_t L1_CACHE_LIMIT = 2048; // Actual L1 limit: 2730 
    size_t sat_limit = len / num_threads;
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
    int *row_edge;
    int *col_edge;
    int *corners;

    size_t len;
    size_t tile_size;
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
void compute_tile_avx(const char *str1p, const char *str2p, size_t tile_size, size_t h, size_t w, int topleft, int *row_edgep, int *col_edgep, int *corner_outp) {

    // AVX diagonal buffers
    __attribute__((aligned(32))) int buffer_prevprev[tile_size + 8];
    __attribute__((aligned(32))) int buffer_prev[tile_size + 8];
    __attribute__((aligned(32))) int buffer_curr[tile_size + 8];

    // Pointers to the buffers
    int *d_prevprev = &buffer_prevprev[1];
    int *d_prev = &buffer_prev[1];
    int *d_curr = &buffer_curr[1];

    // Initialize vector of 1's
    const __m256i v_ones = _mm256_set1_epi32(1);

    size_t maxwave = h + w - 2;
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
            __m256i vec_ins = _mm256_loadu_si256((__m256i*)&d_prev[k]);
            __m256i vec_del = _mm256_loadu_si256((__m256i*)&d_prev[k - 1]);
            __m256i vec_sub = _mm256_loadu_si256((__m256i*)&d_prevprev[k - 1]);

            // Load string 1 and string 2 chunks
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

            // Compare characters to each other
            __m256i v_match = _mm256_cmpeq_epi32(char1_vec, char2_vec);

            // Calculate cost
            __m256i v_cost_sub = _mm256_add_epi32(vec_sub, v_ones);
            v_cost_sub = _mm256_add_epi32(v_cost_sub, v_match);
            __m256i v_cost_ins = _mm256_add_epi32(vec_ins, v_ones);
            __m256i v_cost_del = _mm256_add_epi32(vec_del, v_ones);
            __m256i v_result = avx_min3(v_cost_del, v_cost_ins, v_cost_sub);

            _mm256_storeu_si256((__m256i*)&d_curr[k], v_result);
        }

        // Clean up in case of nonalignment with tile size
        for (; k <= i_max; k++) {
            int prev_del = d_prev[k-1];
            int prev_ins = d_prev[k];
            int prev_sub = d_prevprev[k-1];
                
            int cost = cost(str1p[k], str2p[wave - k]);

            int val = min3(prev_del + 1, prev_ins + 1, prev_sub + cost);

            d_curr[k] = val;
        }

        // Export edge values for inter-tile operations
        size_t start = i_min;
        size_t end = i_max;

        if (end == h - 1) {
            size_t col_index = wave - (h - 1);
            row_edgep[col_index] = d_curr[h - 1];
        }

        if (start == wave - w  + 1) {
            size_t row_index = start;
            col_edgep[row_index] = d_curr[row_index];
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
    int *row_edge = args->row_edge;
    int *col_edge = args->col_edge;
    int *corners = args->corners;
    size_t len = args->len;
    size_t tile_size = args->tile_size;
    size_t num_tiles_i = args->num_tiles_i;
    size_t num_tiles_j = args->num_tiles_j;
    size_t num_threads = args->num_threads;
    size_t thread_id = args->thread_id;

    size_t total_waves = num_tiles_i + num_tiles_j - 1;

    // Diagonal wavefront
    for (size_t wave = 0; wave < total_waves; wave++) {

        size_t ti_min = (wave < num_tiles_i) ? 0 : (wave - num_tiles_j + 1);
        size_t ti_max = (wave < num_tiles_i) ? wave : (num_tiles_i - 1);

        // Iterate through the tile
        for (size_t ti = ti_min + thread_id; ti <= ti_max; ti += num_threads) {

            size_t tj = wave - ti;

            // Determine the tile indices and dimensions
            size_t i_start = ti * tile_size + 1;
            size_t j_start = tj * tile_size + 1;

            size_t i_end = min(i_start + tile_size - 1, len);
            size_t j_end = min(j_start + tile_size - 1, len);

            size_t tile_height = i_end - i_start + 1;
            size_t tile_width = j_end - j_start + 1;

            // Value of upper left tile's bottom right corner
            int current_corner;
            if (ti == 0 && tj == 0) {
                current_corner = 0;
            } else if (ti == 0) {
                current_corner = row_edge[j_start - 1];
            } else if (tj == 0) {
                current_corner = col_edge[i_start - 1];
            } else {
                current_corner = corners[tj - 1];
            }

            // Bottom right corner
            int next_corner = row_edge[j_start + tile_width - 1];

            // Intra-tile computation
            compute_tile_avx(
                &str1[i_start - 1], 
                &str2[j_start - 1],
                tile_size, 
                tile_height, 
                tile_width, 
                current_corner, 
                &row_edge[j_start], 
                &col_edge[i_start], 
                &corners[tj]
            );

            corners[tj] = next_corner;
        }

        // Wait until other threads finish intra-tile operations
        pthread_barrier_wait(args->barrier);
    }

    return NULL;
}

int edit_distance(const char *str1, const char *str2, size_t len) {
    
    if (len == 0) {
        return 0;
    }

    if (len == 1) {
        return cost(str1[0], str2[0]);
    }

    
    // Set up threads and barrier
    long nproc = sysconf(_SC_NPROCESSORS_ONLN);
    if (nproc < 1) {
        nproc = 1;
    }
    
    // Determine number of tiles needed
    size_t tile_size = opt_tilesize(len, (size_t)nproc);

    size_t num_tiles_i = (len + tile_size - 1) / tile_size;
    size_t num_tiles_j = (len + tile_size - 1) / tile_size;

    // Allocate buffers for tile edges and corner values
    int *row_edge = (int *)malloc((len + 1) * sizeof(int));
    int *col_edge = (int *)malloc((len + 1) * sizeof(int));
    int *corners = (int *)malloc(num_tiles_j * sizeof(int));

    if (!row_edge || !col_edge || !corners) {
        free(row_edge);
        free(col_edge);
        free(corners);
        return -1;
    }

    // Initialize row and column edges
    for (size_t k = 0; k <= len; k++) {
        row_edge[k] = k;
    }
    for (size_t k = 0; k <= len; k++) {
        col_edge[k] = k;
    }

    // Limit the number of threads
    size_t min_dimension = min(num_tiles_i, num_tiles_j);

    size_t num_threads = (size_t)nproc;
    
    if (num_threads > min_dimension) {
        num_threads = min_dimension;
    }
    if (len <= tile_size) {
        num_threads = 1;
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
        t_args[i].row_edge = row_edge;
        t_args[i].col_edge = col_edge;
        t_args[i].corners = corners;

        t_args[i].len = len;
        t_args[i].tile_size = tile_size;
        t_args[i].num_tiles_i = num_tiles_i;
        t_args[i].num_tiles_j = num_tiles_j;

        t_args[i].thread_id = i;
        t_args[i].num_threads = num_threads;

        pthread_create(&threads[i], NULL, worker_thread, &t_args[i]);
    }

    for (size_t i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    int result = row_edge[len];

    pthread_barrier_destroy(&barrier);

    free(row_edge);
    free(col_edge);
    free(corners);
    
    return result;
}
