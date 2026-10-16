#include "../ptr.h"
#include <stddef.h>
#include <stdio.h>

static int passed = 0;
static int failed = 0;

static void check(const char *name, int condition)
{
    printf("%s: %s\n", condition ? "PASS" : "FAIL", name);
    if (condition) {
        passed++;
    } else {
        failed++;
    }
}

int main(void)
{
    float x = 1.5f, y = 7.0f;
    swap_float(&x, &y);
    check("swap_float", x == 7.0f && y == 1.5f);

    float values[] = {4.0f, -2.0f, 9.0f, 3.0f};
    float min = 123.0f, max = 456.0f;
    check("minmax several values",
          minmax(values, 4, &min, &max) == 0 &&
          min == -2.0f && max == 9.0f);

    float one[] = {6.0f};
    check("minmax one value",
          minmax(one, 1, &min, &max) == 0 &&
          min == 6.0f && max == 6.0f);

    min = 123.0f;
    max = 456.0f;
    check("minmax n zero leaves outputs unchanged",
          minmax(values, 0, &min, &max) == -1 &&
          min == 123.0f && max == 456.0f);
    check("minmax NULL array leaves outputs unchanged",
          minmax(NULL, 4, &min, &max) == -1 &&
          min == 123.0f && max == 456.0f);
    check("minmax NULL min leaves other output unchanged",
          minmax(values, 4, NULL, &max) == -1 && max == 456.0f);
    check("minmax NULL max leaves other output unchanged",
          minmax(values, 4, &min, NULL) == -1 && min == 123.0f);

    float search[] = {1.0f, 7.0f, 8.0f, 3.0f};
    check("find_above two matches returns first",
          find_above(search, 4, 6.0f) == search + 1);
    check("find_above no match",
          find_above(search, 4, 10.0f) == NULL);
    check("find_above equal threshold is not above",
          find_above(search, 4, 8.0f) == NULL);
    check("find_above NULL", find_above(NULL, 4, 1.0f) == NULL);
    check("find_above n zero", find_above(search, 0, 1.0f) == NULL);

    float even[] = {1.0f, 2.0f, 3.0f, 4.0f};
    reverse(even, 4);
    check("reverse even length",
          even[0] == 4.0f && even[1] == 3.0f &&
          even[2] == 2.0f && even[3] == 1.0f);

    float odd[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    reverse(odd, 5);
    check("reverse odd length",
          odd[0] == 5.0f && odd[1] == 4.0f && odd[2] == 3.0f &&
          odd[3] == 2.0f && odd[4] == 1.0f);
    reverse(NULL, 5);
    reverse(odd, 1);
    check("reverse NULL and one element", odd[0] == 5.0f);

    printf("\nSummary: %d passed, %d failed\n", passed, failed);
    return failed == 0 ? 0 : 1;
}
