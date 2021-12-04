#ifndef __DZ_UI_WINDOW_H__
#define __DZ_UI_WINDOW_H__

#include <dz/screen.h>

/**
  \todo Implement window dragging.
  */
typedef struct {
  char    *title;
  unsigned len;
  rect_t   box;
} window_t;

window_t *window_new(rect_t box, const char *title);
void      window_free(window_t *w);
void      window_draw(window_t *w, screen_t *s);

#endif /* __DZ_UI_WINDOW_H__ */
