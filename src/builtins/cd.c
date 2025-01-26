#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "../commands.h"



int my_cd(int len, char *args[]){
  printf("argc - %d\n", len);
  if(len == 1){
    printf("TODO: ADD GOTO ROOT\n");
    return 1;
  }
  char buf[500];
  if (len == 2){
    if(getcwd(buf, 499) == NULL){
      printf("GETCWD ERROR\n");
      return 0;
    }
    char *comm = command_concat(buf, (char*)args[1]);
    printf("TODO: ADD GOTO ROOT: %s\n", comm);
    int res = chdir((char *)args[1]);
    if(res == 0){
      printf("success: \n");
    } else {
      printf("error %d-%s : res: %d\n", errno,  strerror(errno), res);
    }
    free(comm);
  }
  return 0;
}



