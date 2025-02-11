#include "shell.h"
#include "commands.h"
#include "parser.h"

#define _POSIX_C_SOURCE 200809L

int savesigns = 1;
volatile sig_atomic_t shell_pid = 0;
volatile sig_atomic_t should_force_exit = 0;
volatile sig_atomic_t child_pid = 0;
jmp_buf sigint_buf;

void sig_handler(int sig) {
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

// void run_command(DA *ARGS) {
//   char *res = NULL;
//   if (ARGS == NULL) {
//     exit(1);
//   }
//   DA *sys_comm = get_sys_commands();
//   int does_exists = does_exist_in_commands((char *)ARGS->items[0], sys_comm);
//   if (res == NULL && does_exists > -1) {
//     res = (char *)((command_t *)sys_comm->items[does_exists])->path;
//     free(ARGS->items[0]);
//     ARGS->items[0] = res;
//   }
//
//   child_pid = getpid();
//   execve((char *)ARGS->items[0], (char **)ARGS->items, NULL);
//   perror("SHELL");
//   DA_free(ARGS);
//   if (res != NULL)
//     free(res);
//   Commands_free(sys_comm); // TODO: free comnmand_t
//
//   exit(1);
// }

void single_command_exec(DA *ARGS, DA *STMTS, DA *tokens) {
  if (DA_size(ARGS) == 0) {
    printf("-----------\n");
    return;
  }

  // EXIT COMMAND
  if (is_exit_command((char *)ARGS->items[0])) {
    Token_free_all(tokens);
    Parser_free(STMTS);
    exit(EXIT_SUCCESS);
  }

  if (is_cd((char *)ARGS->items[0])) {
    my_cd(DA_size(ARGS), (char **)ARGS->items);
    return;
  }

  pid_t pid = Fork();
  if (pid == 0) {
    child_pid = getpid();
    exec_command_and_free(ARGS); // RUN COMMAND
    while (waitpid(-1, NULL, 0) > 0) {
      printf("[%d]: killed\n", 0);
    }
    // Token_free_all(tokens);
    //DA_free(ARGS);
  } else {
    // Token_free_all(tokens);
    //DA_free(ARGS);
  }
}

void command_exec() {
  DA *tokens;
  char buf[1024];
  int buf_len;

  prompt_line();
  buf_len = readline(buf);
  if (buf_len == 0)
    return;

  tokens = DA_new();
  tokenize(tokens, buf);
  DA *STMTS = parser(tokens);

  for (int i = 0; i < DA_size(STMTS); i++) {
    statement_t *stmt;
    stmt = ((statement_t *)STMTS->items[i]);
    if (stmt->type == PIPE_STATEMENT) {
      piping(stmt->commands, DA_size(stmt->commands) - 1);
    } else if (stmt->type == COMMAND_STATEMENT) {
      single_command_exec((DA *)stmt->commands->items[0], STMTS, tokens);
    }
  }
  // FREE EVERYTHING
  Parser_free(STMTS);
  Token_free_all(tokens);
}

int main() {
  printf("WELCOME %d\n", getpid());
  shell_pid = getpid();
  int status;
  // Signal(SIGINT, sig_handler);
  Signal(SIGQUIT, sig_handler);
  Signal(SIGTERM, sig_handler);

  if (!sigsetjmp(sigint_buf, 1)) {
    //Signal(SIGINT, sig_handler);
    printf("STARTING\n");
  } else {
    printf("new prompt init\n");
  }

  while (1) {
    command_exec();
    while (waitpid(-1, &status, 0) > 0) {
      //   printf("terminated from parent : WIFEXITED(%d) WEXITSTATUS(%d)\n",
      //         WIFEXITED(status), WEXITSTATUS(status));
    }
  }

  return 0;
}
