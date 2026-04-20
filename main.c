#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void run_tests_small(void);
void run_test_1mil(void);
void test_rand_string(size_t len);

static inline double now_ms(void) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1e6;
}

static inline void printf_ms(double t) {
	if (t > 3.6e+6) {
		printf("%.6f hr\n", t / 3.6e+6);
	} else if (t > 60000.0) {
		printf("%.6f min\n", t / 6e+4);
	} else if (t > 1000.0) {
		printf("%.6f s\n", t / 1e+3);
	} else {
		printf("%.6f ms\n", t);
	}
}

int main(int argc, char *argv[]) {

	if (argc == 1) {
		fprintf(stderr, "Usage: ./edist -n <length>\n");
		exit(EXIT_FAILURE);
	}

	// Default arguments
	long n = - 1;

	const char *optstr = "n:t:b:a";
	
	int opt;

	while ((opt = getopt(argc, argv, optstr)) != -1) {
		switch(opt) {
			case 'n': {
				n = strtol(optarg, NULL, 10);
				if (n <= 0) {
					fprintf(stderr, "Error: n <= 0\n");
					exit(EXIT_FAILURE);
				}
				break;
			}
			case 't':
			case 'b':
			case 'a':
			case ':': {
				printf("option needs a value\n");
				break;
			}
			case '?': {
				printf("unknown option: %c\n", optopt);
				break;
			}
		}
	}

	double t0;
	double t1;

	if (n == -1) {
		fprintf(stderr, "Error: -n <string_length>\n");
	} else {
		t0 = now_ms();
		test_rand_string(n);
		t1 = now_ms();
		printf("Total elapsed time: ");
		printf_ms(t1 - t0);
	}

	// run_tests_small();
	// run_test_1mil(); 

	return 0;
}
