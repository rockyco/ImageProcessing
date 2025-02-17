## Optimize HLS C++ design to reduce latency

rockyco: Optimize the HLS C++ module design to reduce latency. Define a 3-by-3 shift register to store a sliding window of input image pixels, and use two memory buffers to store the previous two lines of image pixels. Ensure that the Sobel filter operations are performed once per clock cycle.

GitHub Copilot: ```cpp
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
    data_t pixelWindow[3][3];
#pragma HLS ARRAY_PARTITION variable=pixelWindow complete dim=1
#pragma HLS ARRAY_PARTITION variable=pixelWindow complete dim=2

    // Initialize line buffers with 0
    INIT_BUFFERS:
    for (int i = 0; i < WIDTH + 2; i++) {
        lineBuffer1[i] = 0;
        lineBuffer2[i] = 0;
    }

    // Initialize pixel window with 0
    INIT_WINDOW:
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            pixelWindow[i][j] = 0;
        }
    }

    // Process the image row by row
    ROW_LOOP:
    for (int row = 0; row < HEIGHT; row++) {
        // Shift line buffers: lineBuffer1 becomes the previous row
        for(int i = 0 ; i < WIDTH+2 ; i++){
            data_t tempData = lineBuffer1[i];
            lineBuffer1[i] = lineBuffer2[i];
            lineBuffer2[i] = tempData;
        }

        // Load the new row into lineBuffer2
        LOAD_ROW:
        for (int col = 0; col < WIDTH; col++) {
#pragma HLS PIPELINE II=1
            data_t newPixel;
            inputImage >> newPixel;
            lineBuffer2[col+1] = newPixel;

            // Update pixel window
            pixelWindow[0][0] = pixelWindow[0][1];
            pixelWindow[0][1] = pixelWindow[0][2];
            pixelWindow[0][2] = lineBuffer1[col];
            pixelWindow[1][0] = pixelWindow[1][1];
            pixelWindow[1][1] = pixelWindow[1][2];
            pixelWindow[1][2] = lineBuffer2[col];
            pixelWindow[2][0] = pixelWindow[2][1];
            pixelWindow[2][1] = pixelWindow[2][2];
            pixelWindow[2][2] = newPixel;

            // Calculate Sobel and write to output
            if (row > 0 && col > 0) {
                int Gx = (pixelWindow[0][0] + 2 * pixelWindow[0][1] + pixelWindow[0][2]) -
                         (pixelWindow[2][0] + 2 * pixelWindow[2][1] + pixelWindow[2][2]);
                int Gy = (pixelWindow[0][0] + 2 * pixelWindow[1][0] + pixelWindow[2][0]) -
                         (pixelWindow[0][2] + 2 * pixelWindow[1][2] + pixelWindow[2][2]);

                int gradient = (int)(abs(Gx) + abs(Gy));

                if (gradient >= 255) {
                    outputImage << 255;
                } else {
                    outputImage << gradient;
                }
            }
        }
        lineBuffer2[0] = 0;
        lineBuffer2[WIDTH+1] = 0;
    }
}
```
