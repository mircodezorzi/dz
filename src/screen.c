#include <dz/screen.h>

#include <dz/common.h>
#include <dz/term.h>
#include <dz/utf8.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <wchar.h>

static const pixel_t pixel_t_default = {
    .c          = L' ',
    .foreground = {.r = 255, .g = 255, .b = 255},
    .background = {.r = 0, .g = 0, .b = 0},
};

static const pixel_t pixel_t_null = {
    .c          = L' ',
    .foreground = {.r = -1, .g = -1, .b = -1},
    .background = {.r = -1, .g = -1, .b = -1},
};

static const wchar_t tileset[8][10] = {
    [TILESET_SINGLE] = {
        [TILE_TRC] = L'┌',
        [TILE_TLC] = L'┐',
        [TILE_BRC] = L'└',
        [TILE_BLC] = L'┘',
        [TILE_H]   = L'─',
        [TILE_V]   = L'│',
        [TILE_RT]  = L'├',
        [TILE_LT]  = L'┤',
        [TILE_UT]  = L'┴',
        [TILE_DT]  = L'┬',
    },

    [TILESET_DOUBLE] = {
        [TILE_TRC] = L'╔',
        [TILE_TLC] = L'╗',
        [TILE_BRC] = L'╚',
        [TILE_BLC] = L'╝',
        [TILE_H]   = L'═',
        [TILE_V]   = L'║',
        [TILE_RT]  = L'╠',
        [TILE_LT]  = L'╣',
        [TILE_UT]  = L'╩',
        [TILE_DT]  = L'╦',
    },

    [TILESET_DASHED] = {
        [TILE_TRC] = L'┌',
        [TILE_TLC] = L'┐',
        [TILE_BRC] = L'└',
        [TILE_BLC] = L'┘',
        [TILE_H]   = L'┄',
        [TILE_V]   = L'┊',
        [TILE_RT]  = L'├',
        [TILE_LT]  = L'┤',
        [TILE_UT]  = L'┴',
        [TILE_DT]  = L'┬',
    },

    [TILESET_BLOCK] = {
        [TILE_FB]   = L'█',
        [TILE_THB]  = L'▀',
        [TILE_BHB]  = L'▄',
        [TILE_LHB]  = L'▌',
        [TILE_RHB]  = L'▐',
        [TILE_TLHB] = L'▗',
        [TILE_TRHB] = L'▖',
        [TILE_BLHB] = L'▘',
        [TILE_BRHB] = L'▝',
    },

    [TILESET_FULL_BLOCKS]  = { [TILE_FB] = L'█' },
    [TILESET_FULL_SHADOW1] = { [TILE_FB] = L'░' },
    [TILESET_FULL_SHADOW2] = { [TILE_FB] = L'▒' },
    [TILESET_FULL_SHADOW3] = { [TILE_FB] = L'▓	' }
};

screen_t *
screen_new()
{
  int i;

  screen_t *s = malloc(sizeof(screen_t));

  /* fetch terminal size */
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  s->w       = w.ws_col;
  s->h       = w.ws_row;
  s->changed = true;

  /* setup screen buffer */
  s->buffer = malloc(s->w * s->h * sizeof(pixel_t));
  for (i = 0; i < s->w * s->h; i++) {
    s->buffer[i] = pixel_t_default;
  }

  return s;
}

void
screen_free(screen_t *s)
{
  if (s->buffer) {
    free(s->buffer);
    s->buffer = NULL;
  }
  free(s);
}

void
screen_pixel_print(pixel_t p, pixel_t old)
{
  if (p.foreground.r != old.foreground.r || p.foreground.g != old.foreground.g
      || p.foreground.b != old.foreground.b) {
    u8_printf("\033[38;2;%d;%d;%dm", p.foreground.r, p.foreground.g, p.foreground.b);
  }
  if (p.background.r != old.background.r || p.background.g != old.background.g
      || p.background.b != old.background.b) {
    u8_printf("\033[48;2;%d;%d;%dm", p.background.r, p.background.g, p.background.b);
  }
  u8_printf("%lc", p.c);
}

