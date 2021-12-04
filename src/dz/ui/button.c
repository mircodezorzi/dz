#include <dz/ui/button.h>

#include <stdlib.h>
#include <string.h>

button_t *
button_new(rect_t box, const char *text, void (*fn)(void *))
{
  button_t *b = malloc(sizeof(button_t));
  b->box      = box;
  b->len      = strlen(text);
  b->text     = malloc(sizeof(char) * b->len);
  memcpy(b->text, text, sizeof(char) * b->len);
  b->fn = fn;
  return b;
}

void
button_free(button_t *b)
{
  free(b->text);
  free(b);
}

void
button_draw(button_t *b, screen_t *s)
{
  screen_draw_rect(s, b->box);
  screen_draw_text(s, SHRINK(b->box), b->text, b->len, color_white);
}

void
button_check(button_t *b, point_t at)
{
  if (at.x > b->box.x                   /**/
      && at.x < b->box.x + b->box.w + 2 /**/
      && at.y > b->box.y                /**/
      && at.y < b->box.y + b->box.h + 2) {
    b->fn(b);
  }
}
