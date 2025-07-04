#ifndef __TOKEN__
#define __TOKEN__
// #include "shell.h"
#include "../lib/src/da.h"
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

typedef enum tokens {
  // basics
  STRING, // COMMAND ?
  IDENTIFIER,
  DASH,
  SEMICOLON,
  POINT,
  DOT,
  SLASH,
  TILDA,
  PIPE
} tokens_t;

typedef struct Token {
  int start;
  int end;
  char *literal;
  tokens_t type;
} Token;

Token *Token_new();
void Token_print(Token *token);
void Token_free(Token *token);
void Token_free_all(DA *tokens);
#endif
