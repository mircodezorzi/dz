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
  } type;
} input_event_t;

/**
  \brief Non blocking read.
  */
static char select_char(void);

input_event_t parse_input(void);

#endif /* __DZ_TERM_INPUT_H__ */
