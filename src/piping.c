#include "piping.h"
#include "commands.h"

void close_prev_pipes_and_read(int curr, int *fildess[])
{
  int i;
  int read_end;
  int write_end;

  if (fildess == NULL)
  {
    // TODO: handle
    fprintf(stderr, "fildes is uninitialized\n");
    exit(EXIT_FAILURE);
  }
  for (i = 0; i < curr - 1; i++)
  {
    if (fildess != NULL)
    {
      int *fd = fildess[i];
      if (fd != NULL){
        read_end = fildess[i][READ_END];
        write_end = fildess[i][WRITE_END];
        close(read_end);
        close(write_end);
      }
    }
  }
  if (fildess != NULL)
  {
    read_end = fildess[i][READ_END];
    if(!read_end) exit(1);
    write_end = fildess[i][WRITE_END];
    dup2(read_end, READ_END);
    close(read_end);
    close(write_end);
  }
}

void create_pipe(int *fildes[], int size){
  int i;

  if (fildes == NULL)
    return;

  for (i = 0; i < size; i++) {
    int *fd = malloc(sizeof(int) * 2);
      if (fd == NULL) {
          fprintf(stderr, "Memory allocation failed\n");
          // Free previously allocated memory
          for (int j = 0; j < i; j++) {
              close(fildes[j][0]);
              close(fildes[j][1]);
              free(fildes[j]);
          }
          exit(EXIT_FAILURE);
    }
    fildes[i] = fd;
    int rax = pipe(fd);
    if (rax < 0) {
      fprintf(stderr, "pipe creation failed\n");
      free(fd);
      for(int j = 0; j < i; j++){
        close(fildes[j][0]);
        close(fildes[j][1]);
        free(fildes[j]);
      }
      exit(EXIT_FAILURE);
    }
  }
}

void close_pipe(int *fildes[], int size)
{
  int i;
  for (i = 0; i < size; i++)
  {
    close(fildes[i][READ_END]);
    close(fildes[i][WRITE_END]);
  }
}

void free_pipe(int *fildes[], int size)
{
  int i;
  for (i = 0; i < size; i++)
  {
    if (fildes[i] != NULL)
    {
      free(fildes[i]);
    }
  }
}

void piping(DA *commands, DA *STMT, DA *tokens, int pipe_size)
{ // type of commands
  int *fildess[pipe_size];
  for (int k = 0; k < pipe_size; k++){
    fildess[k] = NULL;
  }
  int commands_size;
  create_pipe(fildess, pipe_size);
  commands_size = DA_size(commands);
  for (int i = 0; i < commands_size; i++)
  {
    int pid = fork();
    if (pid == 0) {
      if (i == 0) {
        int *fildes = fildess[i];
        if(fildes == NULL) exit(0);
        close(fildes[READ_END]);
        dup2(fildes[WRITE_END], STDOUT_FILENO);
        close(fildes[WRITE_END]);
      }
      else if (i == pipe_size) {
        close_prev_pipes_and_read(i, fildess);
      }
      else {
        int read_end, write_end;
        close_prev_pipes_and_read(i, fildess);
        read_end = fildess[i][READ_END];
        write_end = fildess[i][WRITE_END];
        close(read_end);
        dup2(write_end, WRITE_END);
        close(write_end);
      }
      DA *COMMAND = (DA *)commands->items[i];
      exec_command_and_free(COMMAND, STMT, tokens);
    }
  }

  // doesn't free if failed
  close_pipe(fildess, pipe_size);
  free_pipe(fildess, pipe_size);
  while (wait(NULL) > 0)
    ;
}
