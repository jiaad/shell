#include "fork.h"

int Fork(){
  pid_t pid = fork();
  if(pid == -1){
    perror("FORK");
    exit(EXIT_FAILURE);
  }
  return pid;
}
