#ifndef __DZ_TERM_INPUT_H__
#define __DZ_TERM_INPUT_H__

#include <dz/term/keyboard.h>
#include <dz/term/mouse.h>

typedef struct {
  union {
    kb_event_t    kb;
    mouse_event_t mouse;
  } e;

  enum {
    EVENT_TYPE_NONE,
    EVENT_TYPE_KEYBOARD,
    EVENT_TYPE_MOUSE,
  } type;
} input_event_t;

/**
  \brief Parse keyboard and mouse input.
  \see `set_mouse_mode`.
  \return Struct representing mouse event. `type` member must not be EVENT_TYPE_NONE.
  */
input_event_t parse_input(void);

#endif /* __DZ_TERM_INPUT_H__ */
