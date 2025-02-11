#include "commands.h"
#include <unistd.h>
// #define __TESTING__

/**
 * 
 *
 * COMMANDS
 *
 *
 */
command_t *Command_new() {
  command_t *com;
  com = malloc(sizeof(command_t));
  if(com == NULL){
    fprintf(stderr, "ERROR DURING COMMAND_NEW malloc \n");
    exit(EXIT_FAILURE);
  }
  return com;
}

void Command_free(command_t *command) {
  //
  free(command->command);
  free(command->path);
  free(command);
}

DA *extract_commands_from_dir(DA *commands, char *path) {
  DIR *dirp;
  struct dirent *dir;
  command_t *comm;

  dirp = opendir(path);

  if (dirp == NULL) {
    fprintf(stderr, "Couldn't read %s", path);
    exit(EXIT_FAILURE);
  }

  while ((dir = readdir(dirp))) {
    if(dir == NULL) break;
    comm = Command_new();
    comm->path = command_concat(path, dir->d_name);
    comm->command = strdup(dir->d_name);
    DA_push(commands, comm);
  }
  closedir(dirp);
  return commands;
}

/*
 * GET COMMANDS FROM PATHS
 * [{
 *  command: ls,
 *  fullPath; /bin/ls
 * }]
 */
DA *get_sys_commands() {
  DA *commands;
  commands = DA_new();

  char *paths[2] = {"/usr/bin/", "/bin/"};
  extract_commands_from_dir(commands, paths[0]);
  extract_commands_from_dir(commands, paths[1]);

  return commands;
}

void free_sys_commands(DA *sys_commands){
  int i;
  int len;

  len = DA_size(sys_commands);
  for(i = 0; i < len; i++){
    Command_free(sys_commands->items[i]);
  }
  DA_free(sys_commands);
}


/*
 * command : str
 * commands : [{}, {}] -> struct command_t
 *
 * return -1 if not exist else index
 * */
int does_exist_in_commands(char *command, DA *commands) {
  int i;
  int size;

  i = 0;
  size = DA_size(commands);

  while (i < size) {
    if (strcmp(command, (char *)((command_t *)commands->items[i])->command) ==
        0) {
      return i;
    }
    i++;
  }
  return -1;
}

char *command_concat(char *s1, char *s2) {
  char *res;
  int i = 0;
  int j = 0;
  int s1_len = strlen(s1);
  int s2_len = strlen(s2);
  res = malloc(sizeof(char) * (s1_len + s2_len + 1));
  if (res == NULL) {
    fprintf(stderr, "RES could not be allocated, cannot concatenate\n");
    exit(EXIT_FAILURE);
  }

  while (i < s1_len) {
    res[i] = s1[i];
    i++;
  }
  while (j < s2_len) {
    res[i++] = s2[j];
    j++;
  }
  res[s2_len + s1_len] = '\0';
  return res;
}

void exec_command_and_free(DA *command, DA *STMTS, DA *tokens){
  char *res = NULL;
  if (command== NULL) {
    exit(1);
  }

  DA *sys_comm = get_sys_commands();
  int does_exists = does_exist_in_commands((char *)command->items[0], sys_comm);
  if (res == NULL && does_exists > -1) {
    res = (char *)((command_t *)sys_comm->items[does_exists])->path;
    free(command->items[0]);
    command->items[0] = res;
  }
  DA_push(command, NULL);
  // child_pid = getpid();
  execve((char *)command->items[0], (char **)command->items, NULL);
  perror("SHELL ERROR:");
  Parser_free(STMTS);
  Token_free_all(tokens);
  free_sys_commands(sys_comm);
  exit(1);
}

#ifdef __TESTING__
int main() {
  DA *bin_commands;
  bin_commands = get_sys_commands();
  for (int i = 0; i < DA_size(bin_commands); i++) {
    printf("%d=%s\n", i, (char *)bin_commands->items[i]);
  }
  if (does_exist_in_commands("ls", bin_commands)) {
    char *res = command_concat("/usr/bin/", "ls");
    printf("from ls to %s\n", res);
  }
}
#endif
