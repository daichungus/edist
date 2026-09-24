#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void run_tests_small(size_t nproc);
void run_test_1mil(size_t nproc);
void test_rand_string(size_t len, size_t nproc);

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

	const char *noarg_err = "Usage: ./edist -n <string_length> -t <thread_count>\n";

	if (argc == 1) {
		fprintf(stderr, "%s", noarg_err);
		exit(EXIT_FAILURE);
	}

	// Default arguments
	long n = -1;
	long nproc = sysconf(_SC_NPROCESSORS_ONLN);

	const char *optstr = "n:t:b:a";
	const char *len_error = "Error: n requires positive integer input.\n";
	const char *thread_error = "Error: thread arg must be 1 <= t <=";

	int opt;

	while ((opt = getopt(argc, argv, optstr)) != -1) {
		switch(opt) {
			case 'n': {
				n = strtol(optarg, NULL, 10);

				if (n < 0) {
					fprintf(stderr, "%s", len_error);
					exit(EXIT_FAILURE);
				}

				break;
			}
			case 't': {
			    long thread_arg = strtol(optarg, NULL, 10);

				if ((thread_arg > nproc) || (thread_arg < 1)) {
					fprintf(stderr, "%s %ld\n", thread_error, nproc);
					exit(EXIT_FAILURE);
				}

				nproc = thread_arg;

				break;
			}
			case 'b':
			case 'a':
			case ':': {
				break;
			}
			case '?': {
				printf("Unknown option: %c\n", optopt);
				break;
			}
		}
	}
	
	if (n == -1) {
		fprintf(stderr, "%s", noarg_err);
		exit(EXIT_FAILURE);
	}

	if (nproc <= 0) {
		fprintf(stderr, "No threads are available\n");
		exit(EXIT_FAILURE);
	}
	
	double t0;
	double t1;

	t0 = now_ms();
	test_rand_string(n, (size_t)nproc);
	t1 = now_ms();
	printf("Total elapsed time: ");
	printf_ms(t1 - t0);

	return 0;
}
