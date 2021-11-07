#ifndef __ACGLM_H__
#define __ACGLM_H__

#include <stdint.h>

#define M_PI_4 0.785398163397448309616

#define vec3_assign(Vec, X, Y, Z)                                              \
  do {                                                                         \
    Vec[0] = X;                                                                \
    Vec[1] = Y;                                                                \
    Vec[2] = Z;                                                                \
  } while (0);

/* clang-format off */
#define AGLM_MAT4_IDENTITY_INIT {{1.0f, 0.0f, 0.0f, 0.0f},                      \
                                 {0.0f, 1.0f, 0.0f, 0.0f},                      \
                                 {0.0f, 0.0f, 1.0f, 0.0f},                      \
                                 {0.0f, 0.0f, 0.0f, 1.0f}}

#define AGLM_MAT4_ZERO_INIT     {{0.0f, 0.0f, 0.0f, 0.0f},                    \
                                 {0.0f, 0.0f, 0.0f, 0.0f},                    \
                                 {0.0f, 0.0f, 0.0f, 0.0f},                    \
                                 {0.0f, 0.0f, 0.0f, 0.0f}}

/* clang-format on */

typedef float vec3[3];
typedef float vec4[4];
typedef float mat4[4][4];

void aglm_lookat(vec3 eye, vec3 center, vec3 up, mat4 dest);
void
aglm_perspective(float fovy, float aspect, float nearZ, float farZ, mat4 dest);

void aglm_rotate(mat4 m, float angle, vec3 axis);
void aglm_rotate_make(mat4 m, float angle, vec3 axis);

void aglm_translate(mat4 m, vec3 v);

void  aglm_vec3(vec4 v4, vec3 dest);
void  aglm_vec3_add(vec3 a, vec3 b, vec3 dest);
void  aglm_vec3_copy(vec3 a, vec3 dest);
void  aglm_vec3_cross(vec3 a, vec3 b, vec3 dest);
void  aglm_vec3_crossn(vec3 a, vec3 b, vec3 dest);
void  aglm_vec3_normalize(vec3 v);
void  aglm_vec3_normalize_to(vec3 v, vec3 dest);
void  aglm_vec3_scale(vec3 v, float s, vec3 dest);
void  aglm_vec3_sub(vec3 a, vec3 b, vec3 dest);
void  aglm_vec3_zero(vec3 v);
float aglm_vec3_dot(vec3 a, vec3 b);
float aglm_vec3_norm(vec3 v);
float aglm_vec3_norm2(vec3 v);

void aglm_vec4(vec3 v3, float last, vec4 dest);
void aglm_vec4_copy(vec4 v, vec4 dest);
void aglm_vec4_muladds(vec4 a, float s, vec4 dest);

void aglm_mat4_copy(mat4 mat, mat4 dest);
void aglm_mat4_mul(mat4 m1, mat4 m2, mat4 dest);
void aglm_mat4_mulN(mat4 *__restrict matrices[], uint32_t len, mat4 dest);
void aglm_mat4_mulv(mat4 m, vec4 v, vec4 dest);
void aglm_mat4_mulv3(mat4 m, vec3 v, float last, vec3 dest);
void aglm_mat4_zero(mat4 mat);

void aglm_mul_rot(mat4 m1, mat4 m2, mat4 dest);

#endif /* __ACGLM_H__ */