void
screen_render(screen_t *s)
{
  int i;

  /* used to optimize ansi color escape codes */
  pixel_t old = pixel_t_null;

  for (i = 0; i < s->w * s->h; i++) {
    screen_pixel_print(s->buffer[i], old);
    old = s->buffer[i];
  }
  fflush(0);
  u8_printf("\033[%d;%dH", 1, 1);
}

void
screen_repaint(screen_t *s)
{
  int i;
  for (i = 0; i < s->w * s->h; i++) {
    s->buffer[i] = pixel_t_default;
  }
}

void
screen_update(screen_t *s)
{
  int i, w, h;

  /* fetch terminal size, update if necessary */
  termsize(&w, &h);
  if (s->w != w || s->h != h) {
    s->w       = w;
    s->h       = h;
    s->changed = true;

    s->buffer = realloc(s->buffer, s->w * s->h * sizeof(pixel_t));

    for (i = 0; i < s->w * s->h; i++) {
      s->buffer[i] = pixel_t_default;
    }

    u8_printf(ANSI_CLEAR_SCREEN);
  }
}

wchar_t
screen_merge_chars(wchar_t a, wchar_t b)
{
  switch (a) {
  case L'─':
    switch (b) {
    case L'┌':
    case L'┐': return L'┬';
    case L'└':
    case L'┘': return L'┴';
    case L'│': return L'┼';
    case L'║': return L'╫';
    }
    break;
  case L'│':
    switch (b) {
    case L'─': return L'┼';
    case L'═': return L'╪';
    }
    break;
  case L'┐':
    switch (b) {
    case L'┌':
    case L'─': return L'┬';
    case L'═': return L'╤';
    }
    break;
  case L'┌':
    switch (b) {
    case L'┐':
    case L'─': return L'┬';
    case L'═': return L'╤';
    }
    break;
  case L'═':
    switch (b) {
    case L'┐':
    case L'┌': return L'╤';
    }
    break;
  }
  return b;
}

void
screen_draw_rect(screen_t *s, rect_t r)
{
  int i;

  /* top-right corner */
  s->buffer[r.x + r.y * s->w].c
      = screen_merge_chars(s->buffer[r.x + r.y * s->w].c, tileset[r.tileset][TILE_TRC]);
  s->buffer[r.x + r.y * s->w].foreground = r.color;

  /* top-left corner */
  s->buffer[r.x + r.y * s->w + r.w].c
      = screen_merge_chars(s->buffer[r.x + r.y * s->w + r.w].c, tileset[r.tileset][TILE_TLC]);
  s->buffer[r.x + r.y * s->w + r.w].foreground = r.color;

  /* vertical sides */
  for (i = 1; i <= r.h; i++) {
    s->buffer[r.x + (r.y + i) * s->w].c
        = screen_merge_chars(s->buffer[r.x + (r.y + i) * s->w].c, tileset[r.tileset][TILE_V]);
    s->buffer[r.x + (r.y + i) * s->w].foreground = r.color;

    s->buffer[r.x + (r.y + i) * s->w + r.w].c
        = screen_merge_chars(s->buffer[r.x + (r.y + i) * s->w + r.w].c, tileset[r.tileset][TILE_V]);
    s->buffer[r.x + (r.y + i) * s->w + r.w].foreground = r.color;
  }

  /* horizontal sides */
  for (i = 1; i < r.w; i++) {
    s->buffer[(r.x + i) + r.y * s->w].c
        = screen_merge_chars(s->buffer[(r.x + i) + r.y * s->w].c, tileset[r.tileset][TILE_H]);
    s->buffer[(r.x + i) + r.y * s->w].foreground = r.color;

    s->buffer[(r.x + i) + (r.y + r.h + 1) * s->w].c = screen_merge_chars(
        s->buffer[(r.x + i) + (r.y + r.h + 1) * s->w].c, tileset[r.tileset][TILE_H]);
    s->buffer[(r.x + i) + (r.y + r.h + 1) * s->w].foreground = r.color;
  }

  /* bottom-right corner */
  s->buffer[r.x + (r.y + r.h + 1) * s->w].c
      = screen_merge_chars(s->buffer[r.x + (r.y + r.h + 1) * s->w].c, tileset[r.tileset][TILE_BRC]);
  s->buffer[r.x + (r.y + r.h + 1) * s->w].foreground = r.color;

  /* bottom-left corner */
  s->buffer[r.x + (r.y + r.h + 1) * s->w + r.w].c = screen_merge_chars(
      s->buffer[r.x + (r.y + r.h + 1) * s->w + r.w].c, tileset[r.tileset][TILE_BLC]);
  s->buffer[r.x + (r.y + r.h + 1) * s->w + r.w].foreground = r.color;
}

