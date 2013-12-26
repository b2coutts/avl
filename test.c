// testing harness for my AVL implementation
#include "avl.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX(x,y) ( (x<y) ? y : x)
#define ABS(x) ( (x<0) ? -x : x)
#define HL(x) ( (x->l ? x->l->h : -1) )
#define HR(x) ( (x->r ? x->r->h : -1) )

#define KEYSIZE 10
#define TEST_SIZE 100000


int proper_heights(struct node *n){
    if(!n) return 1;
    int lh = !n->l || proper_heights(n->l);
    int rh = !n->r || proper_heights(n->r);

    return lh && rh && n->h == MAX(HL(n), HR(n))+1;
}

int is_avl(struct node *n){
    if(!n) return 1;
    if(ABS(HL(n) - HR(n)) > 1) return 0;
    return is_avl(n->l) && is_avl(n->r);
}

int is_bst(struct node *n){
    if(!n) return 1;
    if(n->l && strcmp(n->l->k, n->k) >= 0) return 0;
    if(n->r && strcmp(n->r->k, n->k) <= 0) return 0;
    return is_bst(n->l) && is_bst(n->r);
}

void adv_key(char *str){
    int r;
    for(int i = 0; i < KEYSIZE; i++){
        r = rand()%26;
        str[i] += r;
        if(str[i] > 'Z') str[i] -= 26;
    }
}

int nnodes(struct node *n){
    if(!n) return 0;
    return nnodes(n->l) + nnodes(n->r) + 1;
}

int calcheight(struct node *n){
    if(!n) return -1;
    return MAX(calcheight(n->l), calcheight(n->r)) + 1;
}

int main(){
    srand(0xDEADBEEF);
    char *s = strdup("CHARACTERS");
    struct AVLTree *t = avl_create();
    
    // populate AVL tree with TEST_SIZE nodes
    printf("Populating tree with %i nodes...", TEST_SIZE); fflush(stdout);
    char **keys = malloc(sizeof(char*) * TEST_SIZE * 2);
    for(unsigned int i = 0; i < TEST_SIZE; i++){
        adv_key(s);
        keys[i] = strdup(s);
        avl_insert(t, s, (void*)i);
    }
    printf("done.\n");

    //node_print(t->root, 0);

    // perform TEST_SIZE delete/insert operations
    printf("Performing %i deletions/inserts...", TEST_SIZE); fflush(stdout);
    unsigned int ind = TEST_SIZE;
    for(unsigned int i = 0; i < TEST_SIZE; i++){
        int node = ind++;
        if(rand()%2){ // delete a node with 50% probability
            //printf("deleting something\n");
            node = rand()%(--ind);
            avl_delete(t, keys[node]);
            free(keys[node]);
        }
        adv_key(s);
        keys[node] = strdup(s);
        avl_insert(t, s, (void*)node);
    //printf("inserting %s[%i]\n", s, node);
    //node_print(t->root, 0);
    }
    printf("done.\n");

    // verify resulting tree
    printf("Testing height label correctness...");
    int hcheck = proper_heights(t->root);
    printf("%s\n", hcheck ? "PASS" : "FAIL");

    printf("Testing AVL balance correctness...");
    int acheck = is_avl(t->root);
    printf("%s\n", acheck ? "PASS" : "FAIL");

    printf("Testing BST correctness...");
    int bcheck = is_bst(t->root);
    printf("%s\n", bcheck ? "PASS" : "FAIL");

    printf("Testing value correctness...");
    unsigned int correct = 0;
    for(int i = 0; i < ind; i++){
        if(avl_lookup(t, keys[i]) == (void*)i) correct++;
    }
    printf("Score: %i/%i\n", correct, ind);

    //node_print(t->root, 0);
}
