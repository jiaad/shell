#ifndef __ERROR__
#define __ERROR__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void unix_error(char *message);
void posix_error(char *message, int code);

#endif
