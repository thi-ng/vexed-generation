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

#ifndef __vgen_marchsq_h__
#define __vgen_marchsq_h__

struct vgGridIter {
    int w;
    int h;
    int x;
    int y;
    int x0;
    int more;

    void init(int _x, _y, _w, _h) {
        x = x0 = _x;
        y = _y;
        w = _w;
        h = _h;
        more = 1;
    }

    int has_next() {
        return more;
    }

    /**
     * Returns next grid position as [x, y] tuple.
     */
    int[] next() {
        int out[] = {-1, -1};
        if (more) {
            out[0] = x;
            out[1] = y;
            x++;
            if (x == w) {
                x = x0;
                y++;
                more = y < h;
            }
        }
        return out;
    }
}

/**
 * 2D Marching squares implementation based on:
 * https://github.com/thi-ng/ndarray/blob/master/src/contours.org
 *
 * It's the user's responsibility to ensure the border
 * cell values of the grid are set to 0 prior to
 * finding contours.
 *
 * float data[];
 * resize(data, cols * rows);
 * for(int i=npoints(0); i>=0; i--) {
 *     data[i] = vector(point(0,"P", i)).y;
 * }
 *
 * vgMSQ msq;
 * msq->init(data, cols, rows);
 * msq->find_contours(0, 0.1, ident());
 */
struct vgMSQ {
    float src[];
    int w;
    int h;
    float iso;

    void init(float _src[]; int _w, _h) {
        src = _src;
        w = _w;
        h = _h;
    }

    int[] encode_crossings() {
        int out[];
        resize(out, len(src));
        int w1 = w - 1;
        int h1 = h - 1;
        for (int y = 0; y < h1; y++) {
            int i = y * w;
            for (int x = 0; x < w1; x++) {
                int idx = i + x;
                out[idx] = (
                    (src[idx] < iso ? 8 : 0) +
                    (src[idx + 1] < iso ? 4 : 0) +
                    (src[idx + 1 + w] < iso ? 2 : 0) +
                    (src[idx + w] < iso ? 1 : 0)
                );
            }
        }
        return out;
    }

    float mean_cell_value(int x, y) {
        int idx = y * w + x;
        return (src[idx] + src[idx + 1] + src[idx + w] + src[idx + w + 1]) * 0.25;
    }

    float find_iso(int x1, y1, x2, y2) {
        float a = src[y1 * w + x1];
        float b = src[y2 * w + x2];
        return a == b ? 0 : (a - iso) / (a - b);
    }

    vector contour_vertex(int x, y, to) {
        if(to == 0) {
            return set(x + this->find_iso(x, y, x + 1, y), y, 0);
        } else if (to == 1) {
            return set(x + 1, y + this->find_iso(x + 1, y, x + 1, y + 1), 0);
        } else if (to == 2) {
            return set(x + this->find_iso(x, y + 1, x + 1, y + 1), y + 1, 0);
        } else {
            return set(x, y + this->find_iso(x, y, x, y + 1), 0);
        }
    }

    int[] find_contours(int geo; float _iso; matrix tx) {
        iso = _iso;
        int edgeIndex[] = { -1, -1, 2, 0, 1, 0, 1, 0, 0, 0, -1, -1, 0, 0, 0, 0, 3, 0, 2, 0, -1, -1, 1, 0, 3, 0, 2, 0, 3, 0, -1, -1 };
        int nextEdges[] = { 0, -1, 1, 0, 0, 1, -1, 0 };
        int s5[] = { 2, 4, 0, 1, 0, 13, 2, 7 };
        int s10[] = { 3, 2, 1, 8, 3, 11, 1, 14 };

        int coded[] = this->encode_crossings();
        int prims[];
        int curr = -1;
        int to = -1;
        int from;
        int idx, clear;
        int x, y;
        int p[];
        int hasP = false;
        vgGridIter cells;
        cells->init(1, 1, w - 1, h - 1);
        while (cells->has_next()) {
            from = to;
            p = hasP ? p : cells->next();
            hasP = true;
            x = p[0];
            y = p[1];
            int id = coded[y * w + x];
            if (id == 5) {
                idx = ((this->mean_cell_value(x, y) > iso ? 0 : 2) + (from == 3 ? 0 : 1)) * 2;
                to = s5[idx];
                clear = s5[idx + 1];
            } else if (id == 10) {
                idx = ((this->mean_cell_value(x, y) > iso) ? (from == 0 ? 0 : 1) : (from == 2 ? 2 : 3)) * 2;
                to = s10[idx];
                clear = s10[idx + 1];
            } else {
                id *= 2;
                to = edgeIndex[id];
                clear = edgeIndex[id + 1];
            }
            if (from == -1 && to != -1 && curr != -1) {
                push(prims, curr);
                curr = -1;
            }
            if (clear != -1) {
                coded[y * w + x] = clear;
            }
            if (to >= 0) {
                if (curr == -1) {
                    curr = addprim(geo, "poly");
                }
                addvertex(geo, curr, addpoint(geo, tx * this->contour_vertex(x, y, to)));
                p[0] += nextEdges[to * 2];
                p[1] += nextEdges[to * 2 + 1];
            } else {
               hasP = 0;
            }
        }
        if (curr != -1) {
            push(prims, curr);
        }
        return prims;
    }
}

#endif
