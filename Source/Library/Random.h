/*
  ==============================================================================

    Random.h
    Created: 6 Dec 2023 9:15:34pm
    Author:  NeZvers

  ==============================================================================
*/

#pragma once
#include <stdint.h>

// https://stackoverflow.com/questions/11498304/generate-random-numbers-in-c-at-compile-time
template<uint32_t S, uint32_t A = 16807UL, uint32_t C = 0UL, uint32_t M = (1UL << 31) - 1>
struct LinearGenerator {
    static const uint32_t state = ((uint64_t)S * A + C) % M;
    static const uint32_t value = state;
    typedef LinearGenerator<state> next;
    struct Split { // Leapfrog
        typedef LinearGenerator< state, A* A, 0, M> Gen1;
        typedef LinearGenerator<next::state, A* A, 0, M> Gen2;
    };
};