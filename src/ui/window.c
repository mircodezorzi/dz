#include <dz/ui/window.h>

#include <stdlib.h>
#include <string.h>

window_t *
window_new(rect_t box, const char *title)
{
  window_t *w = malloc(sizeof(window_t));
  w->len      = strlen(title);
  w->title    = malloc(w->len * sizeof(char));
  memcpy(w->title, title, w->len);
  w->box = box;
  return w;
}

void
window_free(window_t *w)
{
  free(w->title);
  free(w);
}

void
window_draw(window_t *w, screen_t *s)
{
  rect_t b = (rect_t){
      .x = w->box.x + 1,
      .y = w->box.y + 1,
      .h = 1,
      .w = w->box.w - 2,
  };

  screen_draw_rect(s, w->box);
  screen_draw_text(s, b, w->title, w->len, color_white);
  screen_draw_line(s, P(w->box, 0, 2), P(w->box, w->box.w + 1, 2), w->box.tileset, w->box.color, true);
}
