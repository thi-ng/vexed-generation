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
//  (c) 2016 - 2020 Karsten Schmidt // ASL 2.0 licensed

#ifndef __vgen_attribs_h__
#define __vgen_attribs_h__

#define VG_ATTRIBS(TYPE)                                                    \
    TYPE[] vg_point_attribs_##TYPE(int geo; string attr; const int pts[]) { \
        int n = len(pts);                                                   \
        TYPE res[];                                                         \
        resize(res, n);                                                     \
        for (int i = 0; i < n; i++) {                                       \
            res[i] = point(geo, attr, pts[i]);                              \
        }                                                                   \
        return res;                                                         \
    }                                                                       \
    TYPE[] vg_point_attribs_##TYPE(int geo; string attr; const int prim) {  \
        return vg_point_attribs_##TYPE(geo, attr, primpoints(geo, prim));   \
    }

VG_ATTRIBS(int)
VG_ATTRIBS(float)
VG_ATTRIBS(vector2)
VG_ATTRIBS(vector)
VG_ATTRIBS(vector4)
#undef VG_ATTRIBS

vector[] vg_point_positions(int geo; const int pts[]) {
    return vg_point_attribs_vector(geo, "P", pts);
}

vector[] vg_point_positions(int geo, prim) {
    return vg_point_attribs_vector(geo, "P", primpoints(geo, prim));
}

#define MAX_FLOAT 1e38
#define VG_ATTRIB_RANGE(ATYPE, ANUM)                                        \
    float[] vg_##ATYPE##attrib_range_float(int geo; string id) {            \
        float res[] = {MAX_FLOAT, -MAX_FLOAT};                              \
        for (int i = ANUM(geo); --i >= 0;) {                                \
            float v = ATYPE(geo, id, i);                                    \
            res[0]  = min(res[0], v);                                       \
            res[1]  = max(res[1], v);                                       \
        }                                                                   \
        return res;                                                         \
    }                                                                       \
    vector2[] vg_##ATYPE##attrib_range_vec2(int geo; string id) {           \
        vector2 res[] = {{MAX_FLOAT, MAX_FLOAT}, {-MAX_FLOAT, -MAX_FLOAT}}; \
        for (int i = ANUM(geo); --i >= 0;) {                                \
            vector2 v = ATYPE(geo, id, i);                                  \
            res[0]    = min(res[0], v);                                     \
            res[1]    = max(res[1], v);                                     \
        }                                                                   \
        return res;                                                         \
    }                                                                       \
    vector[] vg_##ATYPE##attrib_range_vec3(int geo; string id) {            \
        vector res[] = {{MAX_FLOAT, MAX_FLOAT, MAX_FLOAT},                  \
                        {-MAX_FLOAT, -MAX_FLOAT, -MAX_FLOAT}};              \
        for (int i = ANUM(geo); --i >= 0;) {                                \
            vector v = ATYPE(geo, id, i);                                   \
            res[0]   = min(res[0], v);                                      \
            res[1]   = max(res[1], v);                                      \
        }                                                                   \
        return res;                                                         \
    }                                                                       \
    vector[] vg_##ATYPE##attrib_range_vec4(int geo; string id) {            \
        vector res[] = {{MAX_FLOAT, MAX_FLOAT, MAX_FLOAT, MAX_FLOAT},       \
                        {-MAX_FLOAT, -MAX_FLOAT, -MAX_FLOAT, -MAX_FLOAT}};  \
        for (int i = ANUM(geo); --i >= 0;) {                                \
            vector v = ATYPE(geo, id, i);                                   \
            res[0]   = min(res[0], v);                                      \
            res[1]   = max(res[1], v);                                      \
        }                                                                   \
        return res;                                                         \
    }

VG_ATTRIB_RANGE(point, npoints)
VG_ATTRIB_RANGE(prim, nprimitives)
VG_ATTRIB_RANGE(vertex, nvertices)

#undef MAX_FLOAT
#undef VG_ATTRIB_RANGE

#endif
