/*
 * Softmax Accelerator - Design 1
 * FPGAs Project 2
 */

#include "softmax.h"
#include <cmath>

extern "C" {

/**
 * Softmax function: out[i] = exp(in[i]) / sum(exp(in[j]))
 * 
 * @param in  Input array of 100 float elements
 * @param out Output array of 100 float elements
 */
void softmax(float *in, float *out) {
#pragma HLS INTERFACE m_axi port=in  offset=slave bundle=gmem0 depth=100
#pragma HLS INTERFACE m_axi port=out offset=slave bundle=gmem1 depth=100
#pragma HLS INTERFACE s_axilite port=return

    float sum = 0.0f;
    // Step 1: Compute sum of exponentials
    for (int i = 0; i < kSize; i++) {
        sum += expf(in[i]);
    }
    // Step 2: Compute softmax for each element
    for (int i = 0; i < kSize; i++) {
        out[i] = expf(in[i]) / sum;
    }
}
}
