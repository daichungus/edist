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
    
    printf("Small test cases:\n");
    double t0 = now_ms();
    run_tests_small(); 
    double t1 = now_ms();
    printf("Elapsed time: %.6f ms\n", t1 - t0);
    printf("\n");
    
    printf("1 million-length string test:\n");
    double t2 = now_ms();
    run_test_1mil(); 
    double t3 = now_ms();
    printf("Elapsed time: %.6f ms\n", t3 - t2);
    printf("\n");

    return 0;
}