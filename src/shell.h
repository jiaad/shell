#ifndef __SHELL__
#define __SHELL__
#include <ctype.h>
#include <fcntl.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
// ------------------------------------
#include "../lib/src/da.h"
#include "../lib/src/error.h"
#include "../lib/src/fork.h"
#include "../lib/src/signal.h"
#include "../src/commands.h"
#include "lexer.h"
#include "parser.h"
#include "readline.h"
#include "token.h"

#define _XOPEN_SOURCE 500
#define _POSIX_C_SOURCE 200809L
#define _POSIX_C_SOURCE 200809L
int my_cd(int len, char *args[]);
#define _SVID_SOURCE
#endif
