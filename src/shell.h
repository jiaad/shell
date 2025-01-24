#ifndef __SHELL__
#define __SHELL__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>
// ------------------------------------
#include "../lib/src/da.h"
#include "token.h"
#include "readline.h"
#include "lexer.h"
#include "../lib/src/error.h"
#include "../lib/src/fork.h"
#include "../src/commands.h"
#include "../lib/src/signal.h"

#define _XOPEN_SOURCE  500
#define _POSIX_C_SOURCE 200809L
#define _POSIX_C_SOURCE 200809L
int my_cd(int len, char *args[]);
#define _SVID_SOURCE
#endif
