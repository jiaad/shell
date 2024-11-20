
#ifndef __LEXER__
#define __LEXER__
void read_commands(DA *tokens, char *command);
void read_command(char *str);
char *slice_str(char *src, int start, int end);
int cs_strlen(char *str);
int shouldTokenizeAsStr(char c);
#endif
