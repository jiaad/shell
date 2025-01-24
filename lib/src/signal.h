#ifndef __SIGNAL__
#define __SIGNAL__
typedef void handler_t(int);
handler_t *Signal(int sig, handler_t  *handler);

#endif
