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

#ifndef __vgen_vector_h__
#define __vgen_vector_h__

vector2 vg_vec2(float x, y) {
    return set(x, y);
}

vector2 vg_vec2(vector3 v) {
    return set(v.x, v.y);
}

vector2 vg_vec2(vector4 v) {
    return set(v.x, v.y);
}

vector vg_vec3(vector2 v) {
    return set(v.x, v.y, 0);
}

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

vector2 vg_swizzle(vector2 v; int x, y) {
    return set(v[x], v[y]);
}

vector vg_swizzle(vector2 v; int x, y, z) {
    return set(v[x], v[y], v[z]);
}

vector4 vg_swizzle(vector2 v; int x, y, z, w) {
    return set(v[x], v[y], v[z], v[w]);
}

vector2 vg_swizzle(vector v; int x, y) {
    return set(v[x], v[y]);
}

vector vg_swizzle(vector v; int x, y, z) {
    return set(v[x], v[y], v[z]);
}

vector4 vg_swizzle(vector v; int x, y, z, w) {
    return set(v[x], v[y], v[z], v[w]);
}

vector2 vg_swizzle(vector4 v; int x, y) {
    return set(v[x], v[y]);
}

vector vg_swizzle(vector4 v; int x, y, z) {
    return set(v[x], v[y], v[z]);
}

vector4 vg_swizzle(vector4 v; int x, y, z, w) {
    return set(v[x], v[y], v[z], v[w]);
}

#endif
