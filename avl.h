// avl.h
#ifndef AVLTREE
#define AVLTREE

#include <stdint.h>

typedef struct AVLTree {
    struct node *root;
} AVLTree;

// assumes string keys
typedef struct node {
    struct node *l;
    struct node *r;
    char *k;
    void *d; // data

    // height: length of longest path downwards. Height of an empty tree is -1.
    // Height of a singleton is 0.
    int32_t h; 
} node;

struct AVLTree* avl_create();

void avl_insert(struct AVLTree *avl, char *key, void *data);
void *avl_lookup(struct AVLTree *avl, char *key);
void avl_delete(struct AVLTree *avl, char *key);
void avl_destroy(struct AVLTree *avl);

#endif
