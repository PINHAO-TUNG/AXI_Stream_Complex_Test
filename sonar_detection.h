#ifndef SONAR_DETECTION_H_
#define SONAR_DETECTION_H_

#include "ap_int.h"
#include "ap_fixed.h"
#include "hls_stream.h"

// #include "defines.h"

// Prototype of top level function for C-synthesis
void sonar_detection(
    hls::stream<float> &sonar_input,
    hls::stream<float> &ml_out);

#endif