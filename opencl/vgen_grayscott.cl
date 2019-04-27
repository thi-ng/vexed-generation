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

#include <vgen_laplacian.h>

kernel void vg_gray_scott2d(
    float timeinc,
    // volume params
    int stridex,
    int stridey,
    int stridez,
    int strideoffset,
    int resx,
    int resy,
    int _resz,
    // diffusion buffers
    global float *A,
    global float *B,
    // RD config
    float  diffa,
    float  diffb,
    float  feed,
    float  kill,
    float  feed2,
    float  kill2,
    float  speed) {

    int gidx = get_global_id(0);
    int gidy = get_global_id(1);
    int i = strideoffset + stridex * gidx + stridey * gidy;

    if (i >= stridex * stridey * stridez) {
        return;
    }

    float rate = A[i] * B[i] * B[i];

    float t = (float)gidx / resx;
    float f = feed + (feed2 - feed) * t;
    float k = kill + (kill2 - kill) * t;

    float feeda = f * (1.0 - A[i]);
    float la = vg_laplacian_2d(A, stridex, stridey, strideoffset, resx, resy, false);
    A[i] += (diffa * la - rate + feeda) * timeinc * speed;

    float killb = (k + f) * B[i];
    float lb = vg_laplacian_2d(B, stridex, stridey, strideoffset, resx, resy, false);
    B[i] += (diffb * lb + rate - killb) * timeinc * speed;
}
