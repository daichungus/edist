#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "tile_calculator.h"
#include "edit_distance.h"

static inline long now_ns(void) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts.tv_sec * 1000000000L + ts.tv_nsec;
}

static inline void printf_ns(long t) {
	if (t > 3.6e+12) {
		printf("%.6Lf hr\n", t / 3.6e+12L);
	} else if (t > 6e+10) {
		printf("%.6Lf min\n", t / 6e+10L);
	} else if (t > 1e+9) {
		printf("%.6Lf s\n", t / 1e+9L);
	} else if (t > 1e+6) {
		printf("%.6Lf ms\n", t / 1e+6L);
	} else if (t > 1000) {
		printf("%ld %cs\n", t / 1000, 230);
	} else {
		printf("%ld ns\n", t);
	}
}

static inline char *gen_dummy(size_t len) {
	char *s = malloc(len + 1);
	memset(s, 'A', len);
	s[len] = '\0';
	return s;
}

/* 
 * Calculate the most most optimal tile dimension. 
 * May not be as fast as theoretical numbers, 
 * due to CPU processes and environmental variables.
 */
size_t calculate_tile() {
	long t0 = now_ns();
	const size_t BENCH_LEN = 32768; 
	char *s1 = gen_dummy(BENCH_LEN);
	char *s2 = gen_dummy(BENCH_LEN);

	size_t candidates[] = {64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};
	size_t num_candidates = sizeof(candidates) / sizeof(candidates[0]);

	long best_time = LONG_MAX;
	size_t best_dim = 2048;

	printf("Finding best tile dimension...\n");

	for (size_t i = 0; i < num_candidates; i++) {
		size_t ts = candidates[i];

		if (ts > BENCH_LEN) continue;

		long t0 = now_ns();

		edit_distance_base(s1, s2, BENCH_LEN, ts, 1);

		long t1 = now_ns();
		long time = t1 - t0;

		if (time < best_time) {
			best_time = time;
			best_dim = ts;
		}
		
	}
	
	long t1 = now_ns();
	printf("Elapsed time: ");
	printf_ns(t1 - t0);

	free(s1);
	free(s2);
	
	return best_dim;
}
