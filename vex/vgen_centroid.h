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

#ifndef __vgen_centroid_h__
#define __vgen_centroid_h__

#include <vgen_math.h>

vector2 vg_centroid(const vector2 pts[]) {
    int n     = len(pts);
    vector2 c = {0, 0};
    for (int i = 0; i < n; i++) {
        c += pts[i];
    }
    return c / n;
}

vector vg_centroid(const vector pts[]) {
    int n    = len(pts);
    vector c = {0, 0, 0};
    for (int i = 0; i < n; i++) {
        c += pts[i];
    }
    return c / n;
}

vector vg_centroid(int geo; const int pts[]) {
    int n    = len(pts);
    vector c = {0, 0, 0};
    for (int i = 0; i < n; i++) {
        c += point(geo, "P", pts[i]);
    }
    return c / n;
}

vector vg_prim_centroid(int geo, prim) {
    return vg_centroid(geo, primpoints(geo, prim));
}

int[] vg_add_edge_centroids(int geo; const int pts[]) {
    int res[];
    int n = len(pts);
    resize(res, n);
    for (int i = n - 1, j = 0; j < n; i = j, j++) {
        res[i] = addpoint(
            geo, vg_mix(point(geo, "P", pts[i]), point(geo, "P", pts[j])));
    }
    return res;
}

int[] vg_add_edge_centroids_uv(int geo; const int pts[]) {
    int res[];
    int n = len(pts);
    resize(res, n);
    for (int i = n - 1, j = 0; j < n; i = j, j++) {
        res[i] = addpoint(
            geo, vg_mix(point(geo, "P", pts[i]), point(geo, "P", pts[j])));
        vector2 uv = vg_mix(point(geo, "uv", pts[i]), point(geo, "uv", pts[j]));
        setpointattrib(geo, "uv", res[i], uv);
    }
    return res;
}

#endif
