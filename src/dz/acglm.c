#include <dz/acglm.h>
#include <dz/amath.h>

#include <math.h>

void
aglm_lookat(vec3 eye, vec3 center, vec3 up, mat4 dest)
{
  vec3 f, u, s;

  aglm_vec3_sub(center, eye, f);
  aglm_vec3_normalize(f);

  aglm_vec3_crossn(f, up, s);
  aglm_vec3_cross(s, f, u);

  dest[0][0] = s[0];
  dest[0][1] = u[0];
  dest[0][2] = -f[0];
  dest[1][0] = s[1];
  dest[1][1] = u[1];
  dest[1][2] = -f[1];
  dest[2][0] = s[2];
  dest[2][1] = u[2];
  dest[2][2] = -f[2];
  dest[3][0] = -aglm_vec3_dot(s, eye);
  dest[3][1] = -aglm_vec3_dot(u, eye);
  dest[3][2] = aglm_vec3_dot(f, eye);
  dest[0][3] = dest[1][3] = dest[2][3] = 0.0f;
  dest[3][3]                           = 1.0f;
}

void
aglm_perspective(float fovy, float aspect, float nearZ, float farZ, mat4 dest)
{
  float f, fn;

  aglm_mat4_zero(dest);

  f  = 1.0f / tanf(fovy * 0.5f);
  fn = 1.0f / (nearZ - farZ);

  dest[0][0] = f / aspect;
  dest[1][1] = f;
  dest[2][2] = (nearZ + farZ) * fn;
  dest[2][3] = -1.0f;
  dest[3][2] = 2.0f * nearZ * farZ * fn;
}

void
aglm_rotate(mat4 m, float angle, vec3 axis)
{
  mat4 rot;
  aglm_rotate_make(rot, angle, axis);
  aglm_mul_rot(m, rot, m);
}

void
aglm_rotate_make(mat4 m, float angle, vec3 axis)
{
  vec3  axisn, v, vs;
  float c;

  c = cosf(angle);

  aglm_vec3_normalize_to(axis, axisn);
  aglm_vec3_scale(axisn, 1.0f - c, v);
  aglm_vec3_scale(axisn, sinf(angle), vs);

  aglm_vec3_scale(axisn, v[0], m[0]);
  aglm_vec3_scale(axisn, v[1], m[1]);
  aglm_vec3_scale(axisn, v[2], m[2]);

  m[0][0] += c;
  m[1][0] -= vs[2];
  m[2][0] += vs[1];
  m[0][1] += vs[2];
  m[1][1] += c;
  m[2][1] -= vs[0];
  m[0][2] -= vs[1];
  m[1][2] += vs[0];
  m[2][2] += c;

  m[0][3] = m[1][3] = m[2][3] = m[3][0] = m[3][1] = m[3][2] = 0.0f;
  m[3][3]                                                   = 1.0f;
}

void
aglm_translate(mat4 m, vec3 v)
{
  aglm_vec4_muladds(m[0], v[0], m[3]);
  aglm_vec4_muladds(m[1], v[1], m[3]);
  aglm_vec4_muladds(m[2], v[2], m[3]);
}

void
aglm_vec3(vec4 v4, vec3 dest)
{
  dest[0] = v4[0];
  dest[1] = v4[1];
  dest[2] = v4[2];
}

void
aglm_vec3_add(vec3 a, vec3 b, vec3 dest)
{
  dest[0] = a[0] + b[0];
  dest[1] = a[1] + b[1];
  dest[2] = a[2] + b[2];
}

void
aglm_vec3_copy(vec3 a, vec3 dest)
{
  dest[0] = a[0];
  dest[1] = a[1];
  dest[2] = a[2];
}

void
aglm_vec3_cross(vec3 a, vec3 b, vec3 dest)
{
  vec3 c;
  c[0] = a[1] * b[2] - a[2] * b[1];
  c[1] = a[2] * b[0] - a[0] * b[2];
  c[2] = a[0] * b[1] - a[1] * b[0];
  aglm_vec3_copy(c, dest);
}

void
aglm_vec3_crossn(vec3 a, vec3 b, vec3 dest)
{
  aglm_vec3_cross(a, b, dest);
  aglm_vec3_normalize(dest);
}

