/*
  ==============================================================================

    Macros.h
    Created: 26 Jul 2024 1:56:55pm
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

#if defined __APPLE__
#define _SIMULATOR_
#endif

#define xstr(s) str(s)
#define str(s) #s

#define _PARAM(p) p, xstr(p)
