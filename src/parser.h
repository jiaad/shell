#ifndef __PARSER__
#define __PARSER__

#include "../lib/src/da.h"
#include "lexer.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct store_t {
  int idx;
  Token *state; // DA
  DA *tokens;
} store_t;

typedef enum STATEMENT_TYPE {
  PIPE_STATEMENT,
  COMMAND_STATEMENT
} STATEMENT_TYPE;

typedef struct statement_t {
  STATEMENT_TYPE type;
  DA *commands;
} statement_t;

DA *parse_tokens(DA *tokens);
DA *E(store_t *store);
int match(store_t *store, char *str);
int matchType(store_t *store, enum tokens tok);
char *primary(store_t *store);
DA *command_extract(store_t *store);
DA *parser(DA *tokens);
void Parser_free(DA *stmts);
#endif
