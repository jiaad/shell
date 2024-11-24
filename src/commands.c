#include "commands.h"
// #define __TESTING__

DA *extract_commands_from_dir(char *path){
  DA *commands;
  DIR *dirp;
  struct dirent *dir;

  commands = DA_new();
  dirp = opendir(path);

  if(dirp == NULL){
    fprintf(stderr, "Couldn't read %s", path);
    exit(EXIT_FAILURE);
  }
  while((dir = readdir(dirp))){
    DA_push(commands, strdup(dir->d_name));
  }
  closedir(dirp);
  return commands;
}

int does_exist_in_commands(char *command, DA *commands){
  int i;

  i = 0;

  while(i < DA_size(commands)){
    //printf("testing %s=%s == %d\n", command, (char*)commands->items[i], strcmp(command, (char *)commands->items[i]));
    if(strcmp(command, (char *)commands->items[i]) == 0){
      return 1;
    }
    i++;
  }
  return 0;
}

char *command_concat(char *s1, char *s2){
  char *res;
  int i = 0;
  int j = 0;
  int s1_len = strlen(s1);
  int s2_len = strlen(s2);
  res = malloc(sizeof(char) * (s1_len + s2_len + 1));
  if(res == NULL){
    fprintf(stderr, "RES could not be allocated, cannot concatenate\n");
    exit(EXIT_FAILURE);
  }

  while(i < s1_len){
    res[i] = s1[i];
    i++;
  }
  while(j < s2_len){
    res[i++] = s2[j];
    j++;
  }
  res[s2_len + s1_len] = '\0';
  return res;
}

#ifdef __TESTING__
int main(){
  DA *bin_commands;
  bin_commands = extract_commands_from_dir("/usr/bin");
  for(int i = 0; i < DA_size(bin_commands); i++){
    printf("%d=%s\n",i, (char*)bin_commands->items[i]);
  }
  if(does_exist_in_commands("ls", bin_commands)){
    char *res = command_concat("/usr/bin/", "ls");
    printf("from ls to %s\n",res);

  }
}
#endif
