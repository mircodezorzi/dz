#include <dz/ui/button.h>

#include <stdlib.h>
#include <string.h>

button_t *
button_new(rect_t box, char *text, void (*fn)(void *))
{
  button_t *b = malloc(sizeof(button_t));

  b->box  = box;
  b->fn   = fn;
  b->len  = strlen(text);
  b->text = malloc(sizeof(char) * b->len);
  memcpy(b->text, text, sizeof(char) * b->len);
  return b;
}

void
button_free(button_t *b)
{
  free(b->text);
  free(b);
}

void
button_draw(screen_t *s, button_t *b)
{
  screen_draw_rect(s, b->box);
  screen_draw_text(s, SHRINK(b->box), b->text, b->len, color_white);
}

void
button_check(button_t *b, point_t at)
{
  /* clang-format off */
  if (at.x > b->box.x
      && at.x < b->box.x + b->box.w
      && at.y > b->box.y
      && at.y < b->box.y + b->box.h) {
    b->fn(b);
  }
  /* clang-format on */
}
