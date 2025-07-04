#include "lexer.h"
#include "shell.h"
#include "token.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define _POSIX_C_SOURCE 200809L
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

int cs_strlen(char *str) {
  int i = 0;
  while (str[i] != '\0')
    i++;
  return i;
}

int shouldTokenizeAsStr(char c) {
  int should_skip = c == ' ' || c == '\t' || c == '\b';
  if (should_skip == 1) {
    return 0;
  }
  if (c == ';' || c == '|')
    return 0;
  int is_ascii = c >= 0x00 && c < 0x7f;
  if (is_ascii == 1) {
    return 1;
  }
  return 0;
}

void read_command(char *str) {
  size_t len = cs_strlen(str);
  size_t i;
  i = 0;
  while (i < len) {
    printf("%c", str[i++]);
  }
  printf("\n");
}

char peekChar(Scanner *scanner) {
  if (scanner->nextPos == EOF) {
    return EOF;
  }

  char ret = scanner->input[scanner->nextPos];
  scanner->pos = scanner->nextPos;
  scanner->nextPos++;
  if (scanner->input[scanner->nextPos] == '\0') {
    scanner->nextPos = EOF;
  }
  return ret;
}

char getCurrChar(Scanner *scanner, char *str) { return str[scanner->pos]; }
char getNextChar(Scanner *scanner, char *str) { return str[scanner->nextPos]; }

int isPeekChar(Scanner *scanner) {
  if (scanner->nextPos == EOF) {
    return EOF;
  }
  if (scanner->input[scanner->nextPos] == '\0') {
    return EOF;
  }
  if (shouldTokenizeAsStr(scanner->input[scanner->nextPos])) {
    return 1;
  }
  return EOF;
}

void free_token(Token *token) { free(token->literal); }

char *slice_str(char *src, int start, int end) {
  int len = (end - start) + 2;
  int i = 0;
  char *res = malloc(sizeof(char) * len);
  if (res == NULL) {
    unix_error("MALLOC FAILED on slice_str: ");
    return NULL;
  }

  while (start <= end) {
    res[i++] = src[start++];
  }
  res[i] = '\0';
  return res;
}
// send array of tokens
//
void tokenize(DA *tokens, char *command) {
  // while next char is not /0
  Scanner scanner;
  scanner.ch = '\0'; // command[0];
  scanner.pos = -1;
  scanner.nextPos = 0;
  scanner.input = command;

  // inside string
  char c;
  while ((c = peekChar(&scanner)) > -1) {
    switch (c) {
    case ' ':
    case '\t':
    case '\n':
    case '\b':
      break;
    case '|': {
      Token *token;
      token = Token_new();
      token->start = scanner.pos;
      token->end = scanner.pos;
      token->type = PIPE;
      token->literal = strdup("|");
      DA_push(tokens, token);
      break;
    }
    case ';': {
      Token *token;
      token = Token_new();
      token->start = scanner.pos;
      token->end = scanner.pos;
      token->type = SEMICOLON;
      token->literal = strdup(";");
      DA_push(tokens, token);
      break;
    }
    default: {
      if (shouldTokenizeAsStr(c)) {
        int start = scanner.pos;
        while (isPeekChar(&scanner) == 1) {
          peekChar(&scanner);
        }
        int end = scanner.pos;
        Token *token;
        token = Token_new();
        token->start = start;
        token->end = end;
        token->type = STRING;
        token->literal = slice_str(scanner.input, start, end);
        DA_push(tokens, token);
      }
    }
    }
  }
}

#define __TESTING_LEXER
// int main(int argc, char **argv) {
#ifdef __TESTING_LEXER__
int main() {
  DA *da;
  da = DA_new();
  printf("[len: %d] [capacity: %d]\n", DA_size(da), da->capacity);
  read_commands(
      da, "find -pingiiiiiii -jiad -ping -leak-check; ls -al; ls .   | ls -al");
  // read_commands(da, "find -pingiiiiiii");
  printf("[len: %d] [capacity: %d]\n", DA_size(da), da->capacity);
  for (int j = 0; j < DA_size(da); j++) {
    Token_print(da->items[j]);
    Token_free(da->items[j]);
  }
  DA_free(da);
  printf("OK\n");
}
#endif
