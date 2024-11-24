#include "shell.h"
#include "readline.h"

#define _POSIX_C_SOURCE 200809L

void run_command(DA *ARGS){
  char *res = NULL;
  if(ARGS == NULL){
    exit(1);
  }
  DA *sys_comm = extract_commands_from_dir("/usr/bin");
  if(does_exist_in_commands((char*)ARGS->items[0], sys_comm) == 1){
    res = command_concat("/usr/bin/", (char *)ARGS->items[0]);
    free(ARGS->items[0]);
    ARGS->items[0] = res;
  }

  execve((char *)ARGS->items[0], (char**)ARGS->items, NULL);
  perror("SHELL");
  DA_free(ARGS);
  if(res != NULL)
    free(res);
  Commands_free(sys_comm);
  exit(1);
}


DA *parse_tokens(DA *tokens){
  int i, len;
  DA *ARGS;
  ARGS = DA_new();
  len = DA_size(tokens);
  i = 0;
  while(i < len){
    if(((Token*)(tokens->items[i]))->literal){
      DA_push(ARGS,((Token*)(tokens->items[i]))->literal);
    }
    i++;
  }
  while(i < ARGS->capacity){
    ARGS->items[i] = NULL; 
    i++;
  }
  return ARGS;
  // free args
}


// int main(int argc, char **argv, char **envp){

int main(){
  printf("WELCOME\n");
  while(1){
    DA *tokens;
    tokens = DA_new();
    char *buf;
    buf = malloc(sizeof(char) * 1024);
    prompt_line();
    readline(buf);
    // CHAGE read_commands to something else
    read_commands(tokens, buf);
    DA *ARGS = parse_tokens(tokens);
    pid_t pid = Fork();
    if(pid == 0) run_command(ARGS);
    wait(NULL);
    for(int j = 0; j < DA_size(tokens); j++){
      Token_free(tokens->items[j]);
    }
    DA_free(tokens);
    DA_free(ARGS);
    free(buf);
  }
  return 0;
}
