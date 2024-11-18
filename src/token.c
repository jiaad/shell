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
  token = malloc(sizeof(Token*));
  return token;
}

void token_print(Token *token) { 
  printf("{literal: %s}\n", token->literal); 
}
void Token_free(Token *token) { 
  free(token->literal);
  free(token);
}

