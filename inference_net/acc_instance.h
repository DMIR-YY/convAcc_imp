#ifndef _ACC_INSTANCE_H_
#define _ACC_INSTANCE_H_

#include "conv_acc_innerpp.h"
#include "max_pool_acc_innerpp.h"
#include "config.h"

conv_acc<data_type, data_type_w, data_type_o, Tm, Tn, Tr, Tc, S_max, K_max, IBUF_t, WBUF_t, OBUF_t> convAcc1;
/*
#if _LAYER_MODE_
void conv_layer_new(
   int N,
   int K,
   int M,
   int R_IN,
   int C_IN,
   int C_OUT,
   int R_OUT,
   int S,
   int P,
   bool act,
   data_type_w *layer_weights,
   data_type_w *layer_bias,
   int weight_offset,
   int bias_offset,
   int in_offset,
   int out_offset,
   data_type *in_data_1,
   data_type_o *out_data_1) {

   convAcc1.conv_layer_acc(N, K, M, R_IN, C_IN, C_OUT, R_OUT, S, P, act, layer_weights, layer_bias, weight_offset, bias_offset, in_offset, out_offset, in_data_1, out_data_1);

}
#endif*/

max_pool_acc<data_type, data_type_w, data_type_o, Tm, Tr, Tc, S_max, K_max> maxPoolAcc1;
/*
#if _LAYER_MODE_
void max_pool_layer_new(
        int R_in,
        int C_in,
        int N,
        int K,
        int R,
        int C,
        int S,
        int P,
        bool act,
        data_type *in_data_1,
        data_type_o *out_data_1) {

    maxPoolAcc1.max_pool_layer_acc(R_in, C_in, N, K, R, C, S, P, act, in_data_1, out_data_1);

}
#endif*/
/*
void conv_core_syn(data_type_w in_buf_0[8][128][128],
                   data_type_w w_buf_0[8][32][16][16],
                   data_type_w b_buf_0[32],
                   data_type_w out_buf_0[32][32][32],
                   int param_in[16])
{
#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS

#pragma HLS ARRAY_PARTITION variable=in_buf_0  complete dim=1
#pragma HLS ARRAY_PARTITION variable=w_buf_0   complete dim=1
#pragma HLS ARRAY_PARTITION variable=w_buf_0   complete dim=2
#pragma HLS ARRAY_PARTITION variable=out_buf_0 complete dim=1

#pragma HLS RESOURCE variable=in_buf_0  core=RAM_1P_BRAM
#pragma HLS RESOURCE variable=w_buf_0   core=RAM_1P_BRAM
#pragma HLS RESOURCE variable=out_buf_0 core=RAM_1P_BRAM
#pragma HLS RESOURCE variable=param_in  core=RAM_1P_BRAM

#pragma HLS INTERFACE bram port=in_buf_0
#pragma HLS INTERFACE bram port=w_buf_0
#pragma HLS INTERFACE bram port=b_buf_0
#pragma HLS INTERFACE bram port=out_buf_0
#pragma HLS INTERFACE bram port=param_in

    int param[16];
    data_type_w b_buf_0_tmp[32];
#pragma HLS ARRAY_PARTITION variable=b_buf_0_tmp complete

    for(int i = 0; i<16; i++){
#pragma HLS PIPELINE
        param[i] = param_in[i];
    }

    for(int i = 0; i<32; i++){
        b_buf_0_tmp[i] = b_buf_0[i];
    }

    convAcc1.conv_core_acc(in_buf_0, w_buf_0, b_buf_0_tmp, out_buf_0, param);
}
*/

void conv_core_syn(data_type_w in_buf_0[Tn][IBUF_t][IBUF_t],
                   data_type_w in_buf_1[Tn][IBUF_t][IBUF_t],
                   data_type_w w_buf_0[Tn][Tm][WBUF_t][WBUF_t],
                   data_type_w b_buf_0[Tm],
                   data_type_w out_buf_0[Tm][OBUF_t][OBUF_t],
                   data_type_w out_buf_1[Tm][OBUF_t][OBUF_t],
                   int conv_param[16],
                   int pool_param[16]) {

#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS

#pragma HLS ARRAY_PARTITION variable=in_buf_0  complete dim=1
#pragma HLS ARRAY_PARTITION variable=in_buf_1  complete dim=1
#pragma HLS ARRAY_PARTITION variable=w_buf_0   complete dim=1
#pragma HLS ARRAY_PARTITION variable=w_buf_0   complete dim=2
#pragma HLS ARRAY_PARTITION variable=out_buf_0 complete dim=1
#pragma HLS ARRAY_PARTITION variable=out_buf_1 complete dim=1

#pragma HLS RESOURCE variable=in_buf_0  core=RAM_1P_BRAM
#pragma HLS RESOURCE variable=in_buf_1  core=RAM_1P_BRAM
#pragma HLS RESOURCE variable=w_buf_0   core=RAM_1P_BRAM
#pragma HLS RESOURCE variable=out_buf_0 core=RAM_1P_BRAM
#pragma HLS RESOURCE variable=out_buf_1 core=RAM_1P_BRAM
#pragma HLS RESOURCE variable=conv_param  core=RAM_1P_BRAM
#pragma HLS RESOURCE variable=pool_param  core=RAM_1P_BRAM

#pragma HLS INTERFACE bram port=in_buf_0
#pragma HLS INTERFACE bram port=in_buf_1
#pragma HLS INTERFACE bram port=w_buf_0
#pragma HLS INTERFACE bram port=b_buf_0
#pragma HLS INTERFACE bram port=out_buf_0
#pragma HLS INTERFACE bram port=out_buf_1
#pragma HLS INTERFACE bram port=conv_param
#pragma HLS INTERFACE bram port=pool_param

    int param1[16];
    int param2[16];

    data_type_w b_buf_0_tmp[Tm];
#pragma HLS ARRAY_PARTITION variable=b_buf_0_tmp complete

    for(int i = 0; i<16; i++){
#pragma HLS PIPELINE
        param1[i] = conv_param[i];
    }

    for(int i = 0; i<16; i++){
#pragma HLS PIPELINE
        param2[i] = pool_param[i];
    }

    for(int i = 0; i<32; i++){
#pragma HLS PIPELINE
        b_buf_0_tmp[i] = b_buf_0[i];
    }

    convAcc1.conv_core_acc(in_buf_0, w_buf_0, b_buf_0_tmp, out_buf_0, param1, param2);
}


/*
void pool_core_syn(data_type_w in_buf[Tm][OBUF_t][OBUF_t],
                   data_type_w out_buf[Tm][OBUF_t][OBUF_t],
                   int pool_param_in[16])
{
#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS

#pragma HLS ARRAY_PARTITION variable=in_buf  complete dim=1
#pragma HLS ARRAY_PARTITION variable=out_buf complete dim=1

#pragma HLS RESOURCE variable=in_buf  core=RAM_1P_BRAM
#pragma HLS RESOURCE variable=out_buf core=RAM_1P_BRAM
#pragma HLS RESOURCE variable=pool_param_in  core=RAM_1P_BRAM

#pragma HLS INTERFACE bram port=in_buf
#pragma HLS INTERFACE bram port=out_buf
#pragma HLS INTERFACE bram port=pool_param_in

    int param[16];
    for(int i = 0; i<16; i++) {
#pragma HLS PIPELINE
        param[i] = pool_param_in[i];
    }
    maxPoolAcc1.mpool_core_acc(in_buf, out_buf, param);

}
*/

#endif
