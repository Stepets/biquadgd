#ifndef SOUND_SYNTH_H
#define SOUND_SYNTH_H

#include <Godot.hpp>
#include <Reference.hpp>

// based on:
// Simple implementation of Biquad filters -- Tom St Denis
// https://www.musicdsp.org/en/latest/Filters/64-biquad-c-code.html
//
// with fixes from:
// https://webaudio.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html

namespace godot {

class Filter: public Reference {
    GODOT_CLASS ( Filter, Reference )

private:
    float a0 = 0, a1 = 0, a2 = 0, a3 = 0, a4 = 0;
    float x1 = 0, x2 = 0, y1 = 0, y2 = 0;

    const float LN2 = 0.69314718055994530942;

public:

    enum Type {
        LPF, /* low pass filter */
        HPF, /* High pass filter */
        BPF, /* band pass filter */
        NOTCH, /* Notch Filter */
        PEQ, /* Peaking band EQ filter */
        LSH, /* Low shelf filter */
        HSH /* High shelf filter */
    };

    static void _register_methods();

    Filter();
    ~Filter();

    void _init ();

    void update ( int type, float freq, float srate, float db_gain, float Q_BW_S = 1 );
    float apply ( float sample );
};

}

#endif

