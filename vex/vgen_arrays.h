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

#ifndef __vgen_arrays_h__
#define __vgen_arrays_h__

#define INTO(TYPE)                             \
    TYPE[] vg_into(TYPE a[]; const TYPE b[]) { \
        int na = len(a);                       \
        int nb = len(b);                       \
        resize(a, na + nb);                    \
        for (int i = 0; i < nb; i++) {         \
            a[na + i] = b[i];                  \
        }                                      \
        return a;                              \
    }

INTO(int)
INTO(float)
INTO(string)
INTO(vector2)
INTO(vector)
INTO(vector4)
INTO(matrix2)
INTO(matrix3)
INTO(matrix)
#undef INTO

#endif
