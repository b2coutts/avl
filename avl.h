// avl.h
#ifndef AVLTREE
#define AVLTREE

#include <stdint.h>

// datatype of value used
typedef int val_t;

typedef struct AVLTree {
    struct node *root;
} AVLTree;

// assumes string keys
typedef struct node {
    struct node *l; // left branch
    struct node *r; // right branch
    char *k;        // key
    val_t d;        // data

    // height: length of longest path downwards. Height of an empty tree is -1.
    // Height of a singleton is 0.
    int32_t h; 
} node;

// create a new AVL tree
struct AVLTree* avl_create();

// insert the key, value pair (key, data) into the AVL Tree avl
void avl_insert(struct AVLTree *avl, char *key, val_t data);

// return a pointer to the value in avl paired with key; return 0 if key is not
// found in avl
val_t *avl_lookup(struct AVLTree *avl, char *key);

// remove the node in avl by key, if it exists
void avl_delete(struct AVLTree *avl, char *key);

// deallocate the memory used by avl
void avl_destroy(struct AVLTree *avl);

#endif
