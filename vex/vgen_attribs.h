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

#ifndef __vgen_attribs_h__
#define __vgen_attribs_h__

#define ATTRIBS(TYPE) \
TYPE[] vg_point_attribs_##TYPE(int geo; string attr; const int pts[]) { \
    int n = len(pts);                                                   \
    TYPE res[];                                                         \
    resize(res, n);                                                     \
    for (int i = n - 1; i >= 0; i--) {                                  \
        res[i] = point(geo, attr, pts[i]);                              \
    }                                                                   \
    return res;                                                         \
}

ATTRIBS(int)
ATTRIBS(float)
ATTRIBS(vector2)
ATTRIBS(vector)
ATTRIBS(vector4)
#undef ATTRIBS

vector[] vg_point_positions(int geo; const int pts[]) {
    return vg_point_attribs_vector(geo, "P", pts);
}

vector[] vg_point_positions(int geo, prim) {
    return vg_point_attribs_vector(geo, "P", primpoints(geo, prim));
}

#endif
