#ifndef __DZ_CAMERA_H__
#define __DZ_CAMERA_H__

#include <dz/acglm.h>

typedef struct {
  vec3 position;
  vec3 up;
  vec3 angle;

  mat4 projection;
  mat4 view;
} camera_t;

camera_t *camera_new(void);
void      camera_free(camera_t *c);
void      camera_update(camera_t *c);
void      camera_map(camera_t *c, vec3 v, vec4 dest);

#endif /* __DZ_CAMERA_H__ */
