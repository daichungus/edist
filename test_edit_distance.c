#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "edit_distance.h"

/*
 * Generate and return a string consisting of n characters.
 */
static inline char *gen_random_string(size_t n, char low, char high) {
	char *s = malloc(n + 1);
	for (size_t i = 0; i < n; i++) {
		s[i] = low + (rand() % (high - low + 1));
	}
	s[n] = '\0';
	return s;
}

/*
 * Test two strings' edit distance,
 * with the result compared against an expected value.
 */
 void test_small(const char* s1, const char* s2, size_t len, const int expect) {
	printf("%s\n", s1);
	printf("%s\n", s2);
	printf("Expected: %d\n", expect);
	const int result = edit_distance(s1, s2, len);
	printf("Calculated: %d\n", result);

	printf("TEST: ");
	if (expect == result) {
		printf("PASS\n");
	} else {
		printf("FAIL\n");
	}
	printf("\n");
}

/*
 * Test two strings' edit distance,
 * each consisting of randomly generated characters.
 */
void test_rand_string(size_t len) {

	srand((unsigned)time(NULL));
	char *s1 = gen_random_string(len, 'a', 'Z');
	char *s2 = gen_random_string(len, 'a', 'Z');

	printf("Length: %zu\n", len);
	const int result = edit_distance(s1, s2, len);

	free(s1);
	free(s2);

	printf("Calculated edit distance: %d\n", result);

}

/*
 * Run a few small-length test cases.
 */
void run_tests_small() {
	test_small("", "", 0, 0);
	test_small("a", "a", 1, 0);
	test_small("a", "b", 1, 1);
	test_small("ab", "ab", 2, 0);
	test_small("ab", "ba", 2, 2);
	test_small("ab", "ba", 2, 2);
	test_small("book", "back", 4, 2);
	test_small("elephant", "relevant", 8, 3);
	test_small("margatroid", "margatroid", 10, 0);
	test_small("margatroid", "spellcheck", 10, 10);
}

/*
 * Run a test for a pair of randomly-generated strings,
 * each consisting of 10^6 characters in length.
 */
void run_test_1mil() {
	test_rand_string(1000000);
}
