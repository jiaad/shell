#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include "signal.h"

handler_t *Signal(int sig, handler_t *handler){
  struct sigaction action, oldaction;
  action.sa_handler = handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = SA_RESTART;

  if(sigaction(sig, &action, &oldaction) == -1){
    printf("Error while using signal\n");
    exit(0);
  }
  return (oldaction.sa_handler);
}
