#include "../istack.h"

#include <stdio.h>
#include <stdlib.h>

static int passed = 0, failed = 0;
#define CHECK(name, expression) do { \
    if (expression) { printf("PASS: %s\n", name); ++passed; } \
    else { printf("FAIL: %s\n", name); ++failed; } \
} while (0)

int main(void)
{
    istack_t *s = istack_create();
    if (s == NULL) { puts("FAIL: creation"); return 1; }
    CHECK("new stack starts empty with capacity 8",
          istack_is_empty(s) && istack_count(s) == 0 &&
          istack_capacity(s) == ISTACK_START);

    int value = 9876;
    CHECK("peek empty leaves output alone",
          istack_peek(s, &value) == -1 && value == 9876);
    CHECK("pop empty leaves output alone",
          istack_pop(s, &value) == -1 && value == 9876);
    CHECK("empty calls accept NULL output",
          istack_pop(s, NULL) == -1 && istack_peek(s, NULL) == -1);

    for (int i = 0; i < 8; ++i) {
        if (istack_push(s, i) != 0) { puts("FAIL: push 8 values"); istack_destroy(s); return 1; }
    }
    CHECK("8 values fit without growth", istack_capacity(s) == 8 && istack_count(s) == 8);
    CHECK("9th push doubles to 16", istack_push(s, 8) == 0 &&
          istack_capacity(s) == 16 && istack_count(s) == 9);
    int correct = 1;
    for (int i = 8; i >= 0; --i) {
        int out = -1;
        if (istack_pop(s, &out) != 0 || out != i) correct = 0;
    }
    CHECK("all values retained in last-in-first-out order", correct && istack_is_empty(s));
    CHECK("capacity does not shrink", istack_capacity(s) == 16);

    correct = 1;
    for (int i = 0; i < 1000; ++i)
        if (istack_push(s, i * 2) != 0) correct = 0;
    CHECK("1000 pushes", correct && istack_count(s) == 1000 && istack_capacity(s) >= 1000);
    size_t n = 0;
    int *copy = istack_to_array(s, &n);
    correct = copy != NULL && n == 1000;
    if (correct) {
        for (int i = 0; i < 1000; ++i)
            if (copy[i] != i * 2) correct = 0;
        copy[0] = -999;
        correct = correct && istack_pop(s, NULL) == 0 &&
                  istack_count(s) == 999;
        /* Reaching the bottom must leave the original first element intact. */
        int *fresh = istack_to_array(s, &n);
        correct = correct && fresh != NULL && n == 999 && fresh[0] == 0;
        free(fresh);
    }
    CHECK("array is bottom-first and independently allocated", correct);
    free(copy);
    correct = 1;
    for (int i = 998; i >= 0; --i) {
        int out = -1;
        if (istack_pop(s, &out) != 0 || out != i * 2) correct = 0;
    }
    CHECK("1000 values pop correctly", correct && istack_is_empty(s));
    n = 123;
    copy = istack_to_array(s, &n);
    CHECK("empty array returns NULL and zero", copy == NULL && n == 0);
    free(copy);

    CHECK("NULL arguments are safe", istack_count(NULL) == 0 &&
          istack_is_empty(NULL) && istack_capacity(NULL) == 0 &&
          istack_push(NULL, 1) == -1 && istack_pop(NULL, &value) == -1 &&
          istack_peek(NULL, &value) == -1);
    n = 42;
    CHECK("NULL to_array sets count zero", istack_to_array(NULL, &n) == NULL && n == 0);
    CHECK("NULL output-count pointer safely rejected", istack_to_array(s, NULL) == NULL);
    istack_destroy(NULL);

    istack_t *second = istack_create();
    CHECK("two stacks are created", second != NULL);
    if (second != NULL) {
        CHECK("two stacks are independent", istack_push(s, 55) == 0 &&
              istack_push(second, 66) == 0 &&
              istack_peek(s, &value) == 0 && value == 55 &&
              istack_count(second) == 1);
        istack_destroy(second); /* Intentionally nonempty. */
    }
    istack_destroy(s); /* Intentionally nonempty. */
    printf("Summary: %d passed, %d failed\n", passed, failed);
    return failed == 0 ? 0 : 1;
}