void
aglm_vec3_normalize(vec3 v)
{
  float norm;

  norm = aglm_vec3_norm(v);

  if (norm == 0.0f) {
    v[0] = v[1] = v[2] = 0.0f;
    return;
  }

  aglm_vec3_scale(v, 1.0f / norm, v);
}

void
aglm_vec3_normalize_to(vec3 v, vec3 dest)
{
  float norm;

  norm = aglm_vec3_norm(v);

  if (norm == 0.0f) {
    aglm_vec3_zero(dest);
    return;
  }

  aglm_vec3_scale(v, 1.0f / norm, dest);
}

void
aglm_vec3_scale(vec3 v, float s, vec3 dest)
{
  dest[0] = v[0] * s;
  dest[1] = v[1] * s;
  dest[2] = v[2] * s;
}

void
aglm_vec3_sub(vec3 a, vec3 b, vec3 dest)
{
  dest[0] = a[0] - b[0];
  dest[1] = a[1] - b[1];
  dest[2] = a[2] - b[2];
}

void
aglm_vec3_zero(vec3 v)
{
  v[0] = v[1] = v[2] = 0.0f;
}

float
aglm_vec3_dot(vec3 a, vec3 b)
{
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

float
aglm_vec3_norm(vec3 v)
{
  return sqrtf(aglm_vec3_norm2(v));
}

float
aglm_vec3_norm2(vec3 v)
{
  return aglm_vec3_dot(v, v);
}

void
aglm_vec4(vec3 v3, float last, vec4 dest)
{
  dest[0] = v3[0];
  dest[1] = v3[1];
  dest[2] = v3[2];
  dest[3] = last;
}

void
aglm_vec4_copy(vec4 v, vec4 dest)
{
  dest[0] = v[0];
  dest[1] = v[1];
  dest[2] = v[2];
  dest[3] = v[3];
}

void
aglm_vec4_muladds(vec4 a, float s, vec4 dest)
{
  dest[0] += a[0] * s;
  dest[1] += a[1] * s;
  dest[2] += a[2] * s;
  dest[3] += a[3] * s;
}

void
aglm_mat4_zero(mat4 mat)
{
  mat4 t = AGLM_MAT4_ZERO_INIT;
  aglm_mat4_copy(t, mat);
}

void
aglm_mat4_copy(mat4 mat, mat4 dest)
{
  dest[0][0] = mat[0][0];
  dest[1][0] = mat[1][0];
  dest[0][1] = mat[0][1];
  dest[1][1] = mat[1][1];
  dest[0][2] = mat[0][2];
  dest[1][2] = mat[1][2];
  dest[0][3] = mat[0][3];
  dest[1][3] = mat[1][3];

  dest[2][0] = mat[2][0];
  dest[3][0] = mat[3][0];
  dest[2][1] = mat[2][1];
  dest[3][1] = mat[3][1];
  dest[2][2] = mat[2][2];
  dest[3][2] = mat[3][2];
  dest[2][3] = mat[2][3];
  dest[3][3] = mat[3][3];
}

void
aglm_mat4_mul(mat4 m1, mat4 m2, mat4 dest)
{
  float a00 = m1[0][0], a01 = m1[0][1], a02 = m1[0][2], a03 = m1[0][3],
        a10 = m1[1][0], a11 = m1[1][1], a12 = m1[1][2], a13 = m1[1][3],
        a20 = m1[2][0], a21 = m1[2][1], a22 = m1[2][2], a23 = m1[2][3],
        a30 = m1[3][0], a31 = m1[3][1], a32 = m1[3][2], a33 = m1[3][3],

        b00 = m2[0][0], b01 = m2[0][1], b02 = m2[0][2], b03 = m2[0][3],
        b10 = m2[1][0], b11 = m2[1][1], b12 = m2[1][2], b13 = m2[1][3],
        b20 = m2[2][0], b21 = m2[2][1], b22 = m2[2][2], b23 = m2[2][3],
        b30 = m2[3][0], b31 = m2[3][1], b32 = m2[3][2], b33 = m2[3][3];

  dest[0][0] = a00 * b00 + a10 * b01 + a20 * b02 + a30 * b03;
  dest[0][1] = a01 * b00 + a11 * b01 + a21 * b02 + a31 * b03;
  dest[0][2] = a02 * b00 + a12 * b01 + a22 * b02 + a32 * b03;
  dest[0][3] = a03 * b00 + a13 * b01 + a23 * b02 + a33 * b03;
  dest[1][0] = a00 * b10 + a10 * b11 + a20 * b12 + a30 * b13;
  dest[1][1] = a01 * b10 + a11 * b11 + a21 * b12 + a31 * b13;
  dest[1][2] = a02 * b10 + a12 * b11 + a22 * b12 + a32 * b13;
  dest[1][3] = a03 * b10 + a13 * b11 + a23 * b12 + a33 * b13;
  dest[2][0] = a00 * b20 + a10 * b21 + a20 * b22 + a30 * b23;
  dest[2][1] = a01 * b20 + a11 * b21 + a21 * b22 + a31 * b23;
  dest[2][2] = a02 * b20 + a12 * b21 + a22 * b22 + a32 * b23;
  dest[2][3] = a03 * b20 + a13 * b21 + a23 * b22 + a33 * b23;
  dest[3][0] = a00 * b30 + a10 * b31 + a20 * b32 + a30 * b33;
  dest[3][1] = a01 * b30 + a11 * b31 + a21 * b32 + a31 * b33;
  dest[3][2] = a02 * b30 + a12 * b31 + a22 * b32 + a32 * b33;
  dest[3][3] = a03 * b30 + a13 * b31 + a23 * b32 + a33 * b33;
}

void
aglm_mat4_mulN(mat4 *__restrict matrices[], uint32_t len, mat4 dest)
{
  uint32_t i;

  aglm_mat4_mul(*matrices[0], *matrices[1], dest);

  for (i = 2; i < len; i++) {
    aglm_mat4_mul(dest, *matrices[i], dest);
  }
}

void
aglm_mat4_mulv(mat4 m, vec4 v, vec4 dest)
{
  vec4 res;
  res[0] = m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2] + m[3][0] * v[3];
  res[1] = m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2] + m[3][1] * v[3];
  res[2] = m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2] + m[3][2] * v[3];
  res[3] = m[0][3] * v[0] + m[1][3] * v[1] + m[2][3] * v[2] + m[3][3] * v[3];
  aglm_vec4_copy(res, dest);
}

