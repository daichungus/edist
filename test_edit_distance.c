#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "edit_distance.h"

static inline char *gen_random_string(size_t n, char low, char high) {
    char *s = malloc(n + 1);
    for (size_t i = 0; i < n; i++) {
        s[i] = low + (rand() % (high - low + 1));
    }
    s[n] = '\0';
    return s;
}

// Directly test two strings
void test_small(const char* str1, const char* str2, size_t len, const int expected) {
    printf("Expected edit distance between \"%s\" and \"%s\": %d\n", str1, str2, expected);
    const int result = edit_distance(str1, str2, len);
    printf("Calculated edit distance between \"%s\" and \"%s\": %d\n", str1, str2, result);

    printf("TEST: ");
    if (expected == result) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }
    printf("\n");
}

// Test random strings. Actual edit distance is unknown.
void test_rand_string(size_t len) {

    srand((unsigned)time(NULL));
    char *str1 = gen_random_string(len, 'a', 'Z');
    char *str2 = gen_random_string(len, 'a', 'Z');

    printf("Length: %zu\n", len);
    const int result = edit_distance(str1, str2, len);

    free(str1);
    free(str2);

    printf("Calculated edit distance: %d\n", result);

}

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

void run_test_1mil() {
    test_rand_string(1000000);
}