void
screen_draw_text(screen_t *s, rect_t r, const char *text, unsigned len, color_t color)
{
  int i;

  for (i = 0; i < min(r.w, len); i++) {
    s->buffer[r.x + r.y * s->w + i].c          = (wchar_t)text[i];
    s->buffer[r.x + r.y * s->w + i].background = r.color;
    s->buffer[r.x + r.y * s->w + i].foreground = color;
  }
  for (; i < r.w; i++) {
    s->buffer[r.x + r.y * s->w + i].c          = L' ';
    s->buffer[r.x + r.y * s->w + i].background = r.color;
    s->buffer[r.x + r.y * s->w + i].foreground = color;
  }
}

void
screen_draw_vline(screen_t *s, point_t a, int len, tileset_e ts, color_t color)
{
  int i;
  for (i = 0; i < len; i++) {
    s->buffer[a.x + (a.y + i) * s->w].c
        = screen_merge_chars(s->buffer[a.x + (a.y + i) * s->w].c, tileset[ts][TILE_V]);

    s->buffer[a.x + (a.y + i) * s->w].foreground = color;
  }
}

void
screen_draw_hline(screen_t *s, point_t a, int len, tileset_e ts, color_t color)
{
  int i;
  for (i = 0; i < len; i++) {
    s->buffer[a.x + i + a.y * s->w].c
        = screen_merge_chars(s->buffer[a.x + i + a.y * s->w].c, tileset[ts][TILE_H]);

    s->buffer[a.x + i + a.y * s->w].foreground = color;
  }
}

void
screen_draw_veline(screen_t *s, point_t a, int len, tileset_e ts, color_t color)
{
  int i;
  s->buffer[a.x + a.y * s->w].c
      = screen_merge_chars(s->buffer[a.x + a.y * s->w].c, tileset[ts][TILE_DT]);
  for (i = 1; i < len - 1; i++) {
    s->buffer[a.x + (a.y + i) * s->w].c
        = screen_merge_chars(s->buffer[a.x + (a.y + i) * s->w].c, tileset[ts][TILE_V]);

    s->buffer[a.x + (a.y + i) * s->w].foreground = color;
  }
  s->buffer[a.x + (a.y + len) * s->w].c
      = screen_merge_chars(s->buffer[a.x + (a.y + len) * s->w].c, tileset[ts][TILE_UT]);
}

void
screen_draw_heline(screen_t *s, point_t a, int len, tileset_e ts, color_t color)
{
  int i;
  s->buffer[a.x + a.y * s->w].c
      = screen_merge_chars(s->buffer[a.x + a.y * s->w].c, tileset[ts][TILE_RT]);
  for (i = 1; i < len - 1; i++) {
    s->buffer[a.x + i + a.y * s->w].c
        = screen_merge_chars(s->buffer[a.x + i + a.y * s->w].c, tileset[ts][TILE_H]);

    s->buffer[a.x + i + a.y * s->w].foreground = color;
  }
  s->buffer[a.x + len - 1 + a.y * s->w].c
      = screen_merge_chars(s->buffer[a.x + len - 1 + a.y * s->w].c, tileset[ts][TILE_LT]);
}

