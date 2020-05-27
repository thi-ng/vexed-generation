//                                 .___
//  ___  __ ____ ___  ___ ____   __| _/
//  \  \/ // __ \\  \/  // __ \ / __ |
//   \   /\  ___/ >    <\  ___// /_/ |
//    \_/  \___  >__/\_ \\___  >____ |
//             \/      \/    \/     \/         __  .__
//     ____   ____   ____   ________________ _/  |_|__| ____   ____
//    / ___\_/ __ \ /    \_/ __ \_  __ \__  \\   __\  |/  _ \ /    \
//   / /_/  >  ___/|   |  \  ___/|  | \// __ \|  | |  (  <_> )   |  \
//   \___  / \___  >___|  /\___  >__|  (____  /__| |__|\____/|___|  /
//  /_____/      \/     \/     \/           \/                    \/
//
//  (c) 2016 Karsten Schmidt // ASL 2.0 licensed

#ifndef __vgen_math_h__
#define __vgen_math_h__

#include <math.h>

#define EPS 1e-6
#define HALF_PI M_PI_2
#define THIRD_PI (M_PI / 3)
#define TAU M_TWO_PI

/**
 * Like VEX built-in `clamp()`, but supports vector2
 */
#define VG_CLAMP(TYPE)            \
    TYPE vg_clamp(TYPE x, a, b) { \
        return clamp(x, a, b);    \
    }

VG_CLAMP(int)
VG_CLAMP(float)
VG_CLAMP(vector2)
VG_CLAMP(vector)
VG_CLAMP(vector4)
#undef VG_CLAMP

/**
 * Smooth minimum functions based on:
 * http://www.iquilezles.org/www/articles/smin/smin.htm
 *
 * Recommended default `k = 32`
 */

#define VG_SMINMAX(NAME)                                                  \
    vector2 NAME(vector2 a, b; float k) {                                 \
        vector2 c = set(NAME(a.x, b.x, k), NAME(a.y, b.y, k));            \
        return c;                                                         \
    }                                                                     \
    vector NAME(vector a, b; float k) {                                   \
        vector c =                                                        \
            set(NAME(a.x, b.x, k), NAME(a.y, b.y, k), NAME(a.z, b.z, k)); \
        return c;                                                         \
    }                                                                     \
    vector4 NAME(vector4 a, b; float k) {                                 \
        vector4 c = set(NAME(a.x, b.x, k), NAME(a.y, b.y, k),             \
                        NAME(a.z, b.z, k), NAME(a.w, b.w, k));            \
        return c;                                                         \
    }

float vg_smin_exp(float a, b, k) {
    return -log(exp(-k * a) + exp(-k * b)) / k;
}
VG_SMINMAX(vg_smin_exp)

float vg_smin_poly(float a, b, k) {
    float h = clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0);
    return vg_mix(b, a, h) - k * h * (1.0 - h);
}
VG_SMINMAX(vg_smin_poly)

float vg_smin_pow(float a, b, k) {
    float pa = pow(a, k);
    float pb = pow(b, k);
    return pow((pa * pb) / (pa + pb), 1.0 / k);
}
VG_SMINMAX(vg_smin_pow)

/**
 * Smooth minimum & maximum based on:
 * https://en.wikipedia.org/wiki/Smooth_maximum
 *
 * Recommended default `k = 32`
 */
float vg_smin(float a, b, k) {
    float ea = exp(-k * a);
    float eb = exp(-k * b);
    return (a * ea + b * eb) / (ea + eb);
}
VG_SMINMAX(vg_smin)

float vg_smax(float a, b, k) {
    float ea = exp(k * a);
    float eb = exp(k * b);
    return (a * ea + b * eb) / (ea + eb);
}
VG_SMINMAX(vg_smax)
#undef VG_SMINMAX

float vg_sclamp(float x, a, b, k) {
    return vg_smin(vg_smax(x, a, k), b, k);
}

vector2 vg_sclamp(vector2 x, a, b; float k) {
    return vg_smin(vg_smax(x, a, k), b, k);
}

vector vg_sclamp(vector x, a, b; float k) {
    return vg_smin(vg_smax(x, a, k), b, k);
}

vector4 vg_sclamp(vector4 x, a, b; float k) {
    return vg_smin(vg_smax(x, a, k), b, k);
}

