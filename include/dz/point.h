#ifndef __DZ_POINT_H__
#define __DZ_POINT_H__

typedef struct {
  int x;
  int y;
} point_t;

/**
  \brief Shorthand to construct points.
  */
#define POINT(X, Y)                                                                                                    \
  ((point_t){                                                                                                          \
      .x = X,                                                                                                          \
      .y = Y,                                                                                                          \
  })

/**
  \brief Transpose point by (\p Dx, \p Dy).
  */
#define TP(Point, Dx, Dy) (POINT(Point.x + Dx, Point.y + Dy))

#endif /* __DZ_POINT_H__ */
