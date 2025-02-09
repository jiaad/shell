#include "commands.h"
#include <unistd.h>
// #define __TESTING__

DA *extract_commands_from_dir(char *path) {
  DA *commands;
  DIR *dirp;
  struct dirent *dir;

  commands = DA_new();
  dirp = opendir(path);

  if (dirp == NULL) {
    fprintf(stderr, "Couldn't read %s", path);
    exit(EXIT_FAILURE);
  }
  while ((dir = readdir(dirp))) {
    DA_push(commands, strdup(dir->d_name));
  }
  closedir(dirp);
  return commands;
}

void Commands_free(DA *commands) {
  int i;
  i = 0;

  while (i < DA_size(commands)) {
    free(commands->items[i]);
    i++;
  }
  DA_free(commands);
}

int does_exist_in_commands(char *command, DA *commands) {
  int i;

  i = 0;

  while (i < DA_size(commands)) {
    // printf("testing %s=%s == %d\n", command, (char*)commands->items[i],
    // strcmp(command, (char *)commands->items[i]));
    if (strcmp(command, (char *)commands->items[i]) == 0) {
      return 1;
    }
    i++;
  }
  return 0;
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

void exec_command_and_free(DA *ARGS) {
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
    res = command_concat("/usr/bin/", (char *)ARGS->items[0]);
    free(ARGS->items[0]);
    ARGS->items[0] = res;
  }
  // child_pid = getpid();
  execve((char *)ARGS->items[0], (char **)ARGS->items, NULL);
  perror("SHELL");
  DA_free(ARGS);
  if (res != NULL)
    free(res);
  Commands_free(sys_comm);
  exit(1);
}

#ifdef __TESTING__
int main() {
  DA *bin_commands;
  bin_commands = extract_commands_from_dir("/usr/bin");
  for (int i = 0; i < DA_size(bin_commands); i++) {
    printf("%d=%s\n", i, (char *)bin_commands->items[i]);
  }
  if (does_exist_in_commands("ls", bin_commands)) {
    char *res = command_concat("/usr/bin/", "ls");
    printf("from ls to %s\n", res);
  }
}
#endif