/**
 * Returns lesser value, irrespective of sign
 */
float vg_absmin(float a, b) {
    return abs(a) < abs(b) ? a : b;
}

/**
 * Returns greater value, irrespective of sign
 */
float vg_absmax(float a, b) {
    return abs(a) > abs(b) ? a : b;
}

/**
 * Like VEX built-in `lerp()`, but I prefer naming used by GLSL
 * If called without `t`, uses t = 0.5
 */
#define VG_MIX(TYPE)                        \
    TYPE vg_mix(const TYPE a, b, t) {       \
        return a + (b - a) * t;             \
    }                                       \
    TYPE vg_mix(const TYPE a, b; float t) { \
        return a + (b - a) * t;             \
    }                                       \
    TYPE vg_mix(const TYPE a, b) {          \
        return (a + b) / 2;                 \
    }

float vg_mix(float a, b, t) {
    return a + (b - a) * t;
}

float vg_mix(float a, b) {
    return (a + b) / 2;
}

VG_MIX(vector2)
VG_MIX(vector)
VG_MIX(vector4)
#undef VG_MIX

/**
 * Bilinear interpolation:
 *
 * C   D   V
 * +---+   ^
 * |   |   |
 * +---+   +----> U
 * A   B
 *
 */
#define VG_MIX_BILINEAR(TYPE)                                       \
    TYPE vg_mix_bilinear(const TYPE a, b, c, d; float u, v) {       \
        return vg_mix(vg_mix(a, b, u), vg_mix(c, d, u), v);         \
    }                                                               \
    TYPE vg_mix_bilinear(const TYPE a, b, c, d; const vector2 uv) { \
        return vg_mix_bilinear(a, b, c, d, uv.x, uv.y);             \
    }

VG_MIX_BILINEAR(float)
VG_MIX_BILINEAR(vector2)
VG_MIX_BILINEAR(vector)
VG_MIX_BILINEAR(vector4)
#undef VG_MIX_BILINEAR

/**
 * Step function
 * @returns 0 if t < e, else 1
 */
float vg_step(float e, t) {
    return t < e ? 0 : 1;
}

vector2 vg_step(const vector2 e, t) {
    vector2 res;
    res.x = t.x < e.x ? 0 : 1;
    res.y = t.y < e.y ? 0 : 1;
    return res;
}

vector vg_step(const vector e, t) {
    vector res;
    res.x = t.x < e.x ? 0 : 1;
    res.y = t.y < e.y ? 0 : 1;
    res.z = t.z < e.z ? 0 : 1;
    return res;
}

vector4 vg_step(const vector4 e, t) {
    vector4 res;
    res.x = t.x < e.x ? 0 : 1;
    res.y = t.y < e.y ? 0 : 1;
    res.z = t.z < e.z ? 0 : 1;
    res.w = t.w < e.w ? 0 : 1;
    return res;
}

/**
 * Polynomial based soft step
 *
 * @returns 0 if t < e, 1 if t > e2, else interpolated value
 */
#define VG_SMOOTHSTEP(TYPE)                          \
    TYPE vg_smoothstep(const TYPE e, e2, t) {        \
        TYPE x = vg_clamp((t - e) / (e2 - e), 0, 1); \
        return (x * -2 + 3) * x * x;                 \
    }

VG_SMOOTHSTEP(float)
VG_SMOOTHSTEP(vector2)
VG_SMOOTHSTEP(vector)
VG_SMOOTHSTEP(vector4)
#undef VG_SMOOTHSTEP

/**
 * A softer version of smoothstep.
 *
 * @returns 0 if t < e, 1 if t > e2, else interpolated value
 */
#define VG_SMOOTHERSTEP(TYPE)                        \
    TYPE vg_smootherstep(const TYPE e, e2, t) {      \
        TYPE x = vg_clamp((t - e) / (e2 - e), 0, 1); \
        return x * x * x * (x * (x * 6 - 15) + 10);  \
    }

VG_SMOOTHERSTEP(float)
VG_SMOOTHERSTEP(vector2)
VG_SMOOTHERSTEP(vector)
VG_SMOOTHERSTEP(vector4)
#undef VG_SMOOTHERSTEP

/**
 * Rounding functions w/ step value
 */
