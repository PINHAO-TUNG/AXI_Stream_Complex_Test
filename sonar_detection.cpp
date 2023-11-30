#include <iostream>
#include <cmath>
#include <complex>
#include <algorithm>
#include "hls_x_complex.h"

#include "sonar_detection.h"

static const unsigned input_data_size = 16834;

float abs_complex(const std::complex<float> &z)
{
    float real = z.real();
    float imag = z.imag();
    float norm = real * real + imag * imag;
    return sqrt(norm);
}

void sonar_detection(
    hls::stream<float> &sonar_input,
    hls::stream<float> &ml_out)
{
#pragma HLS DATAFLOW
    // hls::x_complex<float> input[input_data_size / 2];
    std::complex<float> input[input_data_size / 2];
    float abs_input[input_data_size / 2];

    // Reading the first half of sonar_input into the real part of input
    // Reading the second half of sonar_input into the imag part of input

LoadData:
    for (unsigned i = 0; i < input_data_size; ++i)
    {
        if (i < input_data_size / 2)
        {
            input[i].real(sonar_input.read());
        }
        else
        {
            input[i - (input_data_size / 2)].imag(sonar_input.read());
        }
    }

// Additional operations: Multiply by 5, take absolute value, and take logarithm
ABSLog:
    for (unsigned i = 0; i < input_data_size / 2; ++i)
    {
        input[i] *= 5.0;
        abs_input[i] = abs_complex(input[i]);
        abs_input[i] = std::log(abs_input[i] + 1); // Add 1 to avoid log(0)
    }

    // Sorting the array to get the top 10 values
    // std::partial_sort(abs_input, abs_input + 10, abs_input + input_data_size / 2, std::greater<float>());

    // Output the top 10 values to ml_out
    for (unsigned i = 0; i < 10; ++i)
    {
        ml_out.write(abs_input[i]);
    }
}