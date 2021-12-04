#ifndef __DZ_LAYOUT_BUTTON_H__
#define __DZ_LAYOUT_BUTTON_H__

#include <dz/screen.h>

typedef struct {
  rect_t        box;
  char         *text;
  unsigned long len;
  /* callback */
  void (*fn)(void *);
} button_t;

button_t *button_new(rect_t box, char *text, void (*fn)(void *));
void      button_free(button_t *b);
void      button_draw(screen_t *s, button_t *b);
void      button_check(button_t *b, point_t at);

#endif /* __DZ_LAYOUT_BUTTON_H__ */
