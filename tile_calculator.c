#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "tile_calculator.h"
#include "edit_distance.h"

// Return the time in nanoseconds
static inline long now_ns(void) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts.tv_sec * 1000000000L + ts.tv_nsec;
}

// Helper to generate dummy data
static inline char *gen_dummy(size_t len) {
    char *s = malloc(len + 1);
    memset(s, 'A', len);
    s[len] = '\0';
    return s;
}

// Calculate the most consistently fast tile size. May not always be the absolute fastest
size_t calculate_tile(size_t num_threads) {
    const size_t BENCH_LEN = 32768; 
    char *s1 = gen_dummy(BENCH_LEN);
    char *s2 = gen_dummy(BENCH_LEN);

    size_t candidates[] = {64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};
    size_t num_candidates = sizeof(candidates) / sizeof(candidates[0]);

    long best_time = LONG_MAX;
    size_t best_size = 2048;

    printf("Finding best tile size on %zu threads...\n", num_threads);

    for (size_t i = 0; i < num_candidates; i++) {
        size_t ts = candidates[i];

        if (ts > BENCH_LEN) continue;

        long t0 = now_ns();

        edit_distance_base(s1, s2, BENCH_LEN, BENCH_LEN, ts, num_threads);

        long t1 = now_ns();
        long time = t1 - t0;

        if (time < best_time) {
            best_time = time;
            best_size = ts;
        }
        
    }

    free(s1);
    free(s2);
    
    return best_size;
}