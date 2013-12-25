// avl.h
#ifndef AVLTREE
#define AVLTREE

#include <stdint.h>

typedef struct AVLTree {
    struct node *root;
    int32_t nodes;
} AVLTree;

// assumes string keys
typedef struct node {
    struct node *l;
    struct node *r;
    char *k;
    void* d; // data
    // height: length of longest path downwards. Height of an empty tree is -1. 
    // Height of a singleton is 0.
    int32_t h; 
} node;

struct AVLTree* avl_create();

int32_t avl_insert(struct AVLTree *avl, char *key, void* data);
void* avl_lookup(AVLTree *tree, char *key);
int32_t avl_delete(struct AVLTree *avl, char *key);
void avl_destroy(AVLTree *tree);

//void avl_print(node *tree);
#endif
