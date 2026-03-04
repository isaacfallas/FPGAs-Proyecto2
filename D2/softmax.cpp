/*
 * Softmax Accelerator - Design 2
 * FPGAs Project 2
 */

#include "softmax.h"
#include <cmath>

/*
 * module 1: Load Input
 * Reads data from AXI memory and sends to FIFO stream.
 */
static void load_input(float *in, DataStream &out_stream) {
#pragma HLS INLINE off

load_loop:
    for (int i = 0; i < kSize; i++) {
#pragma HLS PIPELINE
        out_stream.write(in[i]);
    }
}

/*
 * module 2: Compute Softmax
 * Phase 1: Calculate sum of exponentials and store exp values
 * Phase 2: Divide each exp value by the sum
 */
static void compute_softmax(DataStream &in_stream, DataStream &out_stream) {
#pragma HLS INLINE off

    float exp_buffer[kSize];
#pragma HLS BIND_STORAGE variable=exp_buffer type=ram_2p impl=bram

    float sum = 0.0f;

sum_loop:
    for (int i = 0; i < kSize; i++) {
#pragma HLS PIPELINE
        float val = in_stream.read();
        float exp_val = expf(val);
        exp_buffer[i] = exp_val;
        sum += exp_val;
    }

    float scale = 1.0f / sum;

norm_loop:
    for (int i = 0; i < kSize; i++) {
#pragma HLS PIPELINE
        float result = exp_buffer[i] * scale;
        out_stream.write(result);
    }
}

/*
 * module 3: Store Result  
 * Reads from FIFO stream and writes to AXI memory.
 */
static void store_result(DataStream &in_stream, float *out) {
#pragma HLS INLINE off

store_loop:
    for (int i = 0; i < kSize; i++) {
#pragma HLS PIPELINE
        out[i] = in_stream.read();
    }
}

/*
 * Top: Softmax
 * Uses dataflow to run all three modules in parallel.
 */
extern "C" {
void softmax(float *in, float *out) {
#pragma HLS INTERFACE m_axi port=in  offset=slave bundle=gmem0 depth=100
#pragma HLS INTERFACE m_axi port=out offset=slave bundle=gmem1 depth=100
#pragma HLS INTERFACE s_axilite port=return

    static DataStream stream_in;
    static DataStream stream_out;
#pragma HLS STREAM variable=stream_in  depth=16
#pragma HLS STREAM variable=stream_out depth=16

#pragma HLS DATAFLOW
    load_input(in, stream_in);
    compute_softmax(stream_in, stream_out);
    store_result(stream_out, out);
}
}