void
aglm_mat4_mulv3(mat4 m, vec3 v, float last, vec3 dest)
{
  vec4 res;
  aglm_vec4(v, last, res);
  aglm_mat4_mulv(m, res, res);
  aglm_vec3(res, dest);
}

void
aglm_mul_rot(mat4 m1, mat4 m2, mat4 dest)
{
  float a00 = m1[0][0], a01 = m1[0][1], a02 = m1[0][2], a03 = m1[0][3],
        a10 = m1[1][0], a11 = m1[1][1], a12 = m1[1][2], a13 = m1[1][3],
        a20 = m1[2][0], a21 = m1[2][1], a22 = m1[2][2], a23 = m1[2][3],
        a30 = m1[3][0], a31 = m1[3][1], a32 = m1[3][2], a33 = m1[3][3],

        b00 = m2[0][0], b01 = m2[0][1], b02 = m2[0][2], b10 = m2[1][0],
        b11 = m2[1][1], b12 = m2[1][2], b20 = m2[2][0], b21 = m2[2][1],
        b22 = m2[2][2];

  dest[0][0] = a00 * b00 + a10 * b01 + a20 * b02;
  dest[0][1] = a01 * b00 + a11 * b01 + a21 * b02;
  dest[0][2] = a02 * b00 + a12 * b01 + a22 * b02;
  dest[0][3] = a03 * b00 + a13 * b01 + a23 * b02;

  dest[1][0] = a00 * b10 + a10 * b11 + a20 * b12;
  dest[1][1] = a01 * b10 + a11 * b11 + a21 * b12;
  dest[1][2] = a02 * b10 + a12 * b11 + a22 * b12;
  dest[1][3] = a03 * b10 + a13 * b11 + a23 * b12;

  dest[2][0] = a00 * b20 + a10 * b21 + a20 * b22;
  dest[2][1] = a01 * b20 + a11 * b21 + a21 * b22;
  dest[2][2] = a02 * b20 + a12 * b21 + a22 * b22;
  dest[2][3] = a03 * b20 + a13 * b21 + a23 * b22;

  dest[3][0] = a30;
  dest[3][1] = a31;
  dest[3][2] = a32;
  dest[3][3] = a33;
}
