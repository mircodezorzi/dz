#ifndef __DZ_TERM_KEYBOARD_H__
#define __DZ_TERM_KEYBOARD_H__

enum {
  KEY_NONE   = 0x0000,
  KEY_ESCAPE = 0x001b,
  KEY_ENTER  = 0x000a,
  KEY_UP     = 0x0105,
  KEY_DOWN   = 0x0106,
  KEY_LEFT   = 0x0107,
  KEY_RIGHT  = 0x0108,
};

/**
  \brief Keyboard event.
  */
typedef struct {
  int key;
  enum {
    MOD_NONE  = 0,
    MOD_CTRL  = 1,
    MOD_SHIFT = 2,
  } modifier;
} kb_event_t;

kb_event_t parse_key(char *data);

#endif /* __DZ_TERM_KEYBOARD_H__ */
