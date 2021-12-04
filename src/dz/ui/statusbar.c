#include <dz/ui/statusbar.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* forward declaration for ANSI support */
int vsnprintf(char *s, size_t n, const char *format, va_list arg);

statusbar_t *
statusbar_new(const char *text)
{
  statusbar_t *sb = malloc(sizeof(statusbar_t));
  sb->len         = strlen(text);
  sb->text        = malloc(sb->len * sizeof(char));
  memcpy(sb->text, text, sb->len);
  return sb;
}

void
statusbar_free(statusbar_t *sb)
{
  free(sb->text);
  free(sb);
}

void
statusbar_update(statusbar_t *sb, const char *text)
{
  sb->len  = strlen(text);
  sb->text = realloc(sb->text, sb->len * sizeof(char));
  memcpy(sb->text, text, sb->len);
}

void
statusbar_fupdate(statusbar_t *sb, const char *fmt, ...)
{
#define BUFSIZE 256
  char buffer[BUFSIZE];
  bzero(buffer, BUFSIZE);
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, BUFSIZE, fmt, args);
  va_end(args);
  statusbar_update(sb, buffer);
#undef BUFSIZE
}

void
statusbar_draw(statusbar_t *sb, screen_t *s)
{
  int    w;
  int    h;
  rect_t box;

  termsize(&w, &h);

  box = (rect_t){
      .x     = 0,
      .y     = w - 1,
      .w     = h,
      .h     = 1,
      .color = color_red,
  };

  screen_draw_text(s, box, sb->text, sb->len, color_white);
}
