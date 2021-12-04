#include <dz/term/mouse.h>

#include <dz/utf8.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

/* forward declaration for ANSI support */
int snprintf(char *buffer, size_t bufsz, const char *format, ...);

/**
  \brief Returns true whether current terminal is urxvt.
  \see `set_mouse_mode`.
  */
static bool
is_urxvt(void)
{
#define BUFSIZE 256
  char  path[BUFSIZE];
  char *envvar = "TERM";

  if (!getenv(envvar)) {
    return false;
  }

  if (snprintf(path, BUFSIZE, "%s", getenv(envvar)) >= BUFSIZE) {
    return !!strstr(path, "rxvt-unicode");
  }

  return false;
#undef BUFSIZE
}

void
set_mouse_mode(mouse_mode_e mode)
{
  char *ext = is_urxvt() ? "1015" : "1006";

  switch (mode) {
  case MOUSE_MODE_OFF:
    u8_printf("\033[?1000;1002;1003;");
    u8_printf(ext);
    u8_printf("l");
    break;
  case MOUSE_MODE_BASIC:
    u8_printf("\033[?1000;1002;1003;");
    u8_printf("\033[?1000;");
    u8_printf(ext);
    u8_printf("h");
    break;
  case MOUSE_MODE_DRAG:
    u8_printf("\033[?1000;1003l");
    u8_printf("\033[?1002;");
    u8_printf(ext);
    u8_printf("h");
    break;
  case MOUSE_MODE_MOVE:
    u8_printf("\033[?1000;1002l");
    u8_printf("\033[?1003;");
    u8_printf(ext);
    u8_printf("h");
    break;
  }
}

mouse_event_t
parse_mouse(char *data)
{
  point_t  at;
  unsigned button;
  char     c;
  int      r;

  r = sscanf(data + 3, "%d;%d;%d%c", &button, &at.x, &at.y, &c);

  if (r != 4) {
    return (mouse_event_t){.button = BUTTON_NONE};
  }

  at.x -= 1;
  at.y -= 1;

  return (mouse_event_t){
      .at     = at,
      .type   = button == 35 ? MOUSE_MOVE : c == 'M',
      .button = button,
  };
}
