#include "avl.h"
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

/************** helper functions ***************/

// return the max of two integers
int max(int a, int b){
    return a>b ? a : b;
}

// update the height of a node
void update_height(struct node *n){
    int rec = 0;
    if(n->l) rec = max(rec, n->l->h + 1);
    if(n->r) rec = max(rec, n->r->h + 1);
    n->h = rec;
}

// deallocate a node
void node_destroy(struct node *n){
    if(n->l) node_destroy(n->l);
    if(n->r) node_destroy(n->r);
    free(n);
}

// create a node
struct node *node_create(char *key, void *data){
    struct node *n = malloc(sizeof(struct node));
    n->l = 0;
    n->r = 0;
    n->k = key;
    n->d = data;
    n->h = 0;
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
struct node *node_insert(struct node *n, char *key, void *data){
    int height = n->h;
    int cmp = strcmp(n->k, key);
    if(cmp == 0) return n;
    if(cmp < 0){
        if(n->r){
            node_insert(n->r, key, data);
            update_height(n);
            n = rebalance(n);
        }else{
            n->r = node_create(key, data);
            update_height(n);
        }
    }else if(cmp > 0){
        if(n->l){
            node_insert(n->l, key, data);
            update_height(n);
            n = rebalance(n);
        }else{
            n->l = node_create(key, data);
            update_height(n);
        }
    }
    return n;
}

// lookup a value in a node
void *node_lookup(struct node *n, char *key){
    if(!n) return 0;
    int cmp = strcmp(n->k, key);
    if(cmp == 0) return n->d;
    if(cmp < 0) return node_lookup(n->r, key);
    return node_lookup(n->l, key);
}

// delete a value in a node
void node_delete(struct node *n, char *key){
    if(!n) return;
    int cmp = strcmp(n->k, key);
    if(cmp == 0){
        
    }
    if(cmp < 0) node_delete(n->r, key);
    if(cmp > 0) node_delete(n->l, key);
}

/************** implementation functions ***************/

struct AVLTree* avl_create(){
    struct AVLTree *t = malloc(sizeof(struct AVLTree));
    t->root = 0;
    t->nodes = 0;
    return t;
}

void avl_destroy(AVLTree *tree){
    node_destroy(tree->root);
    free(tree);
}
    

int32_t avl_insert(struct AVLTree *avl, char *key, void* data){
    if(avl->root){
        avl->root = node_insert(avl->root, key, data);
    }else{
        avl->root = node_create(key, data);
    }
    return 0;
}

void *avl_lookup(AVLTree *tree, char *key){
    return node_lookup(tree->root, key);
}

/************** testing code *******************/

// print an AVL node visually
void node_print(struct node *n, int ind){
    for(int i = 0; i < ind; i++){
        printf("  ");
    }

    if(n){
        printf("%s(%i)[%i]\n", n->k, n->h, n->d);
        node_print(n->l, ind+1);
        node_print(n->r, ind+1);
    }else{
        printf("~\n");
    }
}

int main(){
    AVLTree *t = avl_create();
    printf("one\n");

    avl_insert(t, "asdf", (void*)100);
    printf("two\n");
    avl_insert(t, "fdsa", (void*)200);

    node_print(t->root, 0);

    printf("three\n");

    avl_insert(t, "zdfs", (void*)300);

    printf("in main:\n");
    node_print(t->root, 1);

    printf("four\n");
    avl_insert(t, "aasdf", (void*)400);
    printf("five\n");
    avl_insert(t, "dicks", (void*)500);
    printf("six\n");

    //printf("root is %s\n", t->root->k);

    node_print(t->root, 0);
}