void
screen_draw_line(screen_t *s, point_t a, point_t b, tileset_e ts, color_t color, bool endings)
{

  if (a.x == b.x) {
    if (endings) {
      screen_draw_veline(s, a, abs(a.y - b.y), ts, color);
    } else {
      screen_draw_vline(s, a, abs(a.y - b.y), ts, color);
    }
  } else if (a.y == b.y) {
    if (endings) {
      screen_draw_heline(s, a, abs(a.x - b.x), ts, color);
    } else {
      screen_draw_hline(s, a, abs(a.x - b.x), ts, color);
    }
  }
}

void
screen_draw_frect(screen_t *s, frect_t r)
{
  int i;
  int j;

#define DECIMAL(f) (((float)((int)(f))) - f)
  float x1 = DECIMAL(r.x);
  float y1 = DECIMAL(r.y);
  float w1 = DECIMAL(r.w);
  float h1 = DECIMAL(r.h);
#undef DECIMAL

  for (i = 0; (float)(i) < r.w; i++) {
    for (j = 0; (float)(j) < r.h; j++) {
      int index = (int)(r.x) + i + ((int)(r.y) + j) * s->w;

      s->buffer[index].c          = tileset[TILESET_BLOCK][TILE_FB];
      s->buffer[index].foreground = r.color;
    }
  }

  /* handle offset rectangles - yes I know, it's ugly. */

  /* left side */
  if (x1) {
    for (i = 0; (float)(i) < r.h; i++) {
      int index = (int)(r.x) + ((int)(r.y) + i) * s->w;

      s->buffer[index].c = tileset[TILESET_BLOCK][TILE_RHB];
    }
  }

  /* top side */
  if (y1) {
    for (i = 0; (float)(i) < r.w; i++) {
      int index = (int)(r.x) + i + (int)(r.y) * s->w;

      s->buffer[index].c = tileset[TILESET_BLOCK][TILE_BHB];
    }
  }

  /* right side */
  if (x1 && !w1) {
    for (i = 0; (float)(i) < r.h; i++) {
      int index = (int)(r.x + r.w) + ((int)(r.y) + i) * s->w;

      s->buffer[index].c          = tileset[TILESET_BLOCK][TILE_LHB];
      s->buffer[index].foreground = color_white;
    }
  }

  /* bottom side */
  if (y1 && !w1) {
    for (i = 0; (float)(i) < r.w; i++) {
      int index = (int)(r.x) + i + ((int)(r.y) + (int)(r.w)) * s->w;

      s->buffer[index].c          = tileset[TILESET_BLOCK][TILE_THB];
      s->buffer[index].foreground = color_white;
    }
  }

  /* top left corner */
  if (x1 && y1) {
    int index = (int)(r.x) + (int)(r.y) * s->w;

    s->buffer[index].c = tileset[TILESET_BLOCK][TILE_BLHB];
  }

  /* top right corner */
  if (x1 && !w1 && y1) {
    int index = (int)(r.x) + (int)(r.w) + (int)(r.y) * s->w;

    s->buffer[index].c = tileset[TILESET_BLOCK][TILE_BRHB];
  }

  /* bottom right corner */
  if (y1 && !h1 && x1) {
    int index = (int)(r.x) + (int)(r.w) + ((int)(r.y) + (int)(r.h)) * s->w;

    s->buffer[index].c          = tileset[TILESET_BLOCK][TILE_TLHB];
    s->buffer[index].foreground = color_white;
  }

  /* bottom left corner */
  if (y1 && !h1 && x1 && !w1) {
    int index = (int)(r.x) + ((int)(r.y) + (int)(r.h)) * s->w;

    s->buffer[index].c          = tileset[TILESET_BLOCK][TILE_TRHB];
    s->buffer[index].foreground = color_white;
  }
}

