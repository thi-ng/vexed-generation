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

#ifndef __vgen_unionfind_h__
#define __vgen_unionfind_h__

struct vgDisjointSet {

    int roots[];
    int ranks[];
    int count;

    void init(int n) {
        roots = array();
        ranks = array();
        resize(roots, n);
        resize(ranks, n);
        count = n;
        for (int i = 0; i < n; i++) {
            roots[i] = i;
        }
    }

    /**
     * Returns canonical ID (tree root) for given `id`. Unless `id`
     * already is unified with some other ID, this will always return
     * `id` itself (since each node is initially its own root).
     *
     * @param id
     */
    int find(int id) {
        int i = id;
        while (i != roots[i]) {
            i = roots[i] = roots[roots[i]];
        }
        return i;
    }

    /**
     * Connects combines the trees of the given two node IDs and returns
     * the new resulting canonical tree root ID.
     *
     * @param a
     * @param b
     */
    int union(int a, b) {
        int rootA = this->find(a);
        int rootB = this->find(b);
        if (rootA == rootB) {
            return rootA;
        }
        count--;
        int ra = ranks[rootA];
        int rb = ranks[rootB];
        if (ra < rb) {
            return roots[rootA] = rootB;
        }
        ra == rb && ranks[rootA]++;
        return roots[rootB] = rootA;
    }

    /**
     * Returns true, if the given two nodes belong to the same tree /
     * subset.
     *
     * @param a
     * @param b
     */
    int unified(int a, b) {
        return this->find(a) == this->find(b);
    }
}

#endif
