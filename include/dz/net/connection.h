#ifndef __NET_CONNECTION_H_
#define __NET_CONNECTION_H_

#include <pthread.h>
#include <sys/epoll.h>

#include <dz/common.h>

typedef enum {
  SOCKET_CREATION_FAILED,
  BIND_FAILED,
  LISTEN_FAILED,
  EPOLL_CREATE_FAILED,
  EPOLL_WAIT_FAILED,
} error_e;

typedef struct {
  void (*on_accept)(void);
  void (*on_read)(char *buffer, int len);
} server_options_t;

typedef struct {
  char *addr;
  int   port;

  bool run;

  int     sock_fd;
  int     epoll_fd;
  error_e err;

  pthread_t thread_id;

  void (*on_accept)(void);
  void (*on_read)(char *buffer, int len);
} server_t;

server_t *server_new(const char *addr, int port, server_options_t opt);
void      server_free(server_t *s);

void server_nonblocking_loop(server_t *s);
void server_loop(void *v);
void server_init(server_t *s);
void server_wait(server_t *s);

void setnonblocking(int fd);
void registerfd(int epoll_fd, int fd);

static void server_process(server_t *s, struct epoll_event *events, int number);

#endif /* __NET_CONNECTION_H_ */
