#include "filter.h"
#include <cmath>

using namespace godot;

void godot::Filter::_register_methods() {
    register_method ( "update", &Filter::update );
    register_method ( "apply", &Filter::apply );
}


godot::Filter::Filter() {
}

godot::Filter::~Filter() {
}

void godot::Filter::_init () {
}

float godot::Filter::apply ( float sample ) {
    float result;

    result = a0 * sample + a1 * x1 + a2 * x2 - a3 * y1 - a4 * y2;

    x2 = x1;
    x1 = sample;

    y2 = y1;
    y1 = result;

    return result;
}



void godot::Filter::update ( int type, float freq, float srate, float db_gain, float Q_BW_S ) {

    float A, omega, sn, cs, alpha, beta;

    float a0, a1, a2, b0, b1, b2;

    A = ::pow ( 10, db_gain /40 );
    omega = 2 * Math_PI * freq /srate;
    sn = ::sin ( omega );
    cs = ::cos ( omega );
    switch ( type ) {
    case PEQ:
    case HPF:
    case LPF:
        alpha = sn / 2 / Q_BW_S;
        break;
    case BPF:
    case NOTCH:
        alpha = sn * ::sinh ( LN2 /2 * Q_BW_S * omega /sn );
        break;
    case LSH:
    case HSH:
        alpha = sn/2 * ::sqrt ( ( A + 1/A ) * ( 1.0 / Q_BW_S - 1 ) + 2 );
        break;
    }
    beta = 2 * ::sqrt ( A );

    switch ( type ) {
    case LPF:
        b0 = ( 1 - cs ) /2;
        b1 = 1 - cs;
        b2 = ( 1 - cs ) /2;
        a0 = 1 + alpha;
        a1 = -2 * cs;
        a2 = 1 - alpha;
        break;
    case HPF:
        b0 = ( 1 + cs ) /2;
        b1 = - ( 1 + cs );
        b2 = ( 1 + cs ) /2;
        a0 = 1 + alpha;
        a1 = -2 * cs;
        a2 = 1 - alpha;
        break;
    case BPF:
        b0 = alpha;
        b1 = 0;
        b2 = -alpha;
        a0 = 1 + alpha;
        a1 = -2 * cs;
        a2 = 1 - alpha;
        break;
    case NOTCH:
        b0 = 1;
        b1 = -2 * cs;
        b2 = 1;
        a0 = 1 + alpha;
        a1 = -2 * cs;
        a2 = 1 - alpha;
        break;
    case PEQ:
        b0 = 1 + ( alpha * A );
        b1 = -2 * cs;
        b2 = 1 - ( alpha * A );
        a0 = 1 + ( alpha /A );
        a1 = -2 * cs;
        a2 = 1 - ( alpha /A );
        break;
    case LSH:
        b0 = A * ( ( A + 1 ) - ( A - 1 ) * cs + beta * alpha );
        b1 = 2 * A * ( ( A - 1 ) - ( A + 1 ) * cs );
        b2 = A * ( ( A + 1 ) - ( A - 1 ) * cs - beta * alpha );
        a0 = ( A + 1 ) + ( A - 1 ) * cs + beta * alpha;
        a1 = -2 * ( ( A - 1 ) + ( A + 1 ) * cs );
        a2 = ( A + 1 ) + ( A - 1 ) * cs - beta * alpha;
        break;
    case HSH:
        b0 = A * ( ( A + 1 ) + ( A - 1 ) * cs + beta * alpha );
        b1 = -2 * A * ( ( A - 1 ) + ( A + 1 ) * cs );
        b2 = A * ( ( A + 1 ) + ( A - 1 ) * cs - beta * alpha );
        a0 = ( A + 1 ) - ( A - 1 ) * cs + beta * alpha;
        a1 = 2 * ( ( A - 1 ) - ( A + 1 ) * cs );
        a2 = ( A + 1 ) - ( A - 1 ) * cs - beta * alpha;
        break;
    }

    this->a0 = b0 /a0;
    this->a1 = b1 /a0;
    this->a2 = b2 /a0;
    this->a3 = a1 /a0;
    this->a4 = a2 /a0;
}


