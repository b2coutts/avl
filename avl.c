#include "avl.h"
#include <stdlib.h>
#include <string.h>

#define MAX(x,y) ( (x<y) ? y : x)
#define HL(x) ( (x->l ? x->l->h : -1) )
#define HR(x) ( (x->r ? x->r->h : -1) )

/************** helper functions/definitions ***************/

typedef struct munge {
    struct node *n;
    char *k;
    val_t d;
} munge;

// update the height of a node
void update_height(struct node *n){
    n->h = MAX(HL(n), HR(n))+1;
}

// deallocate a node
void node_destroy(struct node *n, int recurse){
    free(n->k);
    if(recurse && n->l) node_destroy(n->l, 1);
    if(recurse && n->r) node_destroy(n->r, 1);
    free(n);
}

// create a node
struct node *node_create(char *key, val_t data){
    struct node *n = malloc(sizeof(struct node));
    n->l = 0;
    n->r = 0;
    n->k = malloc(strlen(key)+1);
    strcpy(n->k, key);
    n->d = data;
    n->h = 0;
    return n;
}

// left rotation
struct node *lrot(struct node *n){
    struct node *oldright = n->r;
    n->r = n->r->l;
    oldright->l = n;

    update_height(n);
    update_height(oldright);
    return oldright;
}

// right rotation
struct node *rrot(struct node *n){
    struct node *oldleft = n->l;
    n->l = n->l->r;
    oldleft->r = n;

    update_height(n);
    update_height(oldleft);
    return oldleft;
}

// rebalance a node (assumed node is unbalanced by at most 1; heights differ <2)
struct node *rebalance(struct node *n){
    if(!n) return n;

    int lh = n->l ? n->l->h : -1;
    int rh = n->r ? n->r->h : -1;

    if(lh == rh + 2){
        if(n->l->l && n->l->l->h == rh+1){ // right rotation
            n = rrot(n);
        }else if(n->l->r && n->l->r->h == rh+1){ // left-right rotation
            n->l = lrot(n->l);
            n = rrot(n);
        }
    }else if(rh == lh + 2){
        if(n->r->r && n->r->r->h == lh+1){ // left rotation
            n = lrot(n);
        }else if(n->r->l && n->r->l->h == lh+1){ // right-left rotation
            n->r = rrot(n->r);
            n = lrot(n);
        }
    }

    return n;
}

// insert a kv pair into a node
struct node *node_insert(struct node *n, char *key, val_t data){
    int cmp = strcmp(n->k, key);
    if(cmp < 0)
        n->r = n->r ? node_insert(n->r, key, data) : node_create(key, data);
    else if(cmp > 0)
        n->l = n->l ? node_insert(n->l, key, data) : node_create(key, data);
    update_height(n);
    n = rebalance(n);
    return n;
}

// lookup a value in a node
val_t *node_lookup(struct node *n, char *key){
    if(!n) return 0;
    int cmp = strcmp(n->k, key);
    if(cmp < 0) return node_lookup(n->r, key);
    else if(cmp > 0) return node_lookup(n->l, key);
    else return &(n->d);
}

// remove the rightmost node of a nonempty tree while maintaining balance;
// return the new tree, and the key/data pair of the removed node
struct munge get_rightmost(struct node *n){
    if(n->r){
        struct munge m = get_rightmost(n->r);
        n->r = m.n;
        update_height(n);
        m.n = rebalance(n);
        return m;
    }else{
        struct munge m;
        m.n = n->l;
        m.k = n->k;
        m.d = n->d;
        free(n);
        return m;
    }
}

// delete a value in a node
struct node *node_delete(struct node *n, char *key){
    if(!n) return 0;
    int cmp = strcmp(n->k, key);
    if(cmp == 0){ // delete the root node
        if(!n->l){
            struct node *r = n->r;
            node_destroy(n, 0);
            return r;
        }else if(!n->r){
            struct node *l = n->l;
            node_destroy(n, 0);
            return l;
        }else{
            struct munge m = get_rightmost(n->l);
            free(n->k);
            n->l = m.n;
            n->k = m.k;
            n->d = m.d;
            update_height(n);
            return rebalance(n);
        }
    }
    if(cmp < 0) n->r = node_delete(n->r, key);
    if(cmp > 0) n->l = node_delete(n->l, key);
    update_height(n);
    return rebalance(n);
}

/************** implementation functions ***************/

struct AVLTree* avl_create(){
    struct AVLTree *t = malloc(sizeof(struct AVLTree));
    t->root = 0;
    return t;
}

void avl_destroy(struct AVLTree *tree){
    node_destroy(tree->root, 1);
    free(tree);
}
    

void avl_insert(struct AVLTree *avl, char *key, val_t data){
    if(avl->root){
        avl->root = node_insert(avl->root, key, data);
    }else{
        avl->root = node_create(key, data);
    }
}

void avl_delete(struct AVLTree *avl, char *key){
    avl->root = node_delete(avl->root, key);
}

val_t *avl_lookup(struct AVLTree *tree, char *key){
    return node_lookup(tree->root, key);
}