static void
screen_set_braille_point(screen_t *s, rect_t bb, float x, float y)
{
  static const int pixel_map[4][2] = {
      {0x01, 0x08},
      {0x02, 0x10},
      {0x04, 0x20},
      {0x40, 0x80},
  };

  static const int braille_char_offset = 0x2800;
  static const int braille_char_end    = braille_char_offset + 255;

  x += bb.x * 2 + bb.w + 2;     /* s->w; */
  y += bb.y * 4 + bb.h * 2 + 4; /* s->h * 2; */

  if (x > (bb.x + bb.w) * 2 - 1 || y > (bb.y + bb.h) * 4 - 1 || x <= bb.x * 2 || y <= bb.y * 4) {
    return;
  }

  if (s->buffer[(int)(x / 2) + (int)(y / 4) * s->w].c < braille_char_offset
      || s->buffer[(int)(x / 2) + (int)(y / 4) * s->w].c > braille_char_end) {
    s->buffer[(int)(x / 2) + (int)(y / 4) * s->w].c = braille_char_offset;
  }

  s->buffer[(int)(x / 2) + (int)(y / 4) * s->w].c |= pixel_map[(int)fmod(y, 4.f)][(int)fmod(x, 2)];
  s->buffer[(int)(x / 2) + (int)(y / 4) * s->w].foreground = color_white;
}

void
screen_draw_braille_line(screen_t *s, rect_t bb, point_t a, point_t b)
{
  float xdiff = max(a.x, b.x) - min(a.x, b.x);
  float ydiff = max(a.y, b.y) - min(a.y, b.y);
  float r     = max(xdiff, ydiff);
  float xdir  = a.x <= b.x ? 1.f : -1.f;
  float ydir  = a.y <= b.y ? 1.f : -1.f;

  int i;

  for (i = 0; (float)i < r; i++) {
    float x = (float)a.x;
    float y = (float)a.y;

    if (xdiff > 0) {
      x += ((float)i * xdiff) / (r * xdir);
    }
    if (ydiff > 0) {
      y += ((float)i * ydiff) / (r * ydir);
    }

    screen_set_braille_point(s, bb, x, y);
  }
}

void
screen_fill_rect(screen_t *s, rect_t r, tileset_e ts)
{
  for (int i = 0; i < r.h; i++) {
    for (int j = 0; j < r.w; j++) {
      s->buffer[(r.y + i) * s->w + r.x + j].c          = tileset[ts][TILE_FB];
      s->buffer[(r.y + i) * s->w + r.x + j].foreground = r.color;
    }
  }
}

