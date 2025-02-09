#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ_END 0
#define WRITE_END 1

void close_prev_pipes_and_read(int curr, int *fildess[]) {
  int i;
  int read_end;
  int write_end;
  i = 0;

  for (i = 0; i < curr - 1; i++) {
    printf("foing\n");
    read_end = fildess[i][READ_END];
    write_end = fildess[i][WRITE_END];
    close(read_end);
    close(write_end);
  }
  read_end = fildess[i][READ_END];
  write_end = fildess[i][WRITE_END];
  dup2(read_end, READ_END);
  close(read_end);
  close(write_end);
}

int main(void) {
  int read_end, write_end;
  int fildes1[2];
  int fildes2[2];
  int fildes3[2];
  int fildes4[2];
  int *fildess[4] = {fildes1, fildes2, fildes3};

  pipe(fildess[0]);
  pipe(fildess[1]);
  pipe(fildess[2]);
  // pipe(fildess[3]);
  // pipe(fildes[1]);
  //   pipe(fildes3);
  printf("before fork\n");

  char *commands[4] = {"/bin/ls", "/usr/bin/grep", "/usr/bin/grep",
                       "/usr/bin/grep"};
  char *commands2[4] = {"-al", "j", "n", "socket"};
  int last_idx = 3;
  char *commands3[3] = {"/bin/ls", "/usr/bin/grep"};
  for (int i = 0; i < 4; i++) {

    int pid = fork();
    if (pid == 0) {
      int *fildes = fildess[i];
      if (i == 0) {
        close(fildes[READ_END]);
        dup2(fildes[WRITE_END], STDOUT_FILENO);
        close(fildes[WRITE_END]);
      } else if (i == last_idx) {
        close_prev_pipes_and_read(i, fildess);
      } else {
        int read_end, write_end;
        close_prev_pipes_and_read(i, fildess);
        read_end = fildess[i][READ_END];
        write_end = fildess[i][WRITE_END];
        close(read_end);
        dup2(write_end, WRITE_END);
        close(write_end);
      }
      char *COM[3];
      COM[0] = strdup(commands[i]);
      COM[1] = strdup(commands2[i]);
      COM[2] = NULL;
      if (execve(COM[0], COM, NULL) == -1) {
        free(COM[0]);
        free(COM[1]);
        perror("SHELL");
        exit(0);
      }
    }
  }

  for (int i = 0; i < 3; i++) {
    int *fildes = fildess[i];
    close(fildes[0]);
    close(fildes[1]);
  }

  while (wait(NULL) > 0) {
    printf("this\n");
  };

  return 0;
}
