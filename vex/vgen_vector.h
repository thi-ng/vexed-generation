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

#ifndef __vgen_vector_h__
#define __vgen_vector_h__

vector vg_vec3(vector2 xy; float z) {
    return set(xy.x, xy.y, z);
}

vector vg_vec3(vector4 v) {
    return set(v.x, v.y, v.z);
}

vector4 vg_vec4(vector2 xy, zw) {
    return set(xy.x, xy.y, zw.x, zw.y);
}

vector4 vg_vec4(vector xyz; float w) {
    return set(xyz.x, xyz.y, xyz.z, w);
}

#endif
