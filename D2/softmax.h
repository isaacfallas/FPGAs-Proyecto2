/*
 * Softmax Accelerator - Design 2
 * FPGAs Project 2
 */

#ifndef __SOFTMAX_H__
#define __SOFTMAX_H__

#include <stdint.h>
#include <hls_stream.h>

// Number of elements (fixed at 100 as per requirements)
static constexpr int kSize = 100;

// Stream type for inter-module communication
using DataStream = hls::stream<float>;

extern "C" {
void softmax(float *in, float *out);
}

#endif // __SOFTMAX_H__
