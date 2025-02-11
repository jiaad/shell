#ifndef __PIPING__
#define __PIPING__
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../lib/src/da.h"
#include "commands.h"
#include "parser.h"

#define READ_END 0
#define WRITE_END 1

void piping(DA *commands, DA *STMT, DA *tokens, int pipe_size);
#endif
