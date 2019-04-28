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

#ifndef __vgen_osc_h__
#define __vgen_osc_h__

#include <math.h>
#include <vgen_math.h>

#define OSC(fn)                         \
    float fn(float phase, freq, amp) {  \
        return fn(phase, freq, amp, 0); \
    }                                   \
    float fn(float phase, freq) {       \
        return fn(phase, freq, 1, 0);   \
    }

#define OSCX(fn, x)                         \
    float fn(float phase, freq, amp, dc) {  \
        return fn(phase, freq, amp, dc, x); \
    }                                       \
    float fn(float phase, freq, amp) {      \
        return fn(phase, freq, amp, 0, x);  \
    }                                       \
    float fn(float phase, freq) {           \
        return fn(phase, freq, 1, 0, x);    \
    }

/**
 * Sine oscillator
 */
float vg_osc_sin(float phase, freq, amp, dc) {
    return dc + amp * sin(phase * freq * TAU);
}

OSC(vg_osc_sin)

/**
 * Cosine oscillator
 */
float vg_osc_cos(float phase, freq, amp, dc) {
    return dc + amp * cos(phase * freq * TAU);
}

OSC(vg_osc_cos)

/**
 * Triangle oscillator
 */
float vg_osc_tri(float phase, freq, amp, dc) {
    return dc + amp * (abs((phase * freq * 4) % 4 - 2) - 1);
}

OSC(vg_osc_tri)

/**
 * Concave curved triangle oscillator
 */
float vg_osc_tri_concave(float phase, freq, amp, dc) {
    return dc + amp * (8 * pow(frac(phase * freq) - 0.5, 2) - 1);
}

OSC(vg_osc_tri_concave)

/**
 * Square oscillator (with configurable duty cycle)
 */
float vg_osc_square(float phase, freq, amp, dc, duty) {
    return dc + amp * (((phase * freq) % 1) < duty ? 1 : -1);
}

OSCX(vg_osc_square, 0.5)

/**
 * Sawtooth oscillator
 */
float vg_osc_saw(float phase, freq, amp, dc) {
    return dc + amp * (((phase * freq) % 1) * 2 - 1);
}

OSC(vg_osc_saw)

/**
 * Interpolated Sine-Sawtooth oscillator
 */
float vg_osc_sinsaw(float phase, freq, amp, dc, t) {
    return vg_mix(vg_osc_sin(phase, freq, amp, dc),
                  vg_osc_saw(phase, freq, amp, dc), t);
}

OSCX(vg_osc_sinsaw, 0.5)

/**
 * Interpolated Sine-Square oscillator
 */
float vg_osc_sinsquare(float phase, freq, amp, dc, t) {
    return vg_mix(vg_osc_sin(phase, freq, amp, dc),
                  vg_osc_square(phase, freq, amp, dc), t);
}

OSCX(vg_osc_sinsquare, 0.5)

/**
 * Interpolated Sine-Triangle oscillator
 */
float vg_osc_sintri(float phase, freq, amp, dc, t) {
    return vg_mix(vg_osc_sin(phase, freq, amp, dc),
                  vg_osc_tri(phase, freq, amp, dc), t);
}

OSCX(vg_osc_sintri, 0.5)

/**
 * Interpolated Saw-Square oscillator
 */
float vg_osc_sawsquare(float phase, freq, amp, dc, t) {
    return vg_mix(vg_osc_saw(phase, freq, amp, dc),
                  vg_osc_square(phase, freq, amp, dc), t);
}

OSCX(vg_osc_sawsquare, 0.5)

/**
 * Interpolated Saw-Triangle oscillator
 */
float vg_osc_sawtri(float phase, freq, amp, dc, t) {
    return vg_mix(vg_osc_saw(phase, freq, amp, dc),
                  vg_osc_tri(phase, freq, amp, dc), t);
}

OSCX(vg_osc_sawtri, 0.5)

/**
 * Interpolated Square-Triangle oscillator
 */
float vg_osc_squaretri(float phase, freq, amp, dc, t) {
    return vg_mix(vg_osc_square(phase, freq, amp, dc),
                  vg_osc_tri(phase, freq, amp, dc), t);
}

OSCX(vg_osc_squaretri, 0.5)

/**
 * Returns float array of `n` samples of given ramp param
 */
float[] vg_sample_ramp(string op_path; int n) {
    float res[];
    resize(res, n);
    for (int nn = n - 1, i = nn; i >= 0; i--) {
        res[i] = chramp(op_path, float(i) / nn);
    }
    return res;
}

/**
 * Wavetable oscillator (array based)
 * Use: vg_sample_ramp() to convert ramp into wave table
 */
float vg_osc_wavetable(float table[]; float phase, freq, amp, dc) {
    int n = len(table);
    int i = int(phase * freq * n) % n;
    return dc + amp * table[i < 0 ? i + n : i];
}

/**
 * Invokes oscillator by ID. Only single waveform versions are supported.
 * E.g. `vg_osc_by_id("sin", ...)` => `vg_osc_sin(...)`
 */
float vg_osc_by_id(string id; float phase, freq, amp, dc) {
    if (id == "sin") {
        return vg_osc_sin(phase, freq, amp, dc);
    }
    if (id == "saw") {
        return vg_osc_saw(phase, freq, amp, dc);
    }
    if (id == "square") {
        return vg_osc_square(phase, freq, amp, dc);
    }
    if (id == "tri") {
        return vg_osc_tri(phase, freq, amp, dc);
    }
    if (id == "tri_concave") {
        return vg_osc_tri_concave(phase, freq, amp, dc);
    }
    return 0;
}

#undef OSC
#undef OSCX

#endif
