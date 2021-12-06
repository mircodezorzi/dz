#ifndef __DZ_MESH_H__
#define __DZ_MESH_H__

#include <dz/camera.h>
#include <dz/screen.h>

#define DZ_MESH_INITIAL_CAPACITY 1024

typedef struct {
  /* verticies */
  float *data;
  /* number of verticies */
  int verticies;
} mesh_t;

mesh_t *mesh_load(const char *filepath);
void    mesh_free(mesh_t *m);

/**
  \brief Draw mesh to screen.
  \param bb Target bounding box
  */
void mesh_draw(screen_t *s, rect_t bb, camera_t *c, mesh_t *m);

#endif /* __DZ_MESH_H__ */
