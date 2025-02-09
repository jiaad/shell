#include "piping.h"
#include "commands.h"

void close_prev_pipes_and_read(int curr, int *fildess[]) {
  int i;
  int read_end;
  int write_end;
  i = 0;

  for (i = 0; i < curr - 1; i++) {
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

void create_pipe(int *fildes[], int size) {
  int i;

  for (i = 0; i < size; i++) {
    int *fd = malloc(sizeof(int) * 2);
    fildes[i] = fd;
  }

  for (i = 0; i < size; i++) {
    pipe(fildes[i]);
  }
}

void close_pipe(int *fildes[], int size) {
  int i;
  for (i = 0; i < size; i++) {
    close(fildes[i][READ_END]);
    close(fildes[i][WRITE_END]);
  }
}

void free_pipe(int *fildes[], int size) {
  int i;
  for (i = 0; i < size; i++) {
    free(fildes[i]);
  }
}

void piping(DA *commands, int pipe_size) { // type of commands
  int *fildess[pipe_size];
  create_pipe(fildess, pipe_size);
  int commands_size;
  commands_size = DA_size(commands);
  for (int i = 0; i < commands_size; i++) {

    int pid = fork();
    if (pid == 0) {
      int *fildes = fildess[i];
      if (i == 0) {
        close(fildes[READ_END]);
        dup2(fildes[WRITE_END], STDOUT_FILENO);
        close(fildes[WRITE_END]);
      } else if (i == pipe_size) {
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
      DA *COMMAND = (DA *)commands->items[i];
      exec_command_and_free(COMMAND);
    }
  }

  close_pipe(fildess, pipe_size);
  free_pipe(fildess, pipe_size);

  while (wait(NULL) > 0)
    ;
}
