#include "shell.h"
#include "readline.h"
// int main(int argc, char **argv, char **envp){
int main(){
  printf("WELCOME\n");

  while(1){
  DA *da;
  da = DA_new();
    char buf[1024];
    prompt_line();
    readline(buf);
    read_commands(da, buf);//"find -pingiiiiiii -jiad -ping -leak-check; ls -al; ls .");
  // read_commands(da, "find -pingiiiiiii");
  printf("[len: %d] [capacity: %d]\n",DA_size(da), da->capacity);
  for(int j = 0; j < DA_size(da); j++){
    Token_print(da->items[j]);
    Token_free(da->items[j]);
  }
  DA_free(da);
  printf("OK\n");
  }
  return 0;

}