void
screen_fill(screen_t *s, rect_t bb, int x1, int y1, int x2, int y2, int x3, int y3, int color)
{
#define SWAP(x, y)                                                                                 \
  do {                                                                                             \
    (x) = (x) ^ (y);                                                                               \
    (y) = (x) ^ (y);                                                                               \
    (x) = (x) ^ (y);                                                                               \
  } while (0)

  int  t1x, t2x, y, minx, maxx, t1xp, t2xp;
  bool changed1 = false;
  bool changed2 = false;
  int  signx1, signx2, dx1, dy1, dx2, dy2;
  int  e1, e2;
  int  i;

  /* Sort vertices */
  if (y1 > y2) {
    SWAP(y1, y2);
    SWAP(x1, x2);
  }

  if (y1 > y3) {
    SWAP(y1, y3);
    SWAP(x1, x3);
  }

  if (y2 > y3) {
    SWAP(y2, y3);
    SWAP(x2, x3);
  }

  /* Starting points */
  t1x = t2x = x1;
  y         = y1;

  dx1 = (int)(x2 - x1);
  if (dx1 < 0) {
    dx1    = -dx1;
    signx1 = -1;
  } else {
    signx1 = 1;
  }
  dy1 = (int)(y2 - y1);

  dx2 = (int)(x3 - x1);
  if (dx2 < 0) {
    dx2    = -dx2;
    signx2 = -1;
  } else {
    signx2 = 1;
  }
  dy2 = (int)(y3 - y1);

  if (dy1 > dx1) { /* swap values */
    SWAP(dx1, dy1);
    changed1 = true;
  }

  if (dy2 > dx2) { /* swap values */
    SWAP(dy2, dx2);
    changed2 = true;
  }

  e2 = (int)(dx2 >> 1);
  /* Flat top, just process the second half */
  if (y1 == y2) {
    goto next;
  }
  e1 = (int)(dx1 >> 1);

  for (i = 0; i < dx1;) {
    t1xp = 0;
    t2xp = 0;
    if (t1x < t2x) {
      minx = t1x;
      maxx = t2x;
    } else {
      minx = t2x;
      maxx = t1x;
    }
    /* process first line until y value is about to change */
    while (i < dx1) {
      i++;
      e1 += dy1;
      while (e1 >= dx1) {
        e1 -= dx1;
        if (changed1) {
          t1xp = signx1;
        } else {
          goto next1;
        }
      }
      if (changed1) {
        break;
      } else {
        t1x += signx1;
      }
    }
  /* move line */
  next1:
    /* process second line until y value is about to change */
    while (1) {
      e2 += dy2;
      while (e2 >= dx2) {
        e2 -= dx2;
        if (changed2) {
          t2xp = signx2;
        } else {
          goto next2;
        }
      }
      if (changed2) {
        break;
      } else {
        t2x += signx2;
      }
    }
  next2:
    if (minx > t1x) {
      minx = t1x;
    }
    if (minx > t2x) {
      minx = t2x;
    }
    if (maxx < t1x) {
      maxx = t1x;
    }
    if (maxx < t2x) {
      maxx = t2x;
    }

    screen_draw_braille_line(s, bb, (point_t){.x = minx, .y = y}, (point_t){.x = maxx, .y = y});

    /* now increase y */
    if (!changed1) {
      t1x += signx1;
    }
    t1x += t1xp;
    if (!changed2) {
      t2x += signx2;
    }
    t2x += t2xp;
    y += 1;
    if (y == y2) {
      break;
    }
  }
next:
  /* Second half */
  dx1 = (int)(x3 - x2);
  if (dx1 < 0) {
    dx1    = -dx1;
    signx1 = -1;
  } else {
    signx1 = 1;
  }
  dy1 = (int)(y3 - y2);
  t1x = x2;

  if (dy1 > dx1) { /* swap values */
    SWAP(dy1, dx1);
    changed1 = true;
  } else {
    changed1 = false;
  }

  e1 = (int)(dx1 >> 1);

  for (i = 0; i <= dx1; i++) {
    t1xp = 0;
    t2xp = 0;
    if (t1x < t2x) {
      minx = t1x;
      maxx = t2x;
    } else {
      minx = t2x;
      maxx = t1x;
    }
    /* process first line until y value is about to change */
    while (i < dx1) {
      e1 += dy1;
      while (e1 >= dx1) {
        e1 -= dx1;
        if (changed1) {
          t1xp = signx1;
          break;
        } else {
          goto next3;
        }
      }
      if (changed1) {
        break;
      } else {
        t1x += signx1;
      }
      if (i < dx1) {
        i++;
      }
    }
  next3:
    /* process second line until y value is about to change */
    while (t2x != x3) {
      e2 += dy2;
      while (e2 >= dx2) {
        e2 -= dx2;
        if (changed2) {
          t2xp = signx2;
        } else {
          goto next4;
        }
      }
      if (changed2) {
        break;
      } else {
        t2x += signx2;
      }
    }
  next4:
    if (minx > t1x) {
      minx = t1x;
    }
    if (minx > t2x) {
      minx = t2x;
    }
    if (maxx < t1x) {
      maxx = t1x;
    }
    if (maxx < t2x) {
      maxx = t2x;
    }

    screen_draw_braille_line(s, bb, (point_t){.x = minx, .y = y}, (point_t){.x = maxx, .y = y});

    /* now increase y */
    if (!changed1) {
      t1x += signx1;
    }
    t1x += t1xp;
    if (!changed2) {
      t2x += signx2;
    }
    t2x += t2xp;
    y += 1;
    if (y > y3) {
      return;
    }
  }

#undef SWAP
}
