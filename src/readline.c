#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void prompt_line(){
  printf("[%d] -> ", getpid());
  fflush(stdout);
}

void readline(char buf[]){
  // read until the end

}
