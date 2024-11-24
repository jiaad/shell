#include "shell.h"
#include "readline.h"

#define _POSIX_C_SOURCE 200809L

DA *sys_commands(){
  DA *da;
  da = DA_new();
  DA_push(da, strdup("ls"));
  DA_push(da, strdup("mkdir"));
  DA_push(da, strdup("touch"));
  return da;
}

void run_command(DA *ARGS){
  if(ARGS == NULL){
    exit(1);
  }
  //DA *sys_comm = sys_commands();
  // TODO: use hashmap + read /usr/bin
  execve((char *)ARGS->items[0], (char**)ARGS->items, NULL);
  perror("WHAT IS HAPPENING");
  exit(1);

  // free DA
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
  return ARGS;
  // free args
}


// int main(int argc, char **argv, char **envp){

int main(){
  printf("WELCOME\n");
  DA *sys_comm = sys_commands();

  while(1){
    DA *da;
    da = DA_new();
    char *buf;
    buf = malloc(sizeof(char) * 1024);
    prompt_line();
    readline(buf);
    // CHAGE read_commands to something else
    read_commands(da, buf);//"find -pingiiiiiii -jiad -ping -leak-check; ls -al; ls .");
    // read_commands(da, "find -pingiiiiiii");
    DA *ARGS = parse_tokens(da);
    printf("[len: %d] [capacity: %d]\n",DA_size(da), da->capacity);
    printf("compare %d\n", strcmp(sys_comm->items[0], ((Token*)(da->items[0]))->literal));

    // HERE GOES THE LOGIC OF FORK EXEC
    pid_t pid = Fork();
    if(pid == 0){
      run_command(ARGS);
    }
    wait(NULL);
    for(int j = 0; j < DA_size(da); j++){
      Token_print(da->items[j]);
      Token_free(da->items[j]);
    }
    DA_free(da);
    free(buf);
    printf("OK\n");
  }
  return 0;

}
