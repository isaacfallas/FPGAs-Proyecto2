/*
 * Softmax Accelerator Testbench - Design 1
 * FPGAs Project 2
 */

#include "softmax.h"
#include <iostream>
#include <cmath>
#include <cstdlib>

// Compute reference softmax in software
void softmax_ref(float *in, float *out, int size) {
    float sum = 0.0f;
    for (int i = 0; i < size; i++) {
        sum += expf(in[i]);
    }
    for (int i = 0; i < size; i++) {
        out[i] = expf(in[i]) / sum;
    }
}

int main() {
    float in[kSize];
    float out[kSize];
    float ref[kSize];
    
    // Initialize input with some values
    std::cout << "Input values:" << std::endl;
    for (int i = 0; i < kSize; i++) {
        in[i] = (float)(i - 50) * 0.1f;  // Values from -5.0 to 4.9
        std::cout << in[i] << " ";
        if ((i + 1) % 10 == 0) std::cout << std::endl;
    }
    
    // Compute reference
    softmax_ref(in, ref, kSize);
    
    // Call HLS function
    softmax(in, out);
    
    // Verify results
    std::cout << "\nOutput values:" << std::endl;
    float max_error = 0.0f;
    float sum_out = 0.0f;
    
    for (int i = 0; i < kSize; i++) {
        float error = fabsf(out[i] - ref[i]);
        if (error > max_error) max_error = error;
        sum_out += out[i];
        
        std::cout << out[i] << " ";
        if ((i + 1) % 10 == 0) std::cout << std::endl;
    }
    
    std::cout << "\nSum of outputs: " << sum_out << " (should be ~1.0)" << std::endl;
    std::cout << "Max error vs reference: " << max_error << std::endl;
    
    // Check if test passed
    if (max_error < 1e-5f && fabsf(sum_out - 1.0f) < 1e-5f) {
        std::cout << "\n*** TEST PASSED ***" << std::endl;
        return 0;
    } else {
        std::cout << "\n*** TEST FAILED ***" << std::endl;
        return 1;
    }
}
