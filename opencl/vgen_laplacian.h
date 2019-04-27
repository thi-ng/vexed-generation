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

#ifndef __vgen_laplacian_h__
#define __vgen_laplacian_h__

float vg_laplacian_2d(global float* f, int sx, int sy, int so, int resx, int resy, bool wrap) {
    int x = get_global_id(0);
    int y = get_global_id(1);

    float xo[] = { -1, 0, +1, -1, 0, +1, -1, 0, +1 };
    float yo[] = { +1, +1, +1, 0, 0, 0, -1, -1, -1 };
    float w[]  = { 0.05, 0.20, 0.05, 0.20, -1.0, 0.2, 0.05, 0.2, 0.05 };
    float out = 0;

    resx -=1;
    resy -=1;

    for(int i = 0; i < 9; i++) {
        int x1 = x + xo[i];
        int y1 = y + yo[i];
        if (wrap) {
            if(x1 < 0) x1=resx;
            if(x1 > resx) x1=0;
            if(y1 < 0) y1=resy;
            if(y1 > resy) y1=0;
        }
        else  {
            if(x1 < 0) x1=0;
            if(x1 > resx) x1 = resx;
            if(y1 < 0) y1 = 0;
            if(y1 > resy) y1 = resy;
        }
        float v = f[so + sx * x1 + sy * y1] * w[i];
        out += v;
    }
    return out;
}

#endif
