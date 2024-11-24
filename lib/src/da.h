#ifndef __DA__
#define __DA__
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 8

typedef struct DA {
  int size;
  int capacity;
  void **items;
} DA;



DA *DA_new();
void DA_free(DA *da);
void DA_GROW(DA *da);
void DA_shrink(DA *da);
int DA_push(DA *da, void *x);
void *DA_pop(DA *da);
void *DA_get(DA *da, int idx);
void *DA_set(DA *da, int idx, void *x);
int DA_size(DA *da);


#endif
