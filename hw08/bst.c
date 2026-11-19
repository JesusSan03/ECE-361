#include "bst.h"
#include <stdlib.h>

struct node {
    int data;
    struct node *leftChild;
    struct node *rightChild;
};

struct bst {
    struct node *root;
    size_t count;
};

bst_t *bst_create(void)
{
    bst_t *t = malloc(sizeof(*t));
    if (t == NULL) {
        return NULL;
    }
    t->root = NULL;
    t->count = 0;
    return t;
}

static void free_nodes(struct node *n)
{
    if (n == NULL) {
        return;
    }
    free_nodes(n->leftChild);
    free_nodes(n->rightChild);
    free(n);
}

void bst_destroy(bst_t *t)
{
    if (t == NULL) {
        return;
    }
    free_nodes(t->root);
    free(t);
}

int bst_insert(bst_t *t, int key)
{
    if (t == NULL) {
        return -1;
    }

    struct node *tempNode = malloc(sizeof(*tempNode));
    if (tempNode == NULL) {
        return -1;
    }
    tempNode->data = key;
    tempNode->leftChild = NULL;
    tempNode->rightChild = NULL;

    if (t->root == NULL) {
        t->root = tempNode;
    } else {
        struct node *current = t->root;
        struct node *parent = NULL;

        while (1) {
            parent = current;
            if (key < parent->data) {
                current = current->leftChild;
                if (current == NULL) {
                    parent->leftChild = tempNode;
                    break;
                }
            } else {
                current = current->rightChild;
                if (current == NULL) {
                    parent->rightChild = tempNode;
                    break;
                }
            }
        }
    }
    t->count++;
    return 0;
}


bool bst_contains(const bst_t *t, int key, size_t *visits)
{
    if (visits != NULL) {
        *visits = 0;
    }
    if (t == NULL) {
        return false;
    }

    const struct node *current = t->root;
    while (current != NULL) {
        if (visits != NULL) {
            (*visits)++;
        }
        if (current->data == key) {
            return true;
        }
        if (current->data > key) {
            current = current->leftChild;
        } else {
            current = current->rightChild;
        }
    }
    return false;
}

size_t bst_size(const bst_t *t)
{
    return t == NULL ? 0 : t->count;
}

static size_t height_of(const struct node *n)
{
    if (n == NULL) {
        return 0;
    }
    size_t left = height_of(n->leftChild);
    size_t right = height_of(n->rightChild);
    return 1 + (left > right ? left : right);
}

size_t bst_height(const bst_t *t)
{
    return t == NULL ? 0 : height_of(t->root);
}

int bst_remove(bst_t *t, int key)
{
    if (t == NULL) {
        return -1;
    }

    struct node **place = &t->root;
    while (*place != NULL && (*place)->data != key) {
        if (key < (*place)->data) {
            place = &(*place)->leftChild;
        } else {
            place = &(*place)->rightChild;
        }
    }
    if (*place == NULL) {
        return -1;
    }

    struct node *victim = *place;
    if (victim->leftChild == NULL) {
        *place = victim->rightChild;
        free(victim);
    } else if (victim->rightChild == NULL) {
        *place = victim->leftChild;
        free(victim);
    } else {
        struct node **smallest = &victim->rightChild;
        while ((*smallest)->leftChild != NULL) {
            smallest = &(*smallest)->leftChild;
        }
        struct node *next = *smallest;
        victim->data = next->data;
        *smallest = next->rightChild;
        free(next);
    }
    t->count--;
    return 0;
}
