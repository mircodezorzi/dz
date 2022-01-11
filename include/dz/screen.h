#ifndef __DZ_SCREEN_H__
#define __DZ_SCREEN_H__

#include <dz/color.h>
#include <dz/common.h>
#include <dz/point.h>

#include <wchar.h>

typedef enum {
  TILESET_SINGLE = 0,
  TILESET_DOUBLE = 1,
  TILESET_DASHED = 2,
  TILESET_BLOCK  = 3,

  TILESET_FULL_BLOCKS  = 4,
  TILESET_FULL_SHADOW1 = 5,
  TILESET_FULL_SHADOW2 = 5,
  TILESET_FULL_SHADOW3 = 6,
} tileset_e;

enum {
  TILE_TRC = 0, /* top-right corner    */
  TILE_TLC = 1, /* top-left corner     */
  TILE_BRC = 2, /* bottom-right corner */
  TILE_BLC = 3, /* bottom-left corner  */
  TILE_H   = 4, /* horizontal corner   */
  TILE_V   = 5, /* vertical corner     */
  TILE_UT  = 8, /* up facing T         */
  TILE_DT  = 9, /* down facing T       */
  TILE_RT  = 6, /* right facing T      */
  TILE_LT  = 7, /* left facing T       */

  TILE_FB   = 0, /* full block              */
  TILE_THB  = 1, /* top half block          */
  TILE_BHB  = 2, /* bottom half block       */
  TILE_LHB  = 3, /* left half block         */
  TILE_RHB  = 4, /* right half block        */
  TILE_TLHB = 5, /* top left half block     */
  TILE_TRHB = 6, /* top right half block    */
  TILE_BLHB = 7, /* bottom left half block  */
  TILE_BRHB = 8, /* bottom right half block */
};

typedef struct {
  wchar_t c;
  color_t foreground;
  color_t background;
} pixel_t;

typedef struct {
  int x;
  int y;
  int w;
  int h;

  color_t   color;
  tileset_e tileset;
} rect_t;

#define RECT(X, Y, W, H)                                                                                               \
  ((rect_t){                                                                                                           \
      .x = X,                                                                                                          \
      .y = Y,                                                                                                          \
      .w = W,                                                                                                          \
      .h = H,                                                                                                          \
  })

/**
  \brief Shrink rectangle by 1 pixel.
  */
#define SHRINK(Box) (RECT(Box.x + 1, Box.y + 1, Box.w - 1, Box.h - 1))

/**
  \brief Shrink rectangle by \p Num pixels.
  */
#define SHRINKN(Box, Num) (RECT(Box.x + Num, Box.y + Num, Box.w - Num, Box.h - Num))

/**
  \brief Rectangle with floating point coordinates.
  \see screen_draw_frect
  */
typedef struct {
  float x;
  float y;
  float w;
  float h;

  color_t color;
} frect_t;

/**
  \brief Screen buffer.
  */
typedef struct {
  int w;
  int h;

  /* Only redraw when this flag is true. */
  bool changed;

  /* Screen buffer */
  pixel_t *buffer;

  /* Depth buffer */
  float *detph;
} screen_t;

screen_t *screen_new(void);
void      screen_free(screen_t *s);

/**
  \brief Render screen buffer to terminal.
  */
void screen_render(screen_t *s);

/**
  \brief Reset screen to all black.
  */
void screen_repaint(screen_t *s);

/**
  \brief Update screen buffer.
  \todo Use signals.
 */
void screen_update(screen_t *s);

/**
  \brief Print a single pixel to the screen.
  Uses ANSI escape codes to set foreground and background colors.

  \todo Check for truecolor support on init.
  */
void screen_pixel_print(pixel_t p, pixel_t old);

/**
  \brief Merge two box drawing characters.

  I had to hardcode all the values inside of the function.
    "3.6 Roentgen; Not Great, Not Terrible"

  In C++ this function could also be written as a hashmap.

  You could write a scipt to emit this without having to manually write it.
*/
wchar_t screen_merge_chars(wchar_t a, wchar_t b);

void screen_draw_rect(screen_t *s, rect_t r);

/**
  \brief Draw text inside of bounding box `r`.
  \param r Text bounding box.
  */
void screen_draw_text(screen_t *s, rect_t r, const char *text, unsigned len, color_t color);

/**
  \brief Draw vertical line
  \param len Length of line.
  */
void screen_draw_vline(screen_t *s, point_t a, int len, tileset_e ts, color_t color);

/**
  \brief Draw horizontal line
  \param len Length of line.
  */
void screen_draw_hline(screen_t *s, point_t a, int len, tileset_e ts, color_t color);

/**
  \brief Draw vertical line
  \param len Length of line.
  */
void screen_draw_veline(screen_t *s, point_t a, int len, tileset_e ts, color_t color);

/**
  \brief Draw horizontal line
  \param len Length of line.
  */
void screen_draw_heline(screen_t *s, point_t a, int len, tileset_e ts, color_t color);

/**
  \brief Draws a straight line from `a` to `b`.
  */
void screen_draw_line(screen_t *s, point_t a, point_t b, tileset_e ts, color_t color, bool endings);

/**
  \brief Allows to draw non-character aligned rectangles.
  \todo (optimization) Rewrite this as a single pass.
  \todo (optimization) Rewrite for loops for consecutive memory allocations.
                       tl;dr: Fix `i` and `j`.
  */
void screen_draw_frect(screen_t *s, frect_t r);

/**
  \brief Draw line from point `a` to point `b`.
  \param bb Target bounding box
  \todo (optimization)
  */
void screen_draw_braille_line(screen_t *s, rect_t bb, point_t a, point_t b);

void screen_fill_rect(screen_t *s, rect_t r, tileset_e ts);

void screen_fill(screen_t *s, rect_t bb, int x1, int y1, int x2, int y2, int x3, int y3, int color);

#endif /* __DZ_SCREEN_H__ */
