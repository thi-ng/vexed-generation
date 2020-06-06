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

#ifndef __vgen_tessel_h__
#define __vgen_tessel_h__

#include <vgen_arrays.h>
#include <vgen_centroid.h>

int vg_add_triangle(int geo, a, b, c) {
    int prim = addprim(0, "poly");
    addvertex(geo, prim, a);
    addvertex(geo, prim, b);
    addvertex(geo, prim, c);
    return prim;
}

int vg_add_quad(int geo, a, b, c, d) {
    int prim = addprim(0, "poly");
    addvertex(geo, prim, a);
    addvertex(geo, prim, b);
    addvertex(geo, prim, c);
    addvertex(geo, prim, d);
    return prim;
}

int[] vg_tessellate_first(int geo; const int pts[]) {
    int q = pts[0];
    int n = len(pts);
    int prims[];
    resize(prims, n - 2);
    for (int i = 1; i < n - 1; i++) {
        prims[i] = vg_add_triangle(geo, q, pts[i], pts[i + 1]);
    }
    return prims;
}

int[] vg_tessellate_first(int geo, prim) {
    int prims[] = vg_tessellate_first(geo, primpoints(geo, prim));
    removeprim(geo, prim, 0);
    return prims;
}

int[] vg_tessellate_trifan(int geo; const int pts[]) {
    int c = addpoint(geo, vg_centroid(geo, pts));
    int n = len(pts);
    int prims[];
    resize(prims, n);
    for (int i = n - 1, j = 0; j < n; i = j, j++) {
        prims[j] = vg_add_triangle(geo, c, pts[i], pts[j]);
    }
    return prims;
}

int[] vg_tessellate_trifan(int geo, prim) {
    int prims[] = vg_tessellate_trifan(geo, primpoints(geo, prim));
    removeprim(geo, prim, 0);
    return prims;
}

int[] vg_tessellate_quadfan(int geo; const int pts[]) {
    int mpts[] = vg_add_edge_centroids(geo, pts);
    int c      = addpoint(geo, vg_centroid(geo, pts));
    int n      = len(pts);
    int prims[];
    resize(prims, n);
    for (int i = n - 1, j = 0; j < n; i = j, j++) {
        prims[j] = vg_add_quad(geo, c, mpts[i], pts[j], mpts[j]);
    }
    return prims;
}

int[] vg_tessellate_quadfan(int geo, prim) {
    int prims[] = vg_tessellate_quadfan(geo, primpoints(geo, prim));
    removeprim(geo, prim, 0);
    return prims;
}

int[] vg_tessellate_quadfan_uv(int geo; const int pts[]) {
    int mpts[]  = vg_add_edge_centroids_uv(geo, pts);
    int c       = addpoint(geo, vg_centroid(geo, pts));
    vector2 cuv = vg_centroid(vg_point_attribs_vector2(geo, "uv", pts));
    setpointattrib(geo, "uv", c, cuv);
    int n = len(pts);
    int prims[];
    resize(prims, n);
    for (int i = n - 1, j = 0; j < n; i = j, j++) {
        prims[j] = vg_add_quad(geo, c, mpts[i], pts[j], mpts[j]);
    }
    return prims;
}

int[] vg_tessellate_quadfan_uv(int geo, prim) {
    int prims[] = vg_tessellate_quadfan_uv(geo, primpoints(geo, prim));
    removeprim(geo, prim, 0);
    return prims;
}

int[] vg_tessellate_mid(int geo; const int pts[]) {
    int mpts[] = vg_add_edge_centroids(geo, pts);
    int n      = len(pts);
    int prims[];
    resize(prims, n + 1);
    for (int i = n - 1, j = 0; j < n; i = j, j++) {
        prims[j] = vg_add_triangle(geo, mpts[i], pts[j], mpts[j]);
    }
    int p = addprim(geo, "poly");
    for (int i = 0; i < n; i++) {
        addvertex(geo, p, mpts[i]);
    }
    prims[n - 1] = p;
    return prims;
}

int[] vg_tessellate_mid(int geo, prim) {
    int prims[] = vg_tessellate_mid(geo, primpoints(geo, prim));
    removeprim(geo, prim, 0);
    return prims;
}

int[] vg_quad_strip(int geo; const int row1[]; const int row2[];
                    int num, closed) {
    int res[], i, j;
    resize(res, closed ? num : num - 1);
    if (closed) {
        i = 0;
        j = num - 1;
    } else {
        i = 1;
        j = 0;
    }
    for (int k = 0; i < num; j = i, i++, k++) {
        res[k] = vg_add_quad(geo, row1[j], row1[i], row2[i], row2[j]);
    }
    return res;
}

#endif