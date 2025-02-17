#include "sobelFilter.hpp"

void sobelFilter(hls::stream<int>& inputImage, hls::stream<int>& outputImage) {
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
        // Load the new row into lineBuffer2
        data_t newPixel;
        LOAD_ROW:
        for (int col = 0; col < WIDTH + 2; col++) {
#pragma HLS PIPELINE II=1
            if (col == 0 || col == WIDTH + 1) {
                newPixel = 0;
            } else {
                newPixel = inputImage.read();
           }
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
            // Update the line buffer
            lineBuffer1[col] = lineBuffer2[col];
            lineBuffer2[col] = newPixel;

            // Calculate Sobel and write to output
            if (col > 0 && col < WIDTH + 1) {
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
    }
}
