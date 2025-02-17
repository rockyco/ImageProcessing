#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "sobelFilter.hpp"
#include "hls_stream.h"

void sobelFilter(hls::stream<data_t>& inputImage, hls::stream<data_t>& outputImage) {
#pragma HLS INTERFACE axis port=outputImage
#pragma HLS INTERFACE axis port=inputImage
#pragma HLS INTERFACE ap_ctrl_none port=return

    data_t lineBuffer1[WIDTH+2];
#pragma HLS RESOURCE variable=lineBuffer1 core=RAM_2P_BRAM
    data_t lineBuffer2[WIDTH+2];
#pragma HLS RESOURCE variable=lineBuffer2 core=RAM_2P_BRAM
    data_t tempData;
    // Initialize line buffers with 0
    INIT_BUFFERS:
    for (int i = 0; i < WIDTH + 2; i++) {
        lineBuffer1[i] = 0;
        lineBuffer2[i] = 0;
    }

    // Process the image row by row
    ROW_LOOP:
    for (int row = 0; row < HEIGHT; row++) {
        // Shift line buffers: lineBuffer1 becomes the previous row
        for(int i = 0 ; i < WIDTH+2 ; i++){
            tempData = lineBuffer1[i];
            lineBuffer1[i] = lineBuffer2[i];
            lineBuffer2[i] = tempData;
        }

        // Load the new row into lineBuffer2
        LOAD_ROW:
        for (int col = 0; col < WIDTH; col++) {
            inputImage >> lineBuffer2[col+1];
        }
        lineBuffer2[0] = 0;
        lineBuffer2[WIDTH+1] = 0;

        // Calculate Sobel and write to output
        SOBEL_CALC:
        for (int col = 0; col < WIDTH; col++) {
            int Gx = (lineBuffer1[col] + 2 * lineBuffer1[col+1] + lineBuffer1[col+2]) -
                     (lineBuffer2[col] + 2 * lineBuffer2[col+1] + lineBuffer2[col+2]);
            int Gy = (lineBuffer1[col] + 2 * lineBuffer1[col+1] + lineBuffer2[col]) -
                     (lineBuffer1[col+2] + 2 * lineBuffer2[col+1] + lineBuffer2[col+2]);

            int gradient = (int)(abs(Gx) + abs(Gy));

            if (gradient >= 255) {
                outputImage << 255;
            } else {
                outputImage << gradient;
            }
        }
    }
}
