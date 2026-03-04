/*
 * Softmax Accelerator - Design 1
 * FPGAs Project 2
 */

#ifndef __SOFTMAX_H__
#define __SOFTMAX_H__

#include <stdint.h>
#include <cmath>

// Number of elements
static constexpr int kSize = 100;

extern "C" {
void softmax(float *in, float *out);
}

#endif // __SOFTMAX_H__
