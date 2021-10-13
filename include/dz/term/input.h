#ifndef __DZ_TERM_INPUT_H__
#define __DZ_TERM_INPUT_H__

#include <dz/term/keyboard.h>
#include <dz/term/mouse.h>

typedef struct {
  kb_event_t    kb;
  mouse_event_t mouse;
  enum {
    EVENT_TYPE_KEYBORAD,
    EVENT_TYPE_MOUSE,
    EVENT_TYPE_NONE,
  } type;
} input_event_t;

/**
  \brief Parse keyboard and mouse input.
  \see `set_mouse_mode`.
  \return Struct representing mouse event. `type` member must not be EVENT_TYPE_NONE.
  */
input_event_t parse_input(void);

#endif /* __DZ_TERM_INPUT_H__ */
