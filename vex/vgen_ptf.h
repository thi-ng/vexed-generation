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

#ifndef __vgen_ptf_h__
#define __vgen_ptf_h__

#include <vgen_attribs.h>
#include <vgen_tessel.h>

/**
 * Parallel Transport Frames implementation based on
 * https://github.com/thi-ng/geom/blob/develop/src/types/ptf.org
 *
 * ```
 * vgPTF ptf;
 * ptf->init(0, @primnum, 0);
 * ptf->sweep_tube(vg_point_positions(1, 0), "rscale", chf("twist"), chf("twistPhase"), 1);
 * ```
 */
struct vgPTF {
    int geo;
    int closed;
    int maxPoints;
    vector points[];
    vector tangents[];
    vector normals[];
    vector binormals[];

    void init(int opinput, primnum, close) {
        geo      = opinput;
        closed   = close;
        points   = vg_point_positions(geo, primnum);
        int nump = len(points);
        if (nump > 1) {
            resize(tangents, nump);
            resize(normals, nump);
            resize(binormals, nump);
            maxPoints    = nump - 1;
            vector t     = normalize(points[1] - points[0]);
            vector ta    = abs(t);
            vector bn    = {0, 0, 0};
            int j        = ta.x < ta.y ? 0 : 1;
            j            = ta.z < ta[j] ? 2 : j;
            bn[j]        = 1;
            vector n     = cross(t, normalize(cross(t, bn)));
            tangents[0]  = t;
            normals[0]   = n;
            binormals[0] = cross(t, n);

            for (int i = 1; i < nump; i++) {
                vector s = i < nump - 1
                               ? normalize(points[i + 1] - points[i])
                               : closed ? normalize(points[0] - points[i]) : t;
                vector a = cross(t, s);
                if (abs(length2(a)) > 1e-6) {
                    float theta = acos(clamp(dot(t, s), 0, 1));
                    matrix m    = ident();
                    rotate(m, theta, normalize(a));
                    n = m * n;
                }
                tangents[i]  = s;
                normals[i]   = n;
                binormals[i] = cross(s, n);
                t            = s;
            }
        }
    }

    int add_sweep_point(int idx; const vector profile; string scaleRamp;
                        float twistAmp, twistPhase) {
        float t   = float(idx) / maxPoints;
        vector p  = profile * chramp(scaleRamp, t);
        matrix3 m = ident();
        rotate(m, twistPhase + twistAmp * t, {0.0, 0.0, 1.0});
        p         = m * p;
        vector q  = points[idx];
        vector n  = normals[idx];
        vector bn = binormals[idx];
        vector cp =
            set(p.x * n.x + p.y * bn.x + q.x, p.x * n.y + p.y * bn.y + q.y,
                p.x * n.z + p.y * bn.z + q.z);
        return addpoint(0, cp);
    }

    void sweep_lines(const vector profile[]; string scaleRamp;
                     float twistAmp, twistPhase) {
        int numProfile = len(profile);
        for (int i = 0; i < numProfile; i++) {
            int prim = addprim(geo, "polyline");
            int first;
            for (int j = 0; j <= maxPoints; j++) {
                int p = this->add_sweep_point(j, profile[i], scaleRamp,
                                              twistAmp, twistPhase);
                addvertex(geo, prim, p);
                if (j == 0) {
                    first = p;
                }
            }
            if (closed) {
                addvertex(geo, prim, first);
            }
        }
    }

    void sweep_tube(const vector profile[]; string scaleRamp;
                    float twistAmp, twistPhase;
                    int close) {
        int numProfile = len(profile);
        int prev[], first[];
        for (int j = 0; j <= maxPoints; j++) {
            int curr[];
            resize(curr, numProfile);
            for (int i = 0; i < numProfile; i++) {
                curr[i] = this->add_sweep_point(j, profile[i], scaleRamp,
                                                twistAmp, twistPhase);
            }
            if (j > 0) {
                vg_quad_strip(geo, prev, curr, numProfile, close);
            } else {
                first = curr;
            }
            prev = curr;
        }
        if (closed) {
            vg_quad_strip(geo, prev, first, numProfile, close);
        }
    }

    void sweep_discs(const vector profile[]; string scaleRamp;
                     float twistAmp, twistPhase;
                     int close) {
        int numProfile = len(profile);
        for (int j = 0; j <= maxPoints; j++) {
            int prev, first;
            int c = addpoint(geo, points[j]);
            for (int i = 0; i < numProfile; i++) {
                int curr = this->add_sweep_point(j, profile[i], scaleRamp,
                                                 twistAmp, twistPhase);
                if (i > 0) {
                    vg_add_triangle(geo, c, prev, curr);
                } else {
                    first = curr;
                }
                prev = curr;
            }
            if (close) {
                vg_add_triangle(geo, c, prev, first);
            }
        }
    }
}

#endif
