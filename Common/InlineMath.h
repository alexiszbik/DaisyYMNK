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
