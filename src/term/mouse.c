#include <dz/term/mouse.h>

#include <dz/common.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

bool
is_urxvt()
{
#define BUFSIZE 256
  char  path[BUFSIZE];
  char *envvar = "TERM";

  if (!getenv(envvar)) {
    return false;
  }

  if (snprintf(path, BUFSIZE, "%s", getenv(envvar)) >= BUFSIZE) {
    return strstr(path, "rxvt-unicode");
  }
#undef BUFSIZE

  return false;
}

void
set_mouse_mode(mouse_mode_e mode)
{
  const wchar_t *ext = is_urxvt() ? L_("1015") : L_("1006");

  switch (mode) {
  case MOUSE_MODE_OFF:
    PRINT(L_("\033[?1000;1002;1003;"));
    PRINT(ext);
    PRINT(L_("l"));
    break;
  case MOUSE_MODE_BASIC:
    PRINT(L_("\033[?1000;1002;1003;"));
    PRINT(L_("\033[?1000;"));
    PRINT(ext);
    PRINT(L_("h"));
    break;
  case MOUSE_MODE_DRAG:
    PRINT(L_("\033[?1000;1003l"));
    PRINT(L_("\033[?1002;"));
    PRINT(ext);
    PRINT(L_("h"));
    break;
  case MOUSE_MODE_MOVE:
    PRINT(L_("\033[?1000;1002l"));
    PRINT(L_("\033[?1003;"));
    PRINT(ext);
    PRINT(L_("h"));
    break;
  }
}

mouse_event_t
parse_mouse(char *data)
{
  point_t at;
  int     button;
  char    c;

  sscanf(data, "%d;%d;%d%c", &button, &at.x, &at.y, &c);

  return (mouse_event_t){
      .at     = at,
      .type   = button == 35 ? MOUSE_MOVE : c == 'M',
      .button = button,
  };
}
