#include <dz/term/input.h>

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <wchar.h>

char
select_char()
{
  char buff;
  return read(STDIN_FILENO, &buff, 1) > 0 ? buff : 0;
}

input_event_t
parse_input()
{
#define BUFSIZE 256
  char data[BUFSIZE];
  int  index = 0;
  int  len   = 0;
  while ((data[index++] = select_char()))
    ;
  len = strlen(data);
  if (data[len - 1] == 'm' || data[len - 1] == 'M') {
    return (input_event_t){.mouse = parse_mouse(data)};
  } else {
    return (input_event_t){.kb = parse_key(data)};
  }
#undef BUFSIZE
}
