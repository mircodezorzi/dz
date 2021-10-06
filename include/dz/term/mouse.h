#ifndef __DZ_TERM_MOUSE_H__
#define __DZ_TERM_MOUSE_H__

#include <dz/common.h>
#include <dz/point.h>

typedef enum {
  MOUSE_MODE_OFF,
  /* Report mouse state each click. */
  MOUSE_MODE_BASIC,
  /* Report mouse state each time it is dragged. */
  MOUSE_MODE_DRAG,
  /* Report mouse state each time it moves. */
  MOUSE_MODE_MOVE,
} mouse_mode_e;

/**
  \brief Mouse event.
  */
typedef struct {
  point_t at;

  enum {
    MOUSE_PRESS,
    MOUSE_RELEASE,
    MOUSE_MOVE = 35,
  } type;

  enum {
    BUTTON_LEFT = 0,
    BUTTON_MIDDLE,
    BUTTON_RIGHT,
  } button;
} mouse_event_t;

static bool is_urxvt(void);

/**
  \brief Set appropriate mouse reporting mode.
  \reference https://github.com/a-n-t-h-o-n-y/Escape/blob/master/src/terminal.cpp
  */
void set_mouse_mode(mouse_mode_e mode);

mouse_event_t parse_mouse(char *data);

#endif /* __DZ_TERM_MOUSE_H__ */
