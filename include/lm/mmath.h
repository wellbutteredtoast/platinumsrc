//
// mmath.h -> Matrix math for PlatinumSrc
//

#ifndef _MMATH_H
#define _MMATH_H

#include <math.h>
#include <string.h>
#include "vmath.h"

/* ------------------------------ Types ------------------------------ */

/* column-major, matches OpenGL's expected layout */
typedef struct { float m[16]; } lm_mat4;

/* ------------------------------ mat4 ------------------------------ */

static inline lm_mat4 lm_mat4_identity(void) {
    lm_mat4 r;
    memset(r.m, 0, sizeof(r.m));
    r.m[0]  = 1.0f;
    r.m[5]  = 1.0f;
    r.m[10] = 1.0f;
    r.m[15] = 1.0f;
    return r;
}

static inline lm_mat4 lm_mat4_mul(lm_mat4 a, lm_mat4 b) {
    lm_mat4 r;
    int i, j, k;
    memset(r.m, 0, sizeof(r.m));
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            for (k = 0; k < 4; k++) {
                r.m[i + j * 4] += a.m[i + k * 4] * b.m[k + j * 4];
            }
        }
    }
    return r;
}

static inline void lm_mat4_mulv(lm_mat4 m, lm_vec4 v, lm_vec4 *dest) {
    dest->x = m.m[0] * v.x + m.m[4] * v.y + m.m[8]  * v.z + m.m[12] * v.w;
    dest->y = m.m[1] * v.x + m.m[5] * v.y + m.m[9]  * v.z + m.m[13] * v.w;
    dest->z = m.m[2] * v.x + m.m[6] * v.y + m.m[10] * v.z + m.m[14] * v.w;
    dest->w = m.m[3] * v.x + m.m[7] * v.y + m.m[11] * v.z + m.m[15] * v.w;
}

static inline lm_mat4 lm_mat4_translate(lm_mat4 m, lm_vec3 v) {
    lm_mat4 r = m;
    r.m[12] = m.m[0] * v.x + m.m[4] * v.y + m.m[8]  * v.z + m.m[12];
    r.m[13] = m.m[1] * v.x + m.m[5] * v.y + m.m[9]  * v.z + m.m[13];
    r.m[14] = m.m[2] * v.x + m.m[6] * v.y + m.m[10] * v.z + m.m[14];
    r.m[15] = m.m[3] * v.x + m.m[7] * v.y + m.m[11] * v.z + m.m[15];
    return r;
}

static inline lm_mat4 lm_mat4_scale(lm_mat4 m, lm_vec3 v) {
    lm_mat4 r = m;
    r.m[0]  *= v.x;
    r.m[1]  *= v.x;
    r.m[2]  *= v.x;
    r.m[3]  *= v.x;
    r.m[4]  *= v.y;
    r.m[5]  *= v.y;
    r.m[6]  *= v.y;
    r.m[7]  *= v.y;
    r.m[8]  *= v.z;
    r.m[9]  *= v.z;
    r.m[10] *= v.z;
    r.m[11] *= v.z;
    return r;
}

static inline lm_mat4 lm_mat4_rotate(lm_mat4 m, float angle, lm_vec3 axis) {
    lm_mat4 r;
    lm_mat4 rot;
    float c  = cosf(angle);
    float s  = sinf(angle);
    float t  = 1.0f - c;
    lm_vec3 a = lm_vec3_norm(axis);

    memset(rot.m, 0, sizeof(rot.m));

    rot.m[0]  = t * a.x * a.x + c;
    rot.m[1]  = t * a.x * a.y + s * a.z;
    rot.m[2]  = t * a.x * a.z - s * a.y;
    rot.m[4]  = t * a.x * a.y - s * a.z;
    rot.m[5]  = t * a.y * a.y + c;
    rot.m[6]  = t * a.y * a.z + s * a.x;
    rot.m[8]  = t * a.x * a.z + s * a.y;
    rot.m[9]  = t * a.y * a.z - s * a.x;
    rot.m[10] = t * a.z * a.z + c;
    rot.m[15] = 1.0f;

    r = lm_mat4_mul(m, rot);
    return r;
}

static inline lm_mat4 lm_mat4_persp(float fov, float aspect, float near, float far) {
    lm_mat4 r;
    float f = 1.0f / tanf(fov * 0.5f);
    memset(r.m, 0, sizeof(r.m));
    r.m[0]  = f / aspect;
    r.m[5]  = f;
    r.m[10] = (far + near) / (near - far);
    r.m[11] = -1.0f;
    r.m[14] = (2.0f * far * near) / (near - far);
    return r;
}

static inline lm_mat4 lm_mat4_ortho(float left, float right, float bottom, float top, float near, float far) {
    lm_mat4 r;
    memset(r.m, 0, sizeof(r.m));
    r.m[0]  =  2.0f / (right - left);
    r.m[5]  =  2.0f / (top - bottom);
    r.m[10] = -2.0f / (far - near);
    r.m[12] = -(right + left)   / (right - left);
    r.m[13] = -(top   + bottom) / (top   - bottom);
    r.m[14] = -(far   + near)   / (far   - near);
    r.m[15] =  1.0f;
    return r;
}

static inline lm_mat4 lm_mat4_lookat(lm_vec3 eye, lm_vec3 center, lm_vec3 up) {
    lm_mat4 r;
    lm_vec3 f = lm_vec3_norm(lm_vec3_sub(center, eye));
    lm_vec3 s = lm_vec3_norm(lm_vec3_cross(f, up));
    lm_vec3 u = lm_vec3_cross(s, f);

    memset(r.m, 0, sizeof(r.m));
    r.m[0]  =  s.x;
    r.m[4]  =  s.y;
    r.m[8]  =  s.z;
    r.m[1]  =  u.x;
    r.m[5]  =  u.y;
    r.m[9]  =  u.z;
    r.m[2]  = -f.x;
    r.m[6]  = -f.y;
    r.m[10] = -f.z;
    r.m[12] = -lm_vec3_dot(s, eye);
    r.m[13] = -lm_vec3_dot(u, eye);
    r.m[14] =  lm_vec3_dot(f, eye);
    r.m[15] =  1.0f;
    return r;
}

#endif