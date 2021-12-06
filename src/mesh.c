#include <dz/mesh.h>

#include <dz/acglm.h>
#include <dz/amath.h>

#include <stdio.h>
#include <stdlib.h>

mesh_t *
mesh_load(const char *filepath)
{
  mesh_t *m    = malloc(sizeof(mesh_t));
  m->data      = malloc(sizeof(float) * DZ_MESH_INITIAL_CAPACITY);
  m->verticies = 0;

  float *verticies = malloc(sizeof(float) * DZ_MESH_INITIAL_CAPACITY);

  int verticies_capacity = DZ_MESH_INITIAL_CAPACITY;
  int data_capacity      = DZ_MESH_INITIAL_CAPACITY;

  int i = 0; /* m->data write index   */
  int j = 0; /* verticies write index */

  FILE *fp = fopen(filepath, "r");

  int ch;

  while ((ch = fgetc(fp)) != EOF) {
    switch (ch) {
    case 'f': {
      int a, b, c;
      fscanf(fp, "%d%d%d\n", &a, &b, &c);

      /* resize data array if needed */
      if (j + 9 >= data_capacity) {
        data_capacity *= 2;
        float *data = realloc(m->data, sizeof(float) * data_capacity);
        if (data) {
          m->data = data;
        } else {
          mesh_free(m);
          return NULL;
        }
      }

      m->data[j++] = verticies[3 * (a - 1) + 0];
      m->data[j++] = verticies[3 * (a - 1) + 1];
      m->data[j++] = verticies[3 * (a - 1) + 2];
      m->data[j++] = verticies[3 * (b - 1) + 0];
      m->data[j++] = verticies[3 * (b - 1) + 1];
      m->data[j++] = verticies[3 * (b - 1) + 2];
      m->data[j++] = verticies[3 * (c - 1) + 0];
      m->data[j++] = verticies[3 * (c - 1) + 1];
      m->data[j++] = verticies[3 * (c - 1) + 2];

      m->verticies += 9;

      break;
    }
    case 'v': {
      float a, b, c;
      fscanf(fp, "%f%f%f\n", &a, &b, &c);

      /* resize verticies array if needed */
      if (i + 3 >= verticies_capacity) {
        verticies_capacity *= 2;
        float *v = realloc(verticies, sizeof(float) * verticies_capacity);
        if (v) {
          verticies = v;
        } else {
          mesh_free(m);
          return NULL;
        }
      }

      verticies[i++] = a;
      verticies[i++] = b;
      verticies[i++] = c;

      break;
    }
    default:
      while ((ch = fgetc(fp)) != '\n' && fp)
        ;
      break;
    }
  }

  if (verticies) {
    free(verticies);
  }

  float *data = realloc(m->data, sizeof(float) * m->verticies);
  if (data) {
    m->data = data;
  } else {
    mesh_free(m);
    return NULL;
  }

  return m;
}

void
mesh_free(mesh_t *m)
{
  free(m->data);
  free(m);
}

static void
mesh_draw_line(screen_t *s,
               rect_t    bb,
               camera_t *c,
               float     x1,
               float     y1,
               float     z1,
               float     x2,
               float     y2,
               float     z2)
{
  vec3 v1, v2;

  camera_map(c, (vec3){x1, y1, z1}, v1);
  camera_map(c, (vec3){x2, y2, z2}, v2);

  screen_draw_braille_line(s,
                           bb,
                           (point_t){.x = v1[0], .y = v1[1]},
                           (point_t){.x = v2[0], .y = v2[1]});
}

static bool
shoulddraw(float ax,
           float ay,
           float az,
           float bx,
           float by,
           float bz,
           float cx,
           float cy,
           float cz,
           float camx,
           float camy,
           float camz)
{
  float xx = bx - ax;
  float xy = by - ay;
  float xz = bz - az;
  float yx = cx - ax;
  float yy = cy - ay;
  float yz = cz - az;

  float nx = xy * yz - az * by;
  float ny = xz * yx - ax * bz;
  float nz = xx * yy - ay * bx;

  float l = sqrtf(nx * nx + ny * ny + nz * nz);

  nx /= l;
  ny /= l;
  nz /= l;

  return nx * (ax - camx) + ny * (ay - camy) + nz * (az - camz) < 0.0f;
}

static void
mesh_fill(screen_t *s, rect_t bb, camera_t *c, float *points)
{
  vec3 v1, v2, v3;

  camera_map(c, (vec3){points[0], points[1], points[2]}, v1);
  camera_map(c, (vec3){points[3], points[4], points[5]}, v2);
  camera_map(c, (vec3){points[6], points[7], points[8]}, v3);

  if (shoulddraw(points[0],
                 points[1],
                 points[2],
                 points[3],
                 points[4],
                 points[5],
                 points[6],
                 points[7],
                 points[8],
                 c->position[0],
                 c->position[1],
                 c->position[2])) {
    screen_fill(s, bb, v1[0], v1[1], v2[0], v2[1], v3[0], v3[1], 0);
  }
}

void
mesh_draw(screen_t *s, rect_t bb, camera_t *c, mesh_t *m)
{
  /* clang-format off */
  int i;
  for (i = 0; i < m->verticies; i += 9) {
    if (false) {
      mesh_fill(s, bb, c, m->data + i);
    } else {
      mesh_draw_line(s, bb, c,
                     m->data[i + 0], m->data[i + 1], m->data[i + 2],
                     m->data[i + 3], m->data[i + 4], m->data[i + 5]);
      mesh_draw_line(s, bb, c,
                     m->data[i + 3], m->data[i + 4], m->data[i + 5],
                     m->data[i + 6], m->data[i + 7], m->data[i + 8]);
      mesh_draw_line(s, bb, c,
                     m->data[i + 0], m->data[i + 1], m->data[i + 2],
                     m->data[i + 6], m->data[i + 7], m->data[i + 8]);
    }
  }
}
