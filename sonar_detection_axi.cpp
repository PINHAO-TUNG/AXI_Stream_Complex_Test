#include "sonar_detection_axi.h"
#include <iostream>

void sonar_detection_axi(input_axi_t in[N_IN], output_axi_t out[N_OUT])
{

#pragma HLS INTERFACE axis port = in
#pragma HLS INTERFACE axis port = out
#pragma HLS INTERFACE ap_ctrl_none port = return
#pragma HLS DATAFLOW

    bool is_last = false;
    hls::stream<float> in_local("input_1");
    hls::stream<float> out_local("output_1");

#pragma HLS STREAM variable = in_local depth = 1024
#pragma HLS STREAM variable = out_local depth = 1

    // for (int i = 0; i < N_IN; ++i)
    //{
    //     in_local.write(in[i].data);
    //     is_last |= (in[i].last == 1) ? true : false;
    // }

    for (int i = 0; i < N_IN; ++i)
    {
        input_axi_t tmp_in = in[i];
        in_local.write(tmp_in.data);
        is_last |= (tmp_in.last == 1) ? true : false;
    }

    sonar_detection(in_local, out_local);

    for (int i = 0; i < N_OUT; ++i)
    {
        float data = out_local.read();
        bool last = (is_last && (i == N_OUT - 1)) ? true : false;
        out[i] = output_axi_t(data, last);
    }
}
