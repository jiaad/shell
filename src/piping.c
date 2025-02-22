#include "piping.h"
#include "commands.h"

void close_prev_pipes_and_read(int curr, int fildess[][2])
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

/**
 * create all the pipes needed for the pipes 
 */
void create_pipes(int fildes[][2], int size){
  int i;
  int rax;

  if (fildes == NULL)
    return;

  for (i = 0; i < size; i++) {
    rax = pipe(fildes[i]);
    if (rax < 0) {
      fprintf(stderr, "pipe creation failed\n");
      for(int j = 0; j < i; j++){
        close(fildes[j][0]);
        close(fildes[j][1]);
      }
      exit(EXIT_FAILURE);
    }
  }
}

/*
 * cles the pipes after the end of the programme
 */
void close_pipes(int fildes[][2], int size) {
  int i;
  for (i = 0; i < size; i++) {
    close(fildes[i][READ_END]);
    close(fildes[i][WRITE_END]);
  }
}

void piping(DA *commands, DA *STMT, DA *tokens, int pipe_size) { // type of commands
  int fildess[pipe_size][2];
  int status;
  int childs[DA_size(commands)];
  for (int k = 0; k < pipe_size; k++){
    fildess[k][0] = 0;
    fildess[k][1] = 0;
  }
  int commands_size;
  create_pipes(fildess, pipe_size);
  commands_size = DA_size(commands);
  for (int i = 0; i < commands_size; i++) {
    int pid = fork();
    childs[i] = pid;
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

  close_pipes(fildess, pipe_size);
  for(int c = 0; c < commands_size; c++){
    waitpid(childs[c], &status, 0);
  }
}
