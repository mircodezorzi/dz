#include <dz/term/input.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <wchar.h>

input_event_t
parse_input()
{
#define BUFSIZE    256
#define CHUNK_SIZE 16
  input_event_t ev;
  ev.type = EVENT_TYPE_NONE;

  char data[BUFSIZE] = {0};
  int  len           = 0;
  int  r             = 0;

  do {
    r = read(STDIN_FILENO, data + len, CHUNK_SIZE);
    len += r;
  } while (r && len < BUFSIZE);

  if (len > 0) {
    if (len > 1 && data[len - 1] == 'm' || data[len - 1] == 'M') {
      mouse_event_t m = parse_mouse(data);
      if (m.button != BUTTON_NONE) {
        ev.e.mouse = m;
        ev.type    = EVENT_TYPE_MOUSE;
      }
    } else {
      ev.e.kb = parse_key(data);
      ev.type = EVENT_TYPE_KEYBOARD;
    }
  }
#undef BUFSIZE
#undef CHUNK_SIZE
  return ev;
}
