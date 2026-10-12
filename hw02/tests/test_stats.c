
#include <stdio.h>
#include <math.h>
#include "../stats.h"

/*
 * Checks one test condition.
 * Updates the pass or fail counter.
 * Assumes both counter pointers are valid.
 */
static void check(const char *name, int condition,
                  int *passed, int *failed) {
    if (condition) {
        printf("PASS: %s\n", name);
        (*passed)++;
    } else {
        printf("FAIL: %s\n", name);
        (*failed)++;
    }
}

int main(void) {
    int passed = 0;
    int failed = 0;

    /* Test 1: Maximum at the beginning and end */
    float a[] = {9.0f, 3.0f, 2.0f, 1.0f};
    float b[] = {1.0f, 2.0f, 3.0f, 9.0f};

    check("maximum at start",
          stats_max(a, 4) == 9.0f,
          &passed, &failed);

    check("maximum at end",
          stats_max(b, 4) == 9.0f,
          &passed, &failed);

    /* Test 2: Value exactly at threshold */
    float c[] = {30.0f, 31.0f, 30.0f, 32.0f};
    int start = -1;

    int length = stats_longest_run_above(
        c, 4, 30.0f, &start);

    check("threshold equality",
          length == 1 && start == 1,
          &passed, &failed);

    /* Test 3: Recursive maximum of 10,000 values */
    float large[10000];

    for (int i = 0; i < 10000; i++) {
        large[i] = (float)i;
    }

    check("recursive maximum 10000",
          stats_max(large, 10000) == 9999.0f,
          &passed, &failed);

    /* Test 4: Minimum */
    float d[] = {4.0f, 8.0f, 12.0f};

    check("minimum",
          stats_min(d, 3) == 4.0f,
          &passed, &failed);

    /* Test 5: Mean */
    check("mean",
          fabsf(stats_mean(d, 3) - 8.0f) < 0.0001f,
          &passed, &failed);

    printf("\nSummary: %d passed, %d failed\n",
           passed, failed);

    return failed == 0 ? 0 : 1;
}
