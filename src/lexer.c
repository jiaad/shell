#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
  TILDA
} tokens_t;

typedef struct Token {
  int start;
  int end;
  char *literal;
  tokens_t type;
} Token;

typedef struct Scanner {
  int pos;
  int nextPos;
  char ch;
  char *input;
} Scanner;
void read_commands(Token **tokens, char *command);
void read_command(char *str);
void token_create(Token token);
char *split_str(char *src, int start, int end);
int cs_strlen(char *str);

void token_create(Token token) {
  token.start = 0;
  token.end = 0;
  token.type = STRING;
  token.literal = strdup("jiad");
}
void token_print(Token *token) { printf("{literal: %s}\n", token->literal); }

int cs_strlen(char *str){
  int i = 0;
  while (str[i] != '\0') i++;
  return i;
}
int shouldTokenizeAsStr(char c) {
  if (isalpha(c)){
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

void free_token(Token *token){
  free(token->literal);
}

char *split_str(char *src, int start, int end){
  int len = (end - start) + 2;
  int i = 0;
  char *res = malloc(sizeof(char) * len);
  while(start <= end){
   res[i++] = src[start++] ;
  }
  res[i] = '\0';
  return res;
}
// send array of tokens
//
void read_commands(Token **tokens, char *command) {
  // while next char is not /0
  Scanner scanner;
  scanner.ch = '\0'; // command[0];
  scanner.pos = -1;
  scanner.nextPos = 0;
  scanner.input = command;

  char c;
  while ((c = peekChar(&scanner)) > -1) {
    switch (c) {
    case '-': {
      Token token;
      token.start = scanner.pos;
      token.end = scanner.pos;
      token.type = DASH;
      token.literal = strdup("-");
      token_print(&token);
      //printf(" %c", '-');
      fflush(stdout);
      free_token(&token);
      break;
    }
    case ' ':
      break;
    case ';':{
      Token token;
      token.start = scanner.pos;
      token.end = scanner.pos;
      token.type = SEMICOLON;
      token.literal = strdup(";");
      token_print(&token);
      free_token(&token);
      break;
    }
    case '.':{
      Token token;
      token.start = scanner.pos;
      token.end = scanner.pos;
      token.type = DOT;
      token.literal = strdup(".");
      token_print(&token);
      free_token(&token);
      break;
    }
    case '~':{
      Token token;
      token.start = scanner.pos;
      token.end = scanner.pos;
      token.type = TILDA;
      token.literal = strdup("~");
      token_print(&token);
      free_token(&token);
      break;
    }
    case '/':{
      Token token;
      token.start = scanner.pos;
      token.end = scanner.pos;
      token.type = SLASH;
      token.literal = strdup(".");
      token_print(&token);
      free_token(&token);
      break;
    }
    default:
      if (shouldTokenizeAsStr(c)) {
        int start = scanner.pos;
        while (isPeekChar(&scanner) == 1) {
          char cc = peekChar(&scanner);
        }
        int end = scanner.pos;
        Token token;
        token.start = start;
        token.end = end;
        token.type = STRING;

        token.literal = split_str(scanner.input, start, end);
        token_print(&token);
        free_token(&token);
      }
    }
  }
}

Token **lexer(){
  Token **tokens;
  tokens = calloc(10, sizeof(Token*));
  return tokens;
}
void free_lexer(Token **lexer){
    free(lexer);
}
int main(int argc, char **argv) {
  Token **tokens = lexer();
  read_commands(tokens, "find -pingiiiiiii -jiad -ping -leak-check; ls -al; ls .");
  printf("OK\n");
  free_lexer(tokens);
}
