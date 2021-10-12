#ifndef __DZ_CAMERA_H__
#define __DZ_CAMERA_H__

#include <cglm/cglm.h>

#define vec3_assign(Vec, X, Y, Z)                                                        \
  do {                                                                                   \
    Vec[0] = X;                                                                          \
    Vec[1] = Y;                                                                          \
    Vec[2] = Z;                                                                          \
  } while (0);

typedef struct {
  vec3 position;
  vec3 up;
  vec3 angle;

  mat4 projection;
  mat4 view;
} camera_t;

camera_t *camera_new(void);
void      camera_update(camera_t *c);
void      camera_map(camera_t *c, vec3 v, vec4 dest);

#endif /* __DZ_CAMERA_H__ */
