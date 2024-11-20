#include "error.h"

void unix_error(char *message){
  fprintf(stderr, "%s:%s\n", message, strerror(errno));
}

void posix_error(char *message, int code){
  fprintf(stderr, "%s:%s\n", message, strerror(code));
}
