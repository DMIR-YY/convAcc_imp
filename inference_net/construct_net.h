#ifndef _CONSTRUCT_NET_H_
#define _CONSTRUCT_NET_H_
#include <iostream>
#include <ap_fixed.h>
#include "config.h"
#include "acc_instance.h"

using namespace std;


void conv_pool_layer(
    int layer_param[16],
    int layer_param_1[16],
    int conv_param[16],
    int pool_param[16],
    data_type_w *conv_weight_mem_port,
    data_type_w *conv_bias_mem_port,
    data_type_w *temp_out_0_1,
    data_type_w *temp_out_1_1,
    data_type_w in_buf_0[][IBUF_t][IBUF_t],
    data_type_w w_buf_0[][Tm][WBUF_t][WBUF_t],
    data_type_w b_buf_0[],
    data_type_w out_buf_0[][OBUF_t][OBUF_t],
    data_type_w out_buf_1[][OBUF_t][OBUF_t],
    int w_r_offset,
    int w_c_offset){

    int acc_call_rounds  = 0;
    for(int r = 0; r < layer_param[5]&&r<IBUF_t; r += IBUF_t) {
        for (int c = 0; c < layer_param[6]&&c<IBUF_t; c += IBUF_t) {
            for (int m = 0; m < layer_param[2]; m += Tm) {
                for (int n = 0; n < layer_param[0]; n += Tn) {

                    // fill in buffer
                    conv_param[1] = n;
                    conv_param[2] = r;
                    conv_param[3] = c;

                    convAcc1.in_buf_load(in_buf_0, temp_out_0_1, 0, n, r, c, layer_param[7], layer_param[1], layer_param[8], layer_param[3], layer_param[4], layer_param[0]);
                    convAcc1.b_buf_load(b_buf_0, conv_bias_mem_port, layer_param[11], m);
#if _LAYER_MODE_
                    convAcc1.w_buf_load(w_buf_0, conv_weight_mem_port, layer_param[10], n, m, layer_param[1], layer_param[0], layer_param[2]);
#endif
#if _C_DEBUG_MODE_
                    ofstream conv_out;
                    conv_out.open("in_buf_data.txt", ios::app);
                    conv_out <<"conv input: "<< endl;
                    for (int i = 0; i < layer_param[0]; i++) {
                        for (int j = 0; j < layer_param[3]+layer_param[8]*2; j++) {
                            for(int k = 0; k < layer_param[4]+layer_param[8]*2; k++){
                                conv_out << in_buf_0[i][j][k] << " ";
                            }
                            conv_out << endl;
                        }
                        conv_out << endl;
                    }
                    conv_out.close();

                    /*cout << "conv acc round : "  << acc_call_rounds << endl;
                    cout << "b buf 0 data :" << endl;
                    for (int i =0; i<Tm && i<layer_param[2]; i++) {
                        cout << b_buf_0[i] << "  ";
                    }
                    cout << endl;
                    cout << endl;
                    cout<< "w buf 0 data :" << endl;
                    for(int a=0; a<Tn && a<layer_param[0]; a++) {
                        for (int i = 0; i < Tm && i < layer_param[2]; i++) {
                            for (int j = 0; j < layer_param[1]; j++) {
                                for (int k = 0; k < layer_param[1]; k++) {
                                    cout << w_buf_0[a][i][j][k] << "  ";
                                }
                                cout << endl;
                            }
                            cout << endl;
                        }
                        cout << endl;
                    }
                    cout << endl;
                    cout << endl;
                    cout << "in buf 0 data :" << endl;
                    for (int i =0; i<layer_param[0] && i<Tn; i++){
                        for (int j=0; j<layer_param[3] && j<IBUF_t; j++){
                            for(int k=0; k<layer_param[4] && k<IBUF_t; k++) {
                                cout << in_buf_0[i][j][k] << " ";
                            }
                            cout << endl;
                        }
                        cout << endl;
                    }
                    cout << endl;
                    cout << endl;*/
#endif

#if _C_DEBUG_MODE_
                    cout << "conv layer parameters :" << endl;
                    for(int i =0; i<16; i++){cout << layer_param[i] << "  ";} cout << endl;
#endif
                    // compute buffered data
                    for(int r_offset=0; r_offset < (OBUF_t>layer_param[5]?layer_param[5]:OBUF_t); r_offset+=Tr) {
                        for(int c_offset=0; c_offset < (IBUF_t>layer_param[6]?layer_param[6]:IBUF_t);c_offset+=Tc) {
                            acc_call_rounds++;
                            conv_param[5] = r_offset;
                            conv_param[6] = c_offset;
                            for(int i =0; i<16; i++){cout << conv_param[i] << "  ";} cout << endl;

                            // FPGA impl parameter load should happen here
                            cout << "conv_param = {" ;
                            for (int i = 0; i < 16; i++) {cout << conv_param[i] << ", ";}
                            cout << "}; " << endl;
                            cout << "pool_param = {" ;
                            for (int i = 0; i < 16; i++) {cout << pool_param[i] << ", ";}
                            cout << "}; " << endl;

                            // Accelerator core execution
                            conv_core_syn(in_buf_0, in_buf_1, w_buf_0, b_buf_0, out_buf_0, out_buf_1, conv_param, pool_param);
                            cout << "acc call round = " << acc_call_rounds << endl;
                            for (int i = 0; i < 32; i++) {
                                for (int j = 0; j < 32; j++) {
                                    cout << out_buf_0[0][i][j] << "  ";
                                }
                                cout << endl;
                            }
                            cout << endl;
                        }
                    }
                    //pool_core_syn(out_buf_0, out_buf_1, pool_engine_param_in);

#if _C_DEBUG_MODE_
                    // read results out
                    maxPoolAcc1.output_res(out_buf_0, temp_out_1_1, m, r, c, layer_param_1[2], layer_param_1[4], layer_param_1[5], 1);
                    conv_out.open("conv_pool_buf_data.txt", ios::app);
                    conv_out <<"conv_pool output: "<< endl;
                    for (int i = 0; i < layer_param[2]; i++) {
                        for (int j = 0; j < layer_param_1[4]; j++) {
                            for(int k = 0; k < layer_param_1[5]; k++){
                                conv_out << *(temp_out_1_1 + i*layer_param_1[4]*layer_param_1[5]+j*layer_param_1[5]+k) << " ";
                            }
                            conv_out << endl;
                        }
                        conv_out << endl;
                    }
                    conv_out.close();
#endif
                }
            }
        }
    }

}
/*
void   inference_net(
   data_type_w *conv_weight_port,
   data_type_w *fc_weight_port,
   data_type_w *fc_bias_port,
   data_type_o   fc_3_out_a[10*1*1],
   data_type_o   temp_out_0_1[4704],
   data_type_o   temp_out_1_1[4704]){
#if _HLS_MODE_
#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS
#pragma HLS INTERFACE m_axi depth=2550 port=conv_weight_port
#pragma HLS INTERFACE m_axi depth=4000 port=temp_out_0_1
#pragma HLS INTERFACE m_axi depth=10 port=temp_out_1_1
#pragma HLS INTERFACE m_axi depth=10 port=fc_weight_port
#pragma HLS INTERFACE m_axi depth=4704 port=fc_bias_port
#pragma HLS INTERFACE m_axi depth=4704 port=fc_3_out_a
#endif
#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
   cout << "starting forward network process..........................." << endl;
   cout << "..........................................................." << endl;
#endif
#endif
#if _LAYER_MODE_
   int shift_weight_conv1_1 = 0;
   int shift_weight_conv2_1 = 150;
   int shift_weight_fc1 = 0;
   int shift_bias_fc1 = 0;
   conv_layer_new(1, 5, 6, 28, 28, 28, 28, 1, 2, 1, conv_weight_port, shift_weight_conv1_1, 0, 0,  temp_out_0_1,  temp_out_1_1);
   max_pool_layer_new(28, 28, 6, 2, 14, 14, 2, 0, 1,  temp_out_1_1,  temp_out_0_1);
   conv_layer_new(6, 5, 16, 14, 14, 10, 10, 1, 0, 1, conv_weight_port, shift_weight_conv2_1, 0, 0,  temp_out_0_1,  temp_out_1_1);
   max_pool_layer_new(10, 10, 16, 2, 5, 5, 2, 0, 1,  temp_out_1_1,  temp_out_0_1);
   conv_layer_new(16, 5, 10, 5, 5, 1, 1, 5, 0, 1, fc_weight_port, fc_bias_port, shift_weight_fc1, shift_bias_fc1, 0, 0,  temp_out_0_1,  temp_out_1_1);
   for (int i = 0; i < 10; i++) {
      fc_3_out_a[i+0] = temp_out_1_1[i/1];
      
   }
#endif
#if _C_DEBUG_MODE_
#if _KERNEL_DEBUG_
   cout << "Finished forward network process .........................." << endl;
   cout << "..........................................................." << endl;
#endif
#endif
}*/

#endif //_CONSTRUCT_NET_H_
