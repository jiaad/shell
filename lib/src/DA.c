#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "da.h"



DA *DA_new() {
  DA *da = malloc(sizeof(DA));
  da->size = 0;
  da->capacity = INITIAL_CAPACITY;
  da->items = malloc(INITIAL_CAPACITY * sizeof(void *));
  return da;
}

void DA_free(DA *da) {
  free(da->items);
  free(da);
}

void DA_GROW(DA *da) {
  da->capacity *= 2;
  da->items = realloc(da->items, da->capacity * sizeof(void *));
}

void DA_shrink(DA *da) {
  da->capacity /= 2;
  da->items = realloc(da->items, da->capacity * sizeof(void *));
}

int DA_push(DA *da, void *x) {
  if (da->size >= da->capacity)
    DA_GROW(da);

  da->items[da->size] = x;
  da->size += 1;
  return da->size;
}

void *DA_pop(DA *da) {
  if (da->size <= 0)
    return NULL;

  if (da->capacity > (da->size * 2))
    DA_shrink(da);

  da->size -= 1;
  return da->items[da->size];
}

void *DA_get(DA *da, int idx) {
  if (idx >= da->size)
    return NULL;
  return da->items[idx];
}

void *DA_set(DA *da, int idx, void *x) {
  if (idx >= da->size)
    return NULL;
  da->items[idx] = x;
  return x;
}

int DA_size(DA *da) { return da->size; }

#ifdef TEST
int main(void) {
  DA *da = DA_new();

  int twelve = 12;
  int thirteen = 13;
  int fourteen = 14;
  int fifteen = 15;
  int sixteen = 16;
  int seventeen = 17;
  int eightteen = 18;
  int nineteen = 19;
  int twenty = 20;
  int twentyone = 21;
  int twentytwo = 22;
  char twentythree[] = "twentythree";
  float flt = 12.4;
  // DA_push(da, (void*)12);


  DA_push(da, &twelve);
  DA_push(da, &thirteen);
  DA_push(da, &fourteen);
  DA_push(da, &fifteen);
  DA_push(da, &sixteen);
  DA_push(da, &seventeen);
  DA_push(da, &eightteen);
  DA_push(da, &nineteen);
  DA_push(da, &twenty);
  DA_push(da, &twentyone);
  DA_push(da, &twentytwo);
  DA_push(da, &twentythree);
  DA_push(da, &flt);
  // // assert(((int*)da->items)[0] == 12);
  // printf("res[0] = twelve -> %d\n", *(int*)da->items[0]);
  // // printf("res[0] -> %d\n", ((int*)da->items)[0]);
  assert(*(int *)da->items[0] == twelve);
  assert(*(int *)da->items[1] == thirteen);
  assert(*(int *)da->items[2] == fourteen);
  assert(*(int *)da->items[3] == fifteen);
  assert(*(int *)da->items[4] == sixteen);
  assert(*(int *)da->items[5] == seventeen);
  assert(*(int *)da->items[6] == eightteen);
  assert(*(int *)da->items[7] == nineteen);
  assert(*(int *)da->items[8] == twenty);
  assert(*(int *)da->items[9] == twentyone);
  assert(*(int *)da->items[10] == twentytwo);
  assert(*(int *)da->items[0] == twelve);
  assert(*(int *)da->items[4] == sixteen);

  assert(strcmp((char *)da->items[11], twentythree) == 0);
  assert(*(float *)da->items[12] == flt);
  assert(DA_size(da) == 13);
  assert(DA_size(da) == 13);

  DA_pop(da);
  assert(DA_size(da) == 12);
  DA_pop(da);
  assert(DA_size(da) == 11);

  DA *da2 = DA_new();
  int n = 9000 * INITIAL_CAPACITY;
  int arr[n];
  for (int i = 0; i < 9000; i++) {
    arr[i] = i;
    DA_push(da2, &arr[i]);
  }

  assert(da2->size == 9000);
  assert(*(int *)da2->items[0] == 0);

  for (int i = 0; i < 9000; i++) {
    assert(*(int *)da2->items[i] == i);
    assert(DA_get(da2, i) == &arr[i]);
  }

  for (int i = 0; i < 8000; i++)
    DA_pop(da2);

  DA_free(da);
  DA_free(da2);

  printf("OK\n");
  return 0;
}
#endif
