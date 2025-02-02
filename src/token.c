#include "token.h"
// ls -a
// touch file.c
// mkaidr jiad123
// find . -type d
// grep "jiad"

/**
 * [
 *   Token, type, literal
 *   Token,
 *   Token.
 * ]
 */

Token *Token_new(){
  Token *token;
  token = malloc(sizeof(Token));
  if(token == NULL){
    fprintf(stderr, "Token allocation failed");
    exit(EXIT_FAILURE);
  }
  return token;
}

void Token_print(Token *token) { 
  printf("{literal: %s, type: %d}\n", token->literal, token->type); 
}

void Token_free(Token *token) { 
  free(token->literal);
  free(token);
}

void Token_free_all(DA *tokens){
  int i;
  int len;

  len = DA_size(tokens);
  i = 0;
  for(i = 0; i < len; i++){
    Token_free(tokens->items[i]);
  }
  DA_free(tokens);
}
