#include "garbage_collector.h"
#include <stdlib.h>
#include <errno.h>

typedef struct node node_t;

struct node {
    void * pointer;
    node_t * next;
};

typedef struct{
    node_t * list;
    node_t * last;
    size_t size;
} garbage_collector_t;

static garbage_collector_t gc = {0};

void * Malloc(size_t size){
    void * aux = malloc(size);
    if(aux == NULL || errno == ENOMEM) {
        return NULL;
    }
    struct node * node = malloc(sizeof(struct node));
    if(node == NULL || errno == ENOMEM) {
        free(aux);
        return NULL;
    }
    if(gc.list == NULL){
        gc.list = node;
        gc.last = node;
        node->pointer = aux;
        node->next = NULL;
        return aux;
    }
    //tengo que agregarlo al final
    gc.last->next = node;
    node->next = NULL;
    node->pointer = aux;
    gc.last = node;
    return aux;
}

void * Calloc(size_t count, size_t size){
    void * aux = calloc(count, size);
    if(aux == NULL || errno == ENOMEM) {
        return NULL;
    }
    struct node * node = malloc(sizeof(struct node));
    if(node == NULL || errno == ENOMEM) {
        free(aux);
        return NULL;
    }
    if(gc.list == NULL){
        gc.list = node;
        gc.last = node;
        node->pointer = aux;
        node->next = NULL;
        return aux;
    }
    //tengo que agregarlo al final
    gc.last->next = node;
    node->next = NULL;
    node->pointer = aux;
    gc.last = node;
    return aux;
}

void* Realloc(void* ptr, size_t size){
    node_t * curr = gc.list;
    while(curr!=NULL && curr->pointer != ptr){
        curr = curr->next;
    }
    if(curr == NULL){
        //No lo encontramos, reservamos memoria
        return Malloc(size);
    }

    void * aux = realloc(ptr, size);
    if(aux == NULL || errno == ENOMEM) {
        free(ptr);
        return NULL;
    }
    curr->pointer = aux;
    return aux;
}

void Free_All(){
    if(gc.list == NULL){
        return;
    }
    node_t * curr = gc.list;
    while(curr != NULL){
        node_t* aux = curr->next;
        free(curr->pointer);
        free(curr);
        curr = aux;
    }
}
