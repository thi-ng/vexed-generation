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

#ifndef __vgen_distance_h__
#define __vgen_distance_h__

vector vg_closest_point_line(const vector a; const vector b; const vector p) {
    float len = length(b - a);
    if (len < EPS) {
        return a;
    }
    float t = dot(p - a, b - a) / (len * len);
    if (t < 0) {
        return a;
    } else if (t > 1) {
        return b;
    }
    return a + (b - a) * t;
}

/**
 * Computes closest point on given set of edges (pairs of point IDs).
 */
vector vg_closest_point_edges(int geo; const int edges[]; const vector p) {
    vector closest;
    float minD = 1e6;
    for (int i = len(edges) - 2; i >= 0; i -= 2) {
        vector q = vg_closest_point_line(point(geo, "P", edges[i]),
                                         point(geo, "P", edges[i + 1]), p);
        float d  = distance(p, q);
        if (d < minD) {
            minD    = d;
            closest = q;
        }
    }
    return closest;
}

float vg_dist_manhattan(vector2 a, b) {
    vector d = abs(a - b);
    return d.x + d.y;
}

float vg_dist_manhattan(vector a, b) {
    vector d = abs(a - b);
    return d.x + d.y + d.z;
}

float vg_dist_manhattan(vector4 a, b) {
    vector4 d = abs(a - b);
    return d.x + d.y + d.z + d.w;
}

float vg_dist_chebyshev(vector2 a, b) {
    vector d = abs(a - b);
    return max(d.x, d.y);
}

float vg_dist_chebyshev(vector a, b) {
    vector d = abs(a - b);
    return max(d.x, d.y, d.z);
}

float vg_dist_chebyshev(vector4 a, b) {
    vector4 d = abs(a - b);
    return max(d.x, d.y, d.z, d.w);
}

#endif
