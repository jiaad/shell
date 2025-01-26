#include <sys/types.h>
#include "shell.h"
#include "readline.h"

void prompt_line(){
  printf("[%d] -> ", getpid());
  fflush(stdout);
}

int readline(char buf[]){
  // read until the end
  size_t bytes_read;
  int newline_started = 0;
  char c;
  int bytes_counter = 0;
  while ((bytes_read = read(STDIN_FILENO, &c, 1)) > 0) {
    if (bytes_read <= 0) {
      perror("READ");
      // return EXIT_FAILURE;
      exit(EXIT_FAILURE);
    }
    int bs = c is BACKSLASH;
    if (bs) {
      newline_started = 1;
      continue;
    }
    if (newline_started && !bs && c != NEWLINE) {
      newline_started = 0;
    }
    if (c is NEWLINE && newline_started) {
      (void)!write(STDOUT_FILENO, "here ", 5);
      newline_started = 0;
      (void)!write(STDOUT_FILENO, "\n> ", 3);
      continue;
    }
    if (newline_started == 0 && !bs && c is NEWLINE) {
        break;
    }
  //  write(STDOUT_FILENO, &c, 1);
    buf[bytes_counter] = c;
    if (c != NEWLINE)
      bytes_counter++;
  }
  if(bytes_read == 0){
    // EOF ?
    printf("sooooo\n");
    exit(EXIT_SUCCESS);

  }
  printf("bytes read: %ld\n", bytes_read);
  buf[bytes_counter] = '\0';
  //write(STDOUT_FILENO, "\n", 1);
  return bytes_counter;
}



#ifdef __TESTING_READLINE__
int main(void) {
  char buf[1024];
  prompt_line();
  readline(buf);
  return 0;
}
#endif
