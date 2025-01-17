#include "shell.h"
#include "readline.h"
#include <setjmp.h>

#define _POSIX_C_SOURCE 200809L

int savesigns = 1;
volatile sig_atomic_t shell_pid = 0;
volatile sig_atomic_t should_force_exit = 0;
volatile sig_atomic_t child_pid = 0;
volatile sig_atomic_t force_exit_shell = 0;
jmp_buf sigint_buf;

/*
void sig_handler(int sig){
  printf("here inside sig_handler child_pid: %d sig: %d<->%d\n", child_pid, sig,
SIGINT); if(sig == SIGINT){
 //   exit(EXIT_SUCCESS);
  }
  if(sig == SIGQUIT){\
    exit(EXIT_SUCCESS);
  }
  if(force_exit_shell == 1){
    printf("EXIT\n");
    exit(EXIT_SUCCESS);
  } else if(child_pid > 0){
    printf("EXIT\n");
    kill(sig, child_pid);
  }
}
*/

void sig_handler(int sig) {
  // printf("here inside sig_handler child_pid: %d sig: %d<->%d "
  //        "is_force_exit_shell: %d\n",
  //        child_pid, sig, SIGINT, force_exit_shell);
  if (sig == SIGINT && child_pid == 0) {
    siglongjmp(sigint_buf, -1);
    return;
  }
}

int builtin_commands(char *command) {
  char *commands[] = {"cd", 0};
  char **ptr = commands;
  while (*ptr != 0) {
    if (strcmp(*ptr, command) == 0) {
      return 1;
    }
    ptr++;
  }
  return 0;
}


int is_cd(char *str){
  if(strcmp(str, "cd") == 0){
    return 1;
  }
  return 0;
}

int is_exit_command(char *command) {
  if (strcmp("exit", command) == 0) {
    return 1;
  }
  return 0;
}

void run_command(DA *ARGS) {
  char *res = NULL;
  if (ARGS == NULL) {
    exit(1);
  }
  // if(builtin_commands((char *)ARGS->items[0])){
  //   printf("herhehrehrh");
  //   res = command_concat("./bin/", (char *)ARGS->items[0]);
  //   free(ARGS->items[0]);
  //   ARGS->items[0] = res;
  // }
  DA *sys_comm = extract_commands_from_dir("/usr/bin/");
  if (res == NULL && does_exist_in_commands((char *)ARGS->items[0], sys_comm) == 1) {
    printf("herhehrehrh");
    res = command_concat("/usr/bin/", (char *)ARGS->items[0]);
    free(ARGS->items[0]);
    ARGS->items[0] = res;
  }
  printf(":::::%s:::%s\n", (char *)ARGS->items[0],res);
  child_pid = getpid();
  execve((char *)ARGS->items[0], (char **)ARGS->items, NULL);
  perror("SHELL");
  DA_free(ARGS);
  if (res != NULL)
    free(res);
  Commands_free(sys_comm);
  exit(1);
}

DA *parse_tokens(DA *tokens) {
  int i, len;
  DA *ARGS;
  ARGS = DA_new();
  len = DA_size(tokens);
  i = 0;
  while (i < len) {
    if (((Token *)(tokens->items[i]))->literal) {
      DA_push(ARGS, ((Token *)(tokens->items[i]))->literal);
    }
    i++;
  }
  while (i < ARGS->capacity) {
    ARGS->items[i] = NULL;
    i++;
  }
  return ARGS; // FREE ARGS
}

// int main(int argc, char **argv, char **envp){
void command_exec() {
  //child_pid = getpid();
  DA *tokens;
  tokens = DA_new();
  char *buf;
  buf = malloc(sizeof(char) * 1024);
  prompt_line();
  readline(buf);
  // CHAGE read_commands to something else
  read_commands(tokens, buf);
  DA *ARGS = parse_tokens(tokens);

  // EXIT COMMAND
  if (is_exit_command((char *)ARGS->items[0])) {
    force_exit_shell = 1;
    printf("exiting uwu: %d - is force: %d\n", child_pid, force_exit_shell);
    //  kill(shell_pid, SIGTERM);
    exit(EXIT_SUCCESS);
  }

  if(is_cd((char *)ARGS->items[0])){
    my_cd(DA_size(ARGS), (char**)ARGS->items);
    return;
  }

  pid_t pid = Fork();
  if (pid == 0) {
    child_pid = getpid();
    printf("i am child pid: %d\n", child_pid);
    run_command(ARGS); // RUN COMMAND
    while (waitpid(-1, NULL, 0) < 0) {
      printf("[%d]: killed\n", 0);
    }
    // wait(NULL);
    // child_pid = 0;
    for (int j = 0; j < DA_size(tokens); j++) {
      Token_free(tokens->items[j]);
    }
    DA_free(tokens);
    DA_free(ARGS);
    free(buf);
  }
}

int main() {
  printf("WELCOME %d\n", getpid());
  shell_pid = getpid();
  struct sigaction sig = {0};
  sig.sa_handler = sig_handler;
  sigemptyset(&sig.sa_mask);
  sig.sa_flags = SA_SIGINFO | SA_RESTART;
  //sig.sa_flags = SA_SIGINFO;

  // memset(&sig, 0, sizeof sig);
  sigaction(SIGINT, &sig, NULL);
  sigaction(SIGQUIT, &sig, NULL);
  sigaction(SIGTERM, &sig, NULL);

  while (1) {
    // printf("starting\n");
    if(sigsetjmp(sigint_buf,1) == 0){
      command_exec();
      while (waitpid(-1, NULL, 0) > 0) {
        printf("terminated from parent \n");
      }
    }
    printf("new prompt init\n");
    // switch (sigsetjmp(sigint_buf, savesigns)) {
    // case 0: {
    //   command_exec();
    //   while (waitpid(-1, NULL, 0) < 0) {
    //     printf("terminated from parent \n");
    //   }
    //   break;
    // }
    // case 1:
    //   printf("new prompt init\n");
    //   break;
    // }
  }

  return 0;
}
