#include <dz/net/connection.h>

#include <dz/utf8.h>

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <wchar.h>

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE      1024

void
setnonblocking(int fd)
{
  int opt = fcntl(fd, F_GETFL);
  fcntl(fd, F_SETFL, opt | O_NONBLOCK);
}

void
registerfd(int epoll_fd, int fd)
{
  struct epoll_event event;
  event.data.fd = fd;
  event.events  = EPOLLIN;
  event.events |= EPOLLET;
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
  setnonblocking(fd);
}

server_t *
server_new(const char *addr, int port, server_options_t opt)
{
  server_t *s   = malloc(sizeof(server_t));
  int       len = strlen(addr);
  s->addr       = malloc(strlen(addr) * sizeof(char));
  memcpy(s->addr, addr, len);
  s->port = port;
  s->run  = false;

  s->on_accept = opt.on_accept;
  s->on_read   = opt.on_read;
  return s;
}

void
server_free(server_t *s)
{
  free(s->addr);
  free(s);
}

void
server_nonblocking_loop(server_t *s)
{
  pthread_create(&s->thread_id, NULL, server_loop, (void *)(s));
}

void
server_loop(void *v)
{
  server_t *s = (server_t *)v;
  server_init(s);
}

void
server_init(server_t *s)
{
  int                ret = -1;
  struct epoll_event events[MAX_EVENT_NUMBER];

  struct sockaddr_in address;
  bzero(&address, sizeof(address));
  address.sin_family = AF_INET;
  inet_pton(AF_INET, s->addr, &address.sin_addr);
  address.sin_port = htons(s->port);
  if ((s->sock_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    s->err = SOCKET_CREATION_FAILED;
    return;
  }

  ret = bind(s->sock_fd, (struct sockaddr *)&address, sizeof(address));
  if (ret == -1) {
    s->err = BIND_FAILED;
    return;
  }

  ret = listen(s->sock_fd, 5);
  if (ret == -1) {
    s->err = LISTEN_FAILED;
    return;
  }

  s->epoll_fd = epoll_create(5);
  if (s->epoll_fd == -1) {
    s->err = EPOLL_CREATE_FAILED;
    return;
  }

  registerfd(s->epoll_fd, s->sock_fd);

  for (;;) {
    ret = epoll_wait(s->epoll_fd, events, MAX_EVENT_NUMBER, -1);
    if (ret < 0) {
      s->err = EPOLL_WAIT_FAILED;
      break;
    }

    server_process(s, events, ret);
  }

  close(s->sock_fd);
}

void
server_wait(server_t *s)
{
  int *ptr;
  s->run = false;
  pthread_join(s->thread_id, (void **)&ptr);
}

void
server_process(server_t *s, struct epoll_event *events, int number)
{
  char buf[BUFFER_SIZE];
  int  i;
  for (i = 0; i < number; i++) {
    int sockfd = events[i].data.fd;
    if (sockfd == s->sock_fd) {
      struct sockaddr_in client_address;
      socklen_t          client_addrlength = sizeof(client_address);
      int connfd = accept(s->sock_fd, (struct sockaddr *)&client_address, &client_addrlength);
      registerfd(s->epoll_fd, connfd);
    } else if (events[i].events & EPOLLIN) {
      wprintf(L"et mode: event trigger once!\n");
      for (;;) {
        memset(buf, 0, BUFFER_SIZE);
        ssize_t ret = recv(sockfd, buf, BUFFER_SIZE - 1, 0);
        if (ret < 0) {
          if (errno == EAGAIN || errno == EWOULDBLOCK) {
            wprintf(L"read later!\n");
            break;
          }
          close(sockfd);
          break;
        } else if (ret == 0) {
          close(sockfd);
        } else {
          wprintf(L"get %d bytes of content: %s\n", ret, buf);
        }
      }
    } else {
      wprintf(L"something unexpected happened!\n");
    }
  }
}
