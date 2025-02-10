#ifndef __COMMANDS__
#define __COMMANDS__
#include "../lib/src/da.h"
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef struct Command {
  char *command;
  char *path;

} command_t;

DA *extract_commands_from_dir(DA *commands, char *path);
void Commands_free(DA *commands);
int does_exist_in_commands(char *command, DA *commands);
char *command_concat(char *s1, char *s2);
void exec_command_and_free(DA *ARGS);
DA *get_sys_commands();
#endif
