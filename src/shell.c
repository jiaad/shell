#include "shell.h"

#define _POSIX_C_SOURCE 200809L

int savesigns = 1;
volatile sig_atomic_t shell_pid = 0;
volatile sig_atomic_t should_force_exit = 0;
volatile sig_atomic_t child_pid = 0;
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
    siglongjmp(sigint_buf, 1);
    return;
  }
  if (sig == SIGINT && child_pid != 0) {
    if (kill(child_pid, SIGINT) < 0) {
      perror("ERROR SENDING SIGINT TO CHLD");
    }
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

int is_cd(char *str) {
  if (strcmp(str, "cd") == 0) {
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
  if (res == NULL &&
      does_exist_in_commands((char *)ARGS->items[0], sys_comm) == 1) {
    printf("herhehrehrh");
    res = command_concat("/usr/bin/", (char *)ARGS->items[0]);
    free(ARGS->items[0]);
    ARGS->items[0] = res;
  }
  printf(":::::%s:::%s\n", (char *)ARGS->items[0], res);
  child_pid = getpid();
  execve((char *)ARGS->items[0], (char **)ARGS->items, NULL);
  perror("SHELL");
  DA_free(ARGS);
  if (res != NULL)
    free(res);
  Commands_free(sys_comm);
  exit(1);
}

// DA *parse_tokens(DA *tokens) {
//   int i, len;
//   DA *ARGS;
//   ARGS = DA_new();
//   len = DA_size(tokens);
//   i = 0;
//   while (i < len) {
//     if (((Token *)(tokens->items[i]))->literal) {
//       DA_push(ARGS, ((Token *)(tokens->items[i]))->literal);
//     }
//     i++;
//   }
//   ARGS->items[i] = NULL;
//   // while (i < ARGS->capacity) {
//   //   ARGS->items[i] = NULL; // OPTIMIZE THE LOOP
//   //   i++;
//   // }
//   return ARGS; // FREE ARGS
// }

// int main(int argc, char **argv, char **envp){
void command_exec() {
  DA *tokens;
  char buf[1024];
  int buf_len;

  prompt_line();
  buf_len = readline(buf);
  if (buf_len == 0)
    return;

  tokens = DA_new();
  // CHAGE read_commands to something else
  read_commands(tokens, buf);
  DA *ARGS = parse_tokens(tokens);

  // EMPTY
  printf("----------- DA_SIZE(%d):%d\n", DA_size(ARGS), buf[0]);
  if (DA_size(ARGS) == 0) {
    printf("-----------\n");
    return;
  }

  // EXIT COMMAND
  if (is_exit_command((char *)ARGS->items[0])) {
    Token_free_all(tokens);
    DA_free(ARGS);
    exit(EXIT_SUCCESS);
  }

  if (is_cd((char *)ARGS->items[0])) {
    my_cd(DA_size(ARGS), (char **)ARGS->items);
    Token_free_all(tokens);
    DA_free(ARGS);
    return;
  }

  pid_t pid = Fork();
  if (pid == 0) {
    child_pid = getpid();
    printf("i am child pid: %d\n", child_pid);
    run_command(ARGS); // RUN COMMAND
    printf("doesnot come here\n");
    while (waitpid(-1, NULL, 0) < 0) {
      printf("[%d]: killed\n", 0);
    }
    Token_free_all(tokens);
    DA_free(ARGS);
  } else {
    Token_free_all(tokens);
    DA_free(ARGS);
  }
}

int main() {
  printf("WELCOME %d\n", getpid());
  shell_pid = getpid();
  int status;
  // Signal(SIGINT, sig_handler);
  Signal(SIGQUIT, sig_handler);
  Signal(SIGTERM, sig_handler);

  if (!sigsetjmp(sigint_buf, 1)) {
    //  sigaction(SIGINT, &sig, NULL);
    Signal(SIGINT, sig_handler);

    printf("STARTING\n");
  } else {
    printf("new prompt init\n");
  }

  while (1) {
    // printf("starting\n");
    command_exec();
    // while (waitpid(-1, NULL, 0) > 0) {
    //   printf("terminated from parent \n");
    // }
    while (waitpid(-1, &status, 0) > 0) {
      printf("terminated from parent : WIFEXITED(%d) WEXITSTATUS(%d)\n",
             WIFEXITED(status), WEXITSTATUS(status));
    }
  }

  return 0;
}
