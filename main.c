#include <stdio.h>
#include <time.h>

void run_tests_small(void);
void run_test_1mil(void);

// Get the time now in miliseconds
static inline double now_ms(void) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1e6;
}

int main() {

    double t0 = now_ms();
    run_tests_small();
    // run_test_1mil(); 
    double t1 = now_ms();
    double elapsed_time = t1 - t0;

    // Determine elapsed time units
    if (elapsed_time > 3.6e+6) {
        printf("Elapsed time: %.6f hr\n", elapsed_time / 3.6e+6);
    } else if (elapsed_time > 60000.0) {
        printf("Elapsed time: %.6f min\n", elapsed_time / 60000.0);
    } else if (elapsed_time > 1000.0) {
        printf("Elapsed time: %.6f s\n", elapsed_time / 1000.0);
    } else {
        printf("Elapsed time: %.6f ms\n", elapsed_time);
    }
    printf("\n");

    return 0;
}