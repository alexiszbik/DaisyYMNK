/*
  ==============================================================================

    InlineMath.h
    Created: 11 May 2025 2:39:08pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

#include <math.h>

static inline float ftom(float f)
{
    return (f > 0 ? 17.3123405046f * log(.12231220585f * f) : -1500);
}

inline float fast_mtof(float m)
{
    constexpr float inv12 = 0.0833333333333f;
    constexpr float a440  = 440.f;
    return a440 * exp2f((m - 69.f) * inv12);
}