float vg_floor(float x, prec) {
    return floor(x / prec) * prec;
}

float vg_ceil(float x, prec) {
    return ceil(x / prec) * prec;
}

float vg_round(float x, prec) {
    return rint(x / prec) * prec;
}

#define VG_ROUND(NAME)                                                 \
    vector2 NAME(vector2 v; float prec) {                              \
        return set(NAME(v.x, prec), NAME(v.y, prec));                  \
    }                                                                  \
    vector NAME(vector v; float prec) {                                \
        return set(NAME(v.x, prec), NAME(v.y, prec), NAME(v.z, prec)); \
    }                                                                  \
    vector4 NAME(vector4 v; float prec) {                              \
        return set(NAME(v.x, prec), NAME(v.y, prec), NAME(v.z, prec),  \
                   NAME(v.w, prec));                                   \
    }

VG_ROUND(vg_floor)
VG_ROUND(vg_ceil)
VG_ROUND(vg_round)
#undef VG_ROUND

/**
 * Computes value of one parameter modulo another
 */
#define VG_MOD(TYPE)                       \
    TYPE vg_mod(TYPE x, y) {               \
        return x - y * vg_floor(x / y, 1); \
    }

VG_MOD(float)
VG_MOD(vector2)
VG_MOD(vector)
VG_MOD(vector4)
#undef VG_MOD

/**
 * Returns fractional part of given value, i.e. `x - floor(x)`
 */
#define VG_FRACT(TYPE)             \
    TYPE vg_fract(TYPE x) {        \
        return x - vg_floor(x, 1); \
    }

VG_FRACT(float)
VG_FRACT(vector2)
VG_FRACT(vector)
VG_FRACT(vector4)
#undef VG_FRACT

int vg_eqdelta(float a, b, eps) {
    return abs(a - b) <= eps;
}

int vg_eqdelta(vector2 a, b; float eps) {
    return abs(a.x - b.x) <= eps && abs(a.y - b.y) <= eps;
}

int vg_eqdelta(vector a, b; float eps) {
    return abs(a.x - b.x) <= eps && abs(a.y - b.y) <= eps &&
           abs(a.z - b.z) <= eps;
}

int vg_eqdelta(vector4 a, b; float eps) {
    return abs(a.x - b.x) <= eps && abs(a.y - b.y) <= eps &&
           abs(a.z - b.z) <= eps && abs(a.w - b.w) <= eps;
}

int vg_sign(float x, eps) {
    return vg_eqdelta(x, 0, eps) ? 0 : x < 0 ? -1 : 1;
}

int vg_in_range(float x, min, max) {
    return x >= min && x <= max;
}

int vg_in_range(vector2 v, min, max) {
    return vg_in_range(v.x, min.x, max.x) && vg_in_range(v.y, min.y, max.y);
}

int vg_in_range(vector v, min, max) {
    return vg_in_range(v.x, min.x, max.x) && vg_in_range(v.y, min.y, max.y) &&
           vg_in_range(v.z, min.z, max.z);
}

int vg_in_range(vector4 v, min, max) {
    return vg_in_range(v.x, min.x, max.x) && vg_in_range(v.y, min.y, max.y) &&
           vg_in_range(v.z, min.z, max.z) && vg_in_range(v.w, min.w, max.w);
}

int vg_minid(float a, b) {
    return a <= b ? 0 : 1;
}

int vg_minid(float a, b, c) {
    return a <= b ? (a <= c ? 0 : 2) : b <= c ? 1 : 2;
}

int vg_minid(float a, b, c, d) {
    return a <= b
        ? a <= c
            ? a <= d
                ? 0
                : 3
            : c <= d
            ? 2
            : 3
        : b <= c
        ? b <= d
            ? 1
            : 3
        : c <= d
        ? 2
        : 3;
}

int vg_maxid(float a, b) {
    return a >= b ? 0 : 1;
}

int vg_maxid(float a, b, c) {
    return a >= b ? (a >= c ? 0 : 2) : b >= c ? 1 : 2;
}

int vg_maxid(float a, b, c, d) {
    return a >= b
        ? a >= c
            ? a >= d
                ? 0
                : 3
            : c >= d
            ? 2
            : 3
        : b >= c
        ? b >= d
            ? 1
            : 3
        : c >= d
        ? 2
        : 3;
}

#endif
