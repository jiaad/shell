#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


char *command_concat2(char *s1, char *s2){
  char *res;
  int i = 0;
  int j = 0;
  int s1_len = strlen(s1);
  int s2_len = strlen(s2);
  res = malloc(sizeof(char) * (s1_len + s2_len + 1));
  if(res == NULL){
    fprintf(stderr, "RES could not be allocated, cannot concatenate\n");
    exit(EXIT_FAILURE);
  }

  while(i < s1_len){
    res[i] = s1[i];
    i++;
  }
  while(j < s2_len){
    res[i++] = s2[j];
    j++;
  }
  res[s2_len + s1_len] = '\0';
  return res;
}

int my_cd(int len, char *args[]){
  printf("argc - %d\n", len);
  if(len == 1){
    printf("TODO: ADD GOTO ROOT\n");
    return 1;
  }
  char buf[500];
  if (len == 2){
    getcwd(buf, 499);
    char *comm = command_concat2(buf, (char*)args[1]);
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



