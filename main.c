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

static inline void printf_ms(double t) {
    if (t > 3.6e+6) {
        printf("%.6f hr\n", t / 3.6e+6);
    } else if (t > 60000.0) {
        printf("%.6f min\n", t / 60000.0);
    } else if (t > 1000.0) {
        printf("%.6f s\n", t / 1000.0);
    } else {
        printf("%.6f ms\n", t);
    }
}

int main() {

    double t0 = now_ms();
    // run_tests_small();
    run_test_1mil(); 
    double t1 = now_ms();

    // Determine elapsed time
    printf("Elapsed time: ");
    printf_ms(t1 - t0);
    return 0;
}