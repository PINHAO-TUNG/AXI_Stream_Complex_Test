#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <complex>

#include "sonar_detection_axi.h"
// new add
#include <cmath>
#include <math.h>
// new add
#include <vector>

#include "sonar_detection.h"
// ml
#include "nnet_utils/nnet_helpers.h"
using namespace std;

// const double fs = 5000;
// const double duration = 4;
const int num_samples = 8417;
// load data from file
const int INPUT_LENGTH = 8417;

float abs_complex_tb(const std::complex<float> &z)
{
    float real = z.real();
    float imag = z.imag();
    float norm = real * real + imag * imag;
    return sqrt(norm);
}

int main()
{
    // float xn_input[INPUT_LENGTH * 2];
    // float ml_out[10];

    // c data
    vector<std::complex<float>> input_tb(INPUT_LENGTH * 2);
    float abs_input_tb[INPUT_LENGTH];
    float ml_out_tb[10];

    // load input data
    // real part
    ifstream file_data_real("data_real_5m.txt");
    if (!file_data_real)
    {
        cerr << "Failed to open data_real_5m.txt" << endl;
        return 1;
    }

    vector<float> data_real;
    float value_real;
    while (file_data_real >> value_real)
    {
        data_real.push_back(value_real);
    }

    if (data_real.size() < INPUT_LENGTH)
    {
        cerr << "Insufficient data in data_real_5m.txt" << endl;
        return 1;
    }
    // imag part
    ifstream file_data_imag("data_imag_5m.txt");
    if (!file_data_imag)
    {
        cerr << "Failed to open data_imag_5m.txt" << endl;
        return 1;
    }

    vector<float> data_imag;
    float value_imag;
    while (file_data_imag >> value_imag)
    {
        data_imag.push_back(value_imag);
    }

    if (data_imag.size() < INPUT_LENGTH)
    {
        cerr << "Insufficient data in data_imag_5m.txt" << endl;
        return 1;
    }

    // Combine real and imaginary parts into a single vector
    vector<float> xn_input;
    xn_input.reserve(INPUT_LENGTH * 2);
    xn_input.insert(xn_input.end(), data_real.begin(), data_real.end());
    xn_input.insert(xn_input.end(), data_imag.begin(), data_imag.end());

    // Print the generated xn_input values
    for (int i = 0; i < INPUT_LENGTH * 2; i++)
    {
        if (i < INPUT_LENGTH)
        {
            cout << "xn_input[" << i << "].real: " << xn_input[i] << endl;
        }
        else
        {
            cout << "xn_input[" << (i - INPUT_LENGTH) << "].imag: " << xn_input[i] << endl;
        }
    }

    // cout << "checkpoint1" << endl;

    for (int i = 0; i < INPUT_LENGTH; ++i)
    {
        input_tb[i].real(data_real[i]);
    }

    // cout << "checkpoint2" << endl;

    for (int i = 0; i < INPUT_LENGTH; ++i)
    {
        input_tb[i].imag(data_imag[i]);
    }

    // cout << "checkpoint3" << endl;

    for (unsigned i = 0; i < INPUT_LENGTH; ++i)
    {
        input_tb[i] *= 5.0;
        abs_input_tb[i] = abs_complex_tb(input_tb[i]);
        abs_input_tb[i] = std::log(abs_input_tb[i] + 1); // Add 1 to avoid log(0)
    }

    // cout << "checkpoint4" << endl;

    // std::partial_sort(abs_input_tb, abs_input_tb + 10, abs_input_tb + INPUT_LENGTH / 2, std::greater<float>());

    // cout << "checkpoint5" << endl;

    for (unsigned i = 0; i < 10; ++i)
    {
        ml_out_tb[i] = abs_input_tb[i];
    }

    // cout << "checkpoint6" << endl;

    input_axi_t inputs[INPUT_LENGTH * 2];
    nnet::copy_data_axi<float, input_axi_t, 0, INPUT_LENGTH * 2>(xn_input, inputs);
    output_axi_t outputs[10];

    sonar_detection_axi(inputs, outputs);

    nnet::print_result<output_axi_t, 10>(outputs, std::cout, true);

    for (int i = 0; i < 10; i++)
    {
        cout << "ml_out_tb" << i << ": " << ml_out_tb[i] << endl;
    }
    return 0;
}
