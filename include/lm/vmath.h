//
// vmath.h -> Vector math for PlatinumSrc
//

#ifndef _VMATH_H
#define _VMATH_H

#include <math.h>

typedef struct { float x, y; }          lm_vec2; /* 2D vector */
typedef struct { float x, y, z; }       lm_vec3; /* 3D vector */
typedef struct { float x, y, z, w; }    lm_vec4; /* 4D vector */

/* ------------------------------ vec2 ------------------------------ */

static inline lm_vec2 lm_vec2_add(lm_vec2 a, lm_vec2 b) {
    lm_vec2 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    return r;
}

static inline lm_vec2 lm_vec2_sub(lm_vec2 a, lm_vec2 b) {
    lm_vec2 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    return r;
}

static inline lm_vec2 lm_vec2_scale(lm_vec2 v, float s) {
    lm_vec2 r;
    r.x = v.x * s;
    r.y = v.y * s;
    return r;
}

static inline float lm_vec2_dot(lm_vec2 a, lm_vec2 b) {
    return a.x * b.x + a.y * b.y;
}

static inline float lm_vec2_len(lm_vec2 v) {
    return sqrtf(lm_vec2_dot(v, v));
}

static inline lm_vec2 lm_vec2_norm(lm_vec2 v) {
    return lm_vec2_scale(v, 1.0f / lm_vec2_len(v));
}

/* ------------------------------ vec3 ------------------------------ */

static inline lm_vec3 lm_vec3_add(lm_vec3 a, lm_vec3 b) {
    lm_vec3 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}

static inline lm_vec3 lm_vec3_sub(lm_vec3 a, lm_vec3 b) {
    lm_vec3 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}

static inline lm_vec3 lm_vec3_scale(lm_vec3 v, float s) {
    lm_vec3 r;
    r.x = v.x * s;
    r.y = v.y * s;
    r.z = v.z * s;
    return r;
}

static inline float lm_vec3_dot(lm_vec3 a, lm_vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline float lm_vec3_len(lm_vec3 v) {
    return sqrtf(lm_vec3_dot(v, v));
}

static inline lm_vec3 lm_vec3_norm(lm_vec3 v) {
    return lm_vec3_scale(v, 1.0f / lm_vec3_len(v));
}

static inline lm_vec3 lm_vec3_cross(lm_vec3 a, lm_vec3 b) {
    lm_vec3 r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
}

/* ------------------------------ vec4 ------------------------------ */

static inline lm_vec4 lm_vec4_add(lm_vec4 a, lm_vec4 b) {
    lm_vec4 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    r.w = a.w + b.w;
    return r;
}

static inline lm_vec4 lm_vec4_sub(lm_vec4 a, lm_vec4 b) {
    lm_vec4 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    r.w = a.w - b.w;
    return r;
}

static inline lm_vec4 lm_vec4_scale(lm_vec4 v, float s) {
    lm_vec4 r;
    r.x = v.x * s;
    r.y = v.y * s;
    r.z = v.z * s;
    r.w = v.w * s;
    return r;
}

static inline float lm_vec4_dot(lm_vec4 a, lm_vec4 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

static inline float lm_vec4_len(lm_vec4 v) {
    return sqrtf(lm_vec4_dot(v, v));
}

static inline lm_vec4 lm_vec4_norm(lm_vec4 v) {
    return lm_vec4_scale(v, 1.0f / lm_vec4_len(v));
}

#endif /* _VMATH_H*/