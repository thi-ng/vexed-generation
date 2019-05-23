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

#ifndef __vgen_plane_h__
#define __vgen_plane_h__

#include <vgen_math.h>
#include <vgen_vector.h>

vector4 vg_plane(vector normal; float w) {
    return vg_vec4(normal, w);
}

/**
 * Returns new plane in normal form from given `normal` and
 * point `p`.
 */
vector4 vg_plane_with_point(vector normal, p) {
    vector n = normalize(normal);
    return vg_vec4(n, dot(n, p));
}

/**
 * Returns signed distance from `p` to `plane`
 */
float vg_plane_dist_to_point(vector4 plane; vector p) {
    return dot(vg_vec3(plane), p) - plane.w;
}

/**
 * Returns -1 if `p` is below plane, +1 if above or zero
 * if `p` is on the plane.
 */
int vg_plane_classify_point(vector4 plane; vector p) {
    return vg_sign(vg_plane_dist_to_point(plane, p));
}

/**
 * Returns closest point to `p` on `plane`.
 */
vector vg_plane_closest_point(vector4 plane; vector p) {
    return p - vg_vec3(plane) * vg_plane_dist_to_point(plane, p);
}

/**
 * Computes ray describing the infinite line of intersection of the
 * two given planes `a`, `b` and stores it in output variables `pos`
 * and `dir`. Returns 1 if planes intersect or 0 if they're parallel.
 */
int vg_plane_intersect_plane(vector4 a, b; export vector pos, dir) {
    vector na = vg_vec3(a);
    vector nb = vg_vec3(b);
    if (vg_eqdelta(na, nb, 1e-4)) {
        return 0;
    }
    float dn  = dot(na, nb);
    float det = 1.0 / (1.0 - dn * dn);
    float da  = (a.w - b.w * dn) * det;
    float db  = (b.w - a.w * dn) * det;
    pos       = na * da + nb * db;
    dir       = cross(na, nb);
    return 1;
}

/**
 * Computes intersection point of `plane` with ray defined by
 * `pos` and `dir` (the latter is assumed to be normalized).
 * Returns 1 if intersection occurred and writes result to `isec`,
 * else returns 0.
 */
int vg_plane_intersect_ray(vector4 plane; vector pos, dir; export vector isec) {
    vector n = vg_vec3(plane);
    float d = dot(n, dir);
    if (d < 1e-6) {
        isec = pos + dir * dot(n, n * plane.w - pos) / d;
        return 1;
    }
    return 0;
}

#endif
