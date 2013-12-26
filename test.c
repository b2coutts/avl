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
#define TEST_SIZE 1000

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
        //printf("inserted %s[%i]\n.", s, i);
    }
    printf("done.\n");

    // perform TEST_SIZE delete/insert operations
    printf("Performing random deletions/inserts..."); fflush(stdout);
    unsigned int ind = TEST_SIZE;
    for(unsigned int i = 0; i < TEST_SIZE; i++){
        printf("starting loop, ind is %i\n", ind);
        int node = ind++;
        if(rand()%2){ // delete a node with 50% probability
        printf("ind is %i\n", ind);
            node = rand()%(--ind);
        printf("deleting %s[%i]\n", keys[node], node);
            avl_delete(t, keys[node]);
            free(keys[node]);
        }
        adv_key(s);
        keys[node] = strdup(s);
        avl_insert(t, s, (void*)node);
        printf("inserted %s[%i]\n.", s, node);
    }
    printf("done.");

    // verify resulting tree
    printf("Testing height label correctness...\n");
    int hcheck = proper_heights(t->root);
    printf("%s\n", hcheck ? "PASS" : "FAIL");

    printf("Testing AVL balance correctness...\n");
    int acheck = is_avl(t->root);
    printf("%s\n", acheck ? "PASS" : "FAIL");

    printf("Testing BST correctness...\n");
    int bcheck = is_bst(t->root);
    printf("%s\n", bcheck ? "PASS" : "FAIL");

    printf("Testing value correctness...\n");
    unsigned int correct = 0;
    for(int i = 0; i < ind; i++){
        if(avl_lookup(t, keys[i]) == (void*)i) correct++;
    }
    printf("Score: %i/%i\n", correct, ind);
}
