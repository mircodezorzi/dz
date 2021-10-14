#include <dz/camera.h>
#include <sys/ioctl.h>
#include <unistd.h>

camera_t *
camera_new()
{
  camera_t *c = malloc(sizeof(camera_t));
  vec3_assign(c->position, 5.0f, 0.0f, 5.0f);
  vec3_assign(c->up, 0.0f, 1.0f, 0.0f);
  vec3_assign(c->angle, 0.3f, 0.2f, -1.0f);
  glm_perspective(M_PI_4, 1.0f, 0.1f, 100.0f, c->projection);
  camera_update(c);
  return c;
}

void
camera_update(camera_t *c)
{
  vec3 target;
  glm_vec3_add(c->position, c->angle, target);
  glm_lookat(c->position, target, c->up, c->view);
}

void
camera_map(camera_t *c, vec3 v, vec4 dest)
{
  mat4 model = GLM_MAT4_IDENTITY_INIT;
  mat4 m;

  glm_translate(model, v);
  glm_mat4_mulN((mat4 *[]){&c->projection, &c->view, &model}, 3, m);
  glm_mat4_mulv3(m, v, 1, dest);
}
