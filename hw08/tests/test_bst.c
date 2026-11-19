#include "bst.h"

#include <stdbool.h>
#include <stdio.h>

static bool test_original_tree(void)
{
    const int keys[] = {27, 14, 35, 10, 19, 31, 42};
    bst_t *tree = bst_create();
    if (tree == NULL) {
        return false;
    }

    bool ok = true;
    for (size_t i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
        if (bst_insert(tree, keys[i]) != 0) {
            ok = false;
        }
    }

    size_t visits = 0;
    ok = ok && bst_size(tree) == 7 && bst_height(tree) == 3;
    ok = ok && bst_contains(tree, 31, &visits) && visits == 3;
    ok = ok && !bst_contains(tree, 15, &visits) && visits == 3;
    ok = ok && bst_contains(tree, 27, NULL);
    bst_destroy(tree);
    return ok;
}

static bool test_duplicates(void)
{
    bst_t *tree = bst_create();
    if (tree == NULL) {
        return false;
    }
    bool ok = bst_insert(tree, 5) == 0 && bst_insert(tree, 5) == 0;
    ok = ok && bst_size(tree) == 2 && bst_height(tree) == 2;
    size_t visits = 0;
    ok = ok && bst_contains(tree, 5, &visits) && visits == 1;
    bst_destroy(tree);
    return ok;
}

static bool test_increasing_order(void)
{
    bst_t *tree = bst_create();
    if (tree == NULL) {
        return false;
    }
    bool ok = true;
    for (int i = 1; i <= 50; i++) {
        if (bst_insert(tree, i) != 0) {
            ok = false;
        }
    }
    size_t visits = 0;
    ok = ok && bst_size(tree) == 50 && bst_height(tree) == 50;
    ok = ok && bst_contains(tree, 50, &visits) && visits == 50;
    bst_destroy(tree);
    return ok;
}

static bool test_null_tree(void)
{
    size_t visits = 99;
    bool ok = bst_insert(NULL, 10) == -1;
    ok = ok && !bst_contains(NULL, 10, &visits) && visits == 0;
    ok = ok && bst_size(NULL) == 0 && bst_height(NULL) == 0;
    bst_destroy(NULL);
    return ok;
}

static bool test_empty_tree(void)
{
    bst_t *tree = bst_create();
    if (tree == NULL) {
        return false;
    }
    size_t visits = 123;
    bool ok = bst_size(tree) == 0 && bst_height(tree) == 0;
    /* This is the case that crashed the original search implementation. */
    ok = ok && !bst_contains(tree, 99, &visits) && visits == 0;
    bst_destroy(tree);
    return ok;
}

static int failed = 0;

static void run_test(const char *name, bool (*test)(void))
{
    bool passed = test();
    printf("%s: %s\n", passed ? "PASS" : "FAIL", name);
    fflush(stdout);
    if (!passed) {
        failed++;
    }
}

int main(void)
{
    run_test("seven keys, height, and visits", test_original_tree);
    run_test("duplicate keys are kept", test_duplicates);
    run_test("sorted insertion makes height 50", test_increasing_order);
    run_test("NULL tree arguments", test_null_tree);
    run_test("search an empty tree (original crash)", test_empty_tree);
    return failed == 0 ? 0 : 1;
}
