#include "parser.h"

DA *parse_tokens(DA *tokens) {
  int i, len;
  DA *ARGS;
  ARGS = DA_new();
  len = DA_size(tokens);
  i = 0;
  while (i < len) {
    if (((Token *)(tokens->items[i]))->literal) {
      DA_push(ARGS, ((Token *)(tokens->items[i]))->literal);
    }
    i++;
  }
  ARGS->items[i] = NULL;
  // while (i < ARGS->capacity) {
  //   ARGS->items[i] = NULL; // OPTIMIZE THE LOOP
  //   i++;
  // }
  return ARGS; // FREE ARGS
}
