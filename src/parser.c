#include "parser.h"
/*
 * 1 - PIPE
 * 2 - REDIRECTION
 * 3 - COMMA SEPARATED
 * 4 - AND OPERATOR
 */
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

/*
 *
 *
 *
 *
 */

// pipe statement ?
// parse pipe
// parse string

/**
 *stmts = [
 *  {
 *    type: STATEMENT_TYPE,
 *    commands: DA
 *  },
 *  {
 *    type: STATEMENT_TYPE,
 *    commands: DA
 *  }
 * ]
 */

statement_t *statement_new(DA *commands) {
  statement_t *stmt;
  stmt = malloc(sizeof(statement_t));
  if (stmt == NULL) {
    fprintf(stderr, "Error during malloc on statement_new\n");
    exit(EXIT_FAILURE);
  }
  stmt->type = COMMAND_STATEMENT;
  if (DA_size(commands) >= 2) {
    stmt->type = PIPE_STATEMENT;
  }
  stmt->commands = commands;

  return stmt;
}

store_t *store_new(DA *tokens) {
  store_t *store;
  store = malloc(sizeof(store_t));
  if (store == NULL) {
    printf("MALLOC_ERROR\n");
    exit(EXIT_FAILURE);
  }
  store->idx = 0;
  store->state = tokens->items[0];
  store->tokens = tokens;
  return store;
}

int match(store_t *store, char *str) {
  int res = strcmp((char *)store->state->literal, str) == 0;
  if (res) { // cmp str{
    store->idx++;
    store->state = (Token *)store->tokens->items[store->idx];
    return 1;
  }
  return 0;
}

int matchType(store_t *store, enum tokens tok) {
  if (store->idx == DA_size(store->tokens)) {
    return 0;
  }

  // for (int j = 0; j < DA_size(store->tokens); j++) {
  //   Token_print(store->tokens->items[j]);
  // }
  int res = store->state->type == tok;
  if (res) { // cmp str{
    store->idx++;
    store->state = (Token *)store->tokens
                       ->items[store->idx]; // store->tokens->items[store->idx];
    return 1;
  }
  return 0;
}

Token *previous(store_t *store) {
  //
  return store->tokens->items[store->idx - 1];
}

/* PARSER
 *
 */
DA *parser(DA *tokens) {
  store_t *store;
  DA *stmts;
  int len;
  stmts = DA_new();
  store = store_new(tokens);

  len = DA_size(tokens);
  // fix this issue
  while (store->idx < len) {
    // EXPR
    DA *da;
    da = E(store);
    statement_t *stmt = statement_new(da);
    DA_push(stmts, stmt);
    // for (int j = 0; j < DA_size(da); j++) {
    //   printf("-> %s\n", (char *)da->items[j]);
    //   // Token_free(da->items[j]);
    // }
    // DA_free(da);
  };

  // if (strcmp("$", (char *)store->state->literal) == 0) {
  //   printf("finished\n");
  // }
  free(store);
  return stmts;
}

DA *E(store_t *store) {
  DA *commands = DA_new();
  DA *prim = command_extract(store);
  if (prim != NULL)
    DA_push(commands, prim);
  while (matchType(store, PIPE)) {
    DA *comm = command_extract(store);
    DA_push(commands, comm);
  }
  return commands;
}

DA *command_extract(store_t *store) {
  DA *com = DA_new();

  while (matchType(store, STRING)) {
    Token *token = previous(store);
    DA_push(com, strdup(token->literal));
  }

  return com;
}
char *primary(store_t *store) {
  /**/
  if (store->state->type == STRING) {
    char *str = strdup(store->state->literal);
    if (matchType(store, STRING))
      return str;
  }
  printf("THIS IS BAD\n");
  exit(EXIT_FAILURE);
}

// #define __TESTING_PARSER__

#ifdef __TESTING_PARSER__
int main(void) {
  DA *tokens;
  tokens = DA_new();
  printf("[len: %d] [capacity: %d]\n", DA_size(tokens), tokens->capacity);
  // read_commands(
  //     tokens,
  //     "find -pingiiiiiii -jiad -ping -leak-check; ls -al; ls .   | ls -al");
  // read_commands(da, "find -pingiiiiiii");

  //read_commands(tokens, "ls -al -jiad | grep jiad | ping google.com");
  read_commands(tokens, "/bin/ls");
  printf("[len: %d] [capacity: %d]\n", DA_size(tokens), tokens->capacity);
  for (int j = 0; j < DA_size(tokens); j++) {
    Token_print(tokens->items[j]);
    // Token_free(da->items[j]);
  }
  // DA_free(da);
  printf("OK\n");
  DA *stmts = parser(tokens);

  for (int i = 0; i < DA_size(stmts); i++) {
    statement_t *U_S = ((statement_t *)stmts->items[i]);

    printf("{commands: %d} \n", i);
    for (int k = 0; k < DA_size((DA *)U_S->commands); k++) {

      printf("--> command: %d\n", k);

      DA *M_S = U_S->commands->items[k];
      for (int l = 0; l < DA_size(M_S); l++) {
        printf("----> {k: %d} - {%s}\n", DA_size(M_S), (char *)M_S->items[l]);
      }
    }
  }

  return 0;
}
#endif
