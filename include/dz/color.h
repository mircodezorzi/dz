#ifndef __DZ_COLOR_H__
#define __DZ_COLOR_H__

#include <dz/common.h>

/**
  \brief RGB color representation.

  \todo Use u32 instead. The 24 least significant bits will be used for respectively r, g, b.

    u32 color_from_rgb(char r, char g, char b) {
      return r << 16 | g << 8 | b;
    }
  */
typedef struct {
  int r;
  int g;
  int b;
} color_t;

extern const color_t color_invalid;

extern const color_t color_white;
extern const color_t color_black;
extern const color_t color_red;
extern const color_t color_green;
extern const color_t color_blue;

extern const color_t color_cyan;
extern const color_t color_yellow;
extern const color_t color_purple;
extern const color_t color_orange;

#endif /* __DZ_COLOR_H__ */
