#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "edit_distance.h"

char *gen_random_range(size_t n, char low, char high) {
    char *s = malloc(n + 1);
    for (size_t i = 0; i < n; i++) {
        s[i] = low + (rand() % (high - low + 1));
    }
    s[n] = '\0';
    return s;
}

// Test 0-length string. Expected result: 0
void test_0() {

    const char* str1 = "";
    const char* str2 = "";
    size_t len = 0;

    const int expected = 0;
    printf("Expected edit distance between \"%s\" and \"%s\": %d\n", str1, str2, expected);
    printf("Calculating...\n");
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

// Test 1-length strings. Expected result: 0
void test_1_same() {

    const char* str1 = "a";
    const char* str2 = "a";
    size_t len = 1;

    const int expected = 0;
    printf("Expected edit distance between \"%s\" and \"%s\": %d\n", str1, str2, expected);
    printf("Calculating...\n");
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

// Test 1-length strings. Expected result: 1
void test_1_diff() {

    const char* str1 = "a";
    const char* str2 = "b";
    size_t len = 1;

    const int expected = 1;
    printf("Expected edit distance between \"%s\" and \"%s\": %d\n", str1, str2, expected);
    printf("Calculating...\n");
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

// Test 2-length strings. Expected result: 0
void test_2_same() {

    const char* str1 = "ab";
    const char* str2 = "ab";
    size_t len = 2;

    const int expected = 0;
    printf("Expected edit distance between \"%s\" and \"%s\": %d\n", str1, str2, expected);
    printf("Calculating...\n");
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

// Test 2-length strings. Expected result: 2
void test_2_diff() {

    const char* str1 = "ab";
    const char* str2 = "cd";
    size_t len = 2;

    const int expected = 2;
    printf("Expected edit distance between \"%s\" and \"%s\": %d\n", str1, str2, expected);
    printf("Calculating...\n");
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

// Test 3-length strings. Expected result: 0
void test_3_same() {

    const char* str1 = "abc";
    const char* str2 = "abc";
    size_t len = 3;

    const int expected = 0;
    printf("Expected edit distance between \"%s\" and \"%s\": %d\n", str1, str2, expected);
    printf("Calculating...\n");
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

// Test 3-length strings. Expected result: 2
void test_3_diff() {

    const char* str1 = "abc";
    const char* str2 = "cba";
    size_t len = 3;

    const int expected = 2;
    printf("Expected edit distance between \"%s\" and \"%s\": %d\n", str1, str2, expected);
    printf("Calculating...\n");
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

// Test 8-length strings. Expected result: 3
void test_8_diff() {

    const char* str1 = "elephant";
    const char* str2 = "relevant";
    size_t len = 8;

    const int expected = 3;
    printf("Expected edit distance between \"%s\" and \"%s\": %d\n", str1, str2, expected);
    printf("Calculating...\n");
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

// Test 9-length strings. Expected result: 0
void test_9_same() {

    const char* str1 = "bakabakas";
    const char* str2 = "bakabakas";
    size_t len = 9;

    const int expected = 0;
    printf("Expected edit distance between \"%s\" and \"%s\": %d\n", str1, str2, expected);
    printf("Calculating...\n");
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

// Test 9-length strings. Expected result: 3
void test_9_diff() {

    const char* str1 = "bakabakas";
    const char* str2 = "takatakay";
    size_t len = 9;

    const int expected = 3;
    printf("Expected edit distance between \"%s\" and \"%s\": %d\n", str1, str2, expected);
    printf("Calculating...\n");
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

// Test 128-length strings. Expected result: 0
void test_128_same() {

    const char* str1 = "3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC";
    const char* str2 = "3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC";
    size_t len = 128;

    const int expected = 0;
    printf("Expected edit distance between \"%s\" and \"%s\": %d\n", str1, str2, expected);
    printf("Calculating...\n");
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

// Test 128-length strings, one is reversed. Expected result: 122
void test_128_reversed() {

    const char* str1 = "3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC";
    const char* str2 = "CCzxB67OJ08oDS5MUR3q2xSX9VAIpHeFTgPAla9jVpUPvypA8BLqC4jaqewlrVaIwe3lKTZrLGvn68z75wDABldLFXoKDAGnfmElyojM6wd8Iu1SZn1VIZt7j5HhPyv3";
    size_t len = 128;

    const int expected = 122;
    printf("Expected edit distance between \"%s\" and \"%s\": %d\n", str1, str2, expected);
    printf("Calculating...\n");
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

// Test 128-length strings, one has small differences. Expected result: 2
void test_128_smalldiff() {

    const char* str1 = "3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC";
    const char* str2 = "3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlFARw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC";
    size_t len = 128;

    const int expected = 2;
    printf("Expected edit distance between \"%s\" and \"%s\": %d\n", str1, str2, expected);
    printf("Calculating...\n");
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

// Test 256-length strings. Expected result: 0
void test_256_same() {

    const char* str1 = "3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC";
    const char* str2 = "3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC";
    size_t len = 256;

    const int expected = 0;
    printf("Expected edit distance between \"%s\" and \"%s\": %d\n", str1, str2, expected);
    printf("Calculating...\n");
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

// Test 256-length strings. Expected result: 2
void test_256_small() {

    const char* str1 = "3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC";
    const char* str2 = "3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrKTZl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC";
    size_t len = 256;

    const int expected = 2;
    printf("Expected edit distance between \"%s\" and \"%s\": %d\n", str1, str2, expected);
    printf("Calculating...\n");
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

// Test 256-length strings. Expected result: 231
void test_256_scrambled() {

    const char* str1 = "3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC3vyPhH5j7tZIV1nZS1uI8dw6MjoylEmfnGADKoXFLdlBADw57z86nvGLrZTKl3ewIaVrlweqaj4CqLB8ApyvPUpVj9alAPgTFeHpIAV9XSx2q3RUM5SDo80JO76BxzCC";
    const char* str2 = "opnU0BSVCDrPSlq8PUuzqX6aIwA8376meeMAfIo5MBPvZB6pAoaw3nUpTTlTMx6O1dC1d2j3LEDluIX3F7V4GS9wJAPLaCvVl65HzvCILt9SdAJKB5DVje4jF5yGRTynOHAPG9V8xy17LHDjpwrdMmIfqnvve3j8tEZFy8HlCGIaLDwD9wRAIzjZ5yg1lBXj88gVPxAC8K7ll7qh7aVLqalZXopo2AxolrV3SpI0ezKw5hUyjBnZwKSFnqrvA6eZ";
    size_t len = 256;

    // const int expected = 231;
    const int expected = 240;
    printf("Expected edit distance between \"%s\" and \"%s\": %d\n", str1, str2, expected);
    printf("Calculating...\n");
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

void test_37_same() {

    const char* str1 = "K9JQZiUk4F678E3R9W78nGIb5dRW0FM4aI2Bx";
    const char* str2 = "K9JQZiUk4F678E3R9W78nGIb5dRW0FM4aI2Bx";
    size_t len = 37;

    const int expected = 0;
    printf("Expected edit distance between \"%s\" and \"%s\": %d\n", str1, str2, expected);
    printf("Calculating...\n");
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

void test_37_diff() {

    const char* str1 = "K9JQZiUk4F678E3R9W78nGIb5dRW0FM4aI2Bx";
    const char* str2 = "R4GWW92IQRa7MUBb3xFE9d75K84ZI86nk0JiF";
    size_t len = 37;

    const int expected = 36;
    printf("Expected edit distance between \"%s\" and \"%s\": %d\n", str1, str2, expected);
    printf("Calculating...\n");
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

void test_64_same() {

    const char* str1 = "IaEqFRnXGmNwKBvSyBGPDvKjSGREUfZbYDGBjgTUqjLwXHLFEZpDZVpLjjttPXHJ";
    const char* str2 = "IaEqFRnXGmNwKBvSyBGPDvKjSGREUfZbYDGBjgTUqjLwXHLFEZpDZVpLjjttPXHJ";
    size_t len = 64;

    const int expected = 0;
    printf("Expected edit distance between \"%s\" and \"%s\": %d\n", str1, str2, expected);
    printf("Calculating...\n");
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

void test_64_diff() {

    const char* str1 = "IaEqFRnXGmNwKBvSyBGPDvKjSGREUfZbYDGBjgTUqjLwXHLFEZpDZVpLjjttPXHJ";
    const char* str2 = "IaEqFRnXGmNwKBvSyBGPDvKjSGREUfZbYDGBjjTUqjLwXHLFEZpDZVpLjjttPXHJ";
    size_t len = 64;

    const int expected = 1;
    printf("Expected edit distance between \"%s\" and \"%s\": %d\n", str1, str2, expected);
    printf("Calculating...\n");
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

void test_65_same() {

    const char* str1 = "IaEqFRnXGmNwKBvSyBGPDvKjSGREUfZbYDGBjgTUqjLwXHLFEZpDZVpLjjttPXHJa";
    const char* str2 = "IaEqFRnXGmNwKBvSyBGPDvKjSGREUfZbYDGBjgTUqjLwXHLFEZpDZVpLjjttPXHJa";
    size_t len = 65;

    const int expected = 0;
    printf("Expected edit distance between \"%s\" and \"%s\": %d\n", str1, str2, expected);
    printf("Calculating...\n");
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

void test_65_diff() {

    const char* str1 = "IaEqFRnXGmNwKBvSyBGPDvKjSGREUfZbYDGBjgTUqjLwXHLFEZpDZVpLjjttPXHJb";
    const char* str2 = "IaEqFRnXGmNwKBvSyBGPDvKjSGREUfZbYDGBjgTUqjLwXHLFEZpDZVpLjjttPXHJa";
    size_t len = 65;

    const int expected = 1;
    printf("Expected edit distance between \"%s\" and \"%s\": %d\n", str1, str2, expected);
    printf("Calculating...\n");
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

// Test random strings. Don't actual edit distance.
void test_the_beast() {

    size_t len = 1000000;
    srand((unsigned)time(NULL));
    char *str1 = gen_random_range(len, 'a', 'Z');
    char *str2 = gen_random_range(len, 'a', 'Z');

    printf("Length: %zu\n", len);
    const int result = edit_distance(str1, str2, len);

    free(str1);
    free(str2);

    printf("Calculated edit distance: %d\n", result);

}

void run_tests_small() {
	test_0();
    test_1_same();
    test_1_diff();
    test_2_same();
    test_2_diff();
    test_3_same();
    test_3_diff();
    test_8_diff();
    test_9_same();
    test_9_diff();
    test_37_same();
    test_37_diff();
    test_64_same();
    test_64_diff();
    test_65_same();
    test_65_diff();
    test_128_same();
    test_128_smalldiff();
    test_128_reversed();
    test_256_same();
    test_256_small();
    test_256_scrambled();
}

void run_test_1mil() {
    test_the_beast();
}
