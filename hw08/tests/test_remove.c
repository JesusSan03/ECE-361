#include "bst.h"

#include <stdbool.h>
#include <stdio.h>

static int failures = 0;

static void check(bool good, const char *name)
{
    printf("%s: %s\n", good ? "PASS" : "FAIL", name);
    if (!good) {
        failures++;
    }
}

static bst_t *original_tree(void)
{
    const int keys[] = {27, 14, 35, 10, 19, 31, 42};
    bst_t *tree = bst_create();
    if (tree == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
        if (bst_insert(tree, keys[i]) != 0) {
            bst_destroy(tree);
            return NULL;
        }
    }
    return tree;
}

static void test_original_sequence(void)
{
    bst_t *tree = original_tree();
    if (tree == NULL) {
        check(false, "make original tree");
        return;
    }

    check(bst_remove(tree, 10) == 0 && bst_size(tree) == 6 &&
          !bst_contains(tree, 10, NULL) && bst_height(tree) == 3,
          "remove leaf 10");

    check(bst_remove(tree, 14) == 0 && bst_size(tree) == 5 &&
          !bst_contains(tree, 14, NULL) && bst_contains(tree, 19, NULL),
          "remove one-child node 14");

    size_t visits = 0;
    check(bst_remove(tree, 27) == 0 && bst_size(tree) == 4 &&
          !bst_contains(tree, 27, NULL) &&
          bst_contains(tree, 31, &visits) && visits == 1 &&
          bst_height(tree) == 3,
          "remove two-child root 27 using successor 31");

    check(bst_remove(tree, 99) == -1 && bst_size(tree) == 4,
          "missing key does not change tree");
    bst_destroy(tree);
}

static void test_duplicates_and_root(void)
{
    bst_t *tree = bst_create();
    if (tree == NULL) {
        check(false, "make duplicate tree");
        return;
    }
    bool ok = bst_insert(tree, 5) == 0 &&
              bst_insert(tree, 5) == 0 && bst_insert(tree, 5) == 0;
    ok = ok && bst_remove(tree, 5) == 0 && bst_size(tree) == 2;
    ok = ok && bst_contains(tree, 5, NULL);
    ok = ok && bst_remove(tree, 5) == 0 && bst_size(tree) == 1;
    ok = ok && bst_remove(tree, 5) == 0 && bst_size(tree) == 0;
    ok = ok && !bst_contains(tree, 5, NULL) && bst_height(tree) == 0;
    ok = ok && bst_remove(tree, 5) == -1;
    check(ok, "remove one duplicate at a time, including root");
    bst_destroy(tree);
}

static void test_successor_with_child(void)
{
    bst_t *tree = bst_create();
    if (tree == NULL) {
        check(false, "make successor tree");
        return;
    }
    const int keys[] = {20, 10, 40, 30, 50, 35};
    bool ok = true;
    for (size_t i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
        if (bst_insert(tree, keys[i]) != 0) {
            ok = false;
        }
    }
    ok = ok && bst_remove(tree, 20) == 0;
    ok = ok && bst_size(tree) == 5 && !bst_contains(tree, 20, NULL);
    ok = ok && bst_contains(tree, 30, NULL) && bst_contains(tree, 35, NULL);
    check(ok, "successor with a right child stays connected");
    bst_destroy(tree);
}

static void test_two_child_duplicates(void)
{
    bst_t *tree = bst_create();
    if (tree == NULL) {
        check(false, "make two-child duplicate tree");
        return;
    }
    const int keys[] = {20, 10, 30, 25, 25, 40};
    bool ok = true;
    for (size_t i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
        if (bst_insert(tree, keys[i]) != 0) {
            ok = false;
        }
    }
    ok = ok && bst_remove(tree, 20) == 0 && bst_size(tree) == 5;
    ok = ok && bst_contains(tree, 25, NULL);
    ok = ok && bst_remove(tree, 25) == 0 && bst_size(tree) == 4;
    ok = ok && bst_contains(tree, 25, NULL);
    ok = ok && bst_remove(tree, 25) == 0 && bst_size(tree) == 3;
    ok = ok && !bst_contains(tree, 25, NULL);
    check(ok, "two-child removal keeps duplicates searchable");
    bst_destroy(tree);
}

int main(void)
{
    check(bst_remove(NULL, 1) == -1, "NULL tree cannot remove a key");
    test_original_sequence();
    test_duplicates_and_root();
    test_successor_with_child();
    test_two_child_duplicates();
    return failures == 0 ? 0 : 1;
}
