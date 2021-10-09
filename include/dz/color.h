#ifndef __DZ_COLOR_H__
#define __DZ_COLOR_H__

typedef struct color {
  int r;
  int g;
  int b;
} color_t;

extern const struct color color_invalid;

extern const struct color color_white;
extern const struct color color_black;
extern const struct color color_red;
extern const struct color color_green;
extern const struct color color_blue;

#endif /* __DZ_COLOR_H__ */
