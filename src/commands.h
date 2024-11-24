#ifndef __COMMANDS__
#define __COMMANDS__
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include "../lib/src/da.h"


DA *extract_commands_from_dir(char *path);
void Commands_free(DA *commands);
int does_exist_in_commands(char *command, DA *commands);
char *command_concat(char *s1, char *s2);
#endif
