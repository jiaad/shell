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
  printf("{literal: %s}\n", token->literal); 
}
void Token_free(Token *token) { 
  free(token->literal);
  free(token);
}

