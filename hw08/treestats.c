#include "bst.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    bst_t *tree = bst_create();
    if (tree == NULL) {
        fputs("could not make tree\n", stderr);
        return 1;
    }

    char word[64];
    size_t values = 0;
    while (scanf("%63s", word) == 1) {
        char *end;
        errno = 0;
        long number = strtol(word, &end, 10);

        if (word[0] == '\0' || *end != '\0' || errno == ERANGE ||
            number < INT_MIN || number > INT_MAX) {
            printf("not a number after %zu values\n", values);
            bst_destroy(tree);
            return 1;
        }

        if (bst_insert(tree, (int)number) != 0) {
            fputs("could not insert value\n", stderr);
            bst_destroy(tree);
            return 1;
        }
        values++;
    }

    if (bst_size(tree) == 0) {
        puts("empty tree");
    } else {
        printf("n %zu, height %zu\n", bst_size(tree), bst_height(tree));
    }
    bst_destroy(tree);
    return 0;
}
