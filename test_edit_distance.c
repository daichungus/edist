#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

// Directly test two strings of the same length
void test_small(const char* str1, const char* str2, const int expected) {
    printf("Expected edit distance between \"%s\" and \"%s\": %d\n", str1, str2, expected);
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    const int result = edit_distance(str1, str2, len1, len2);
    printf("Calculated edit distance between \"%s\" and \"%s\": %d\n", str1, str2, result);

    printf("TEST: ");
    if (expected == result) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }
    printf("\n");
}

// Test two random strings of the same length. Actual edit distance is unknown.
void test_rand_string(size_t len) {

    srand((unsigned)time(NULL));
    char *str1 = gen_random_string(len, 'a', 'Z');
    char *str2 = gen_random_string(len, 'a', 'Z');

    printf("Length: %zu\n", len);
    const int result = edit_distance(str1, str2, len, len);

    free(str1);
    free(str2);

    printf("Calculated edit distance: %d\n", result);
    printf("\n");
}

// Test two random strings of different lengths. Actual edit distance is unknown if similar.
void test_rand_string_diff(size_t len1, size_t len2) {

    srand((unsigned)time(NULL));
    char *str1 = gen_random_string(len1, 'a', 'Z');
    char *str2 = gen_random_string(len2, 'a', 'Z');

    printf("Length 1: %zu\n", len1);
    printf("Length 2: %zu\n", len2);
    const int result = edit_distance(str1, str2, len1, len2);

    free(str1);
    free(str2);

    printf("Calculated edit distance: %d\n", result);
    printf("\n");
}

void run_tests_small() {
	test_small("", "", 0);
    test_small("a", "a", 0);
    test_small("a", "b", 1);
    test_small("ab", "ab", 0);
    test_small("ab", "ba", 2);
    test_small("ab", "ba", 2);
    test_small("book", "back", 2);
    test_small("elephant", "relevant", 3);
    test_small("margatroid", "margatroid", 0);
    test_small("margatroid", "spellcheck", 10);
    test_small("", "a", 1);
    test_small("a", "ab", 1);
    test_small("a", "aaaaa", 4);
    test_small("b", "aaaaa", 5);
    test_small("alice", "marisa", 4);
    test_small("abcdefg", "abc", 4);
    test_small("K9JQZiUk4F678E3R9W78nGIb5dRW0FM4aI2Bx", "K9JQZiUk4F678E3R9W78nGIb5dRW0FM4aI2Bx", 0);
    test_small("K9JQZiUk4F678E3R9W78nGIb5dRW0FM4aI2Bx", "R4GWW92IQRa7MUBb3xFE9d75K84ZI86nk0JiF", 36);
    test_small(
        "IaEqFRnXGmNwKBvSyBGPDvKjSGREUfZbYDGBjgTUqjLwXHLFEZpDZVpLjjttPXHJ", 
        "IaEqFRnXGmNwKBvSyBGPDvKjSGREUfZbYDGBjgTUqjLwXHLFEZpDZVpLjjttPXHJ", 
        0
    );
    test_small(
        "IaEqFRnXGmNwKBvSyBGPDvKjSGREUfZbYDGBjgTUqjLwXHLFEZpDZVpLjjttPXHJ", 
        "IaEqFRnXGmNwKBvSyBGPDvKjSGREUfZbYDGBjjTUqjLwXHLFEZpDZVpLjjttPXHJ", 
        1
    );
    test_small(
        "IaEqFRnXGmNwKBvSyBGPDvKjSGREUfZbYDGBjgTUqjLwXHLFEZpDZVpLjjttPXHJa", 
        "IaEqFRnXGmNwKBvSyBGPDvKjSGREUfZbYDGBjgTUqjLwXHLFEZpDZVpLjjttPXHJa", 
        0
    );
    test_small(
        "IaEqFRnXGmNwKBvSyBGPDvKjSGREUfZbYDGBjgTUqjLwXHLFEZpDZVpLjjttPXHJb", 
        "IaEqFRnXGmNwKBvSyBGPDvKjSGREUfZbYDGBjgTUqjLwXHLFEZpDZVpLjjttPXHJa", 
        1
    );
    test_small(
        "3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC", 
        "3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC", 
        0
    );
    test_small(
        "3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC", 
        "CCzxB67OJ08oDS5MUR3q2xSX9VAIpHeFTgPAla9jVpUPvypA8BLqC4jaqewlrVaIwe3lKTZrLGvn68z75wDABldLFXoKDAGnfmElyojM6wd8Iu1SZn1VIZt7j5HhPyv3", 
        122
    );
    test_small(
        "3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC", 
        "3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlFARw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC",  
        2
    );
    test_small(
        "3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC", 
        "3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC", 
        0
    );
    test_small(
        "3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC", 
        "3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrKTZl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC", 
        2
    );
    test_small(
        "3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC", 
        "opnU0BSVCDrPSlq8PUuzqX6aIwA8376meeMAfIo5MBPvZB6pAoaw3nUpTTlTMx6O1dC1d2j3LEDluIX3F7V4GS9wJAPLaCvVl65HzvCILt9SdAJKB5DVje4jF5yGRTynOHAPG9V8xy17LHDjpwrdMmIfqnvve3j8tEZFy8HlCGIaLDwD9wRAIzjZ5yg1lBXj88gVPxAC8K7ll7qh7aVLqalZXopo2AxolrV3SpI0ezKw5hUyjBnZwKSFnqrvA6eZ", 
        240
    );
}

void run_test_random(size_t len) {
    test_rand_string(len);
}

void run_test_random_diff(size_t len1, size_t len2) {
    test_rand_string_diff(len1, len2);
}

void run_test_1mil() {
    test_rand_string(1000000);
}