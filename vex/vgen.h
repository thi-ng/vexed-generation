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

#define VG_STR(x) #x
#define VG_STR2(x) VG_STR(x)

// semantic versioning

#define VG_VERSION_MAJOR 1
#define VG_VERSION_MINOR 7
#define VG_VERSION_PATCH 0
#define VG_VERSION                                                        \
    (VG_STR2(VG_VERSION_MAJOR) "." VG_STR2(VG_VERSION_MINOR) "." VG_STR2( \
        VG_VERSION_PATCH))

void vg_min_version(string version) {
    if (VG_VERSION < version) {
        error("Please upgrade thi.ng/vgen to v%s or newer", version);
    }
}

// submodules

#include <vgen_2d.h>
#include <vgen_arrays.h>
#include <vgen_attribs.h>
#include <vgen_bounds.h>
#include <vgen_centroid.h>
#include <vgen_disjointset.h>
#include <vgen_distance.h>
#include <vgen_marchingsquares.h>
#include <vgen_math.h>
#include <vgen_osc.h>
#include <vgen_plane.h>
#include <vgen_ptf.h>
#include <vgen_tessel.h>
#include <vgen_vector.h>
