#include <dz/color.h>

#define DEFCOLOR(Name, Red, Green, Blue)                                                           \
  const struct color color_##Name = {                                                              \
      Red,                                                                                         \
      Green,                                                                                       \
      Blue,                                                                                        \
  }

DEFCOLOR(invalid, -1, -1, -1);

DEFCOLOR(white, 255, 255, 255);
DEFCOLOR(black, 0, 0, 0);
DEFCOLOR(red, 255, 0, 0);
DEFCOLOR(green, 0, 0, 255);
DEFCOLOR(blue, 0, 0, 255);

#undef DEFCOLOR
