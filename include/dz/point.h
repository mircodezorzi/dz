#ifndef __DZ_POINT_H__
#define __DZ_POINT_H__

/**
  \brief Shrink rectangle.
  */
#define SHRINK(Box) ((rect_t){.x = Box.x + 1, .y = Box.y + 1, .w = Box.w - 1, .h = Box.h - 1})

/**
  \brief Shrink rectangle by n pixels.
  */
#define SHRINKN(Box, Num)                                                                          \
  ((rect_t){.x = Box.x + Num, .y = Box.y + Num, .w = Box.w - Num, .h = Box.h - Num})

/**
  \brief Transpose point
  */
#define P(Point, Dx, Dy) ((point_t){.x = Point.x + Dx, .y = Point.y + Dy})

typedef struct {
  int x;
  int y;
} point_t;

#endif /* __DZ_POINT_H__ */
