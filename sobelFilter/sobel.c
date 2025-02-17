#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define WIDTH 64
#define HEIGHT 64

//#define EDGE_THRESHOLD 50

//#define INPUT_DEBUG
//#define OUTPUT_DEBUG
void sobelEdgeDetection(int *inputImage, int *outputImage,int EDGE_THRESHOLD) {
#pragma HLS INTERFACE m_axi depth=4096 port=outputImage offset=slave bundle=out
#pragma HLS INTERFACE m_axi depth=4096 port=inputImage offset=slave bundle=in
//#pragma HLS INTERFACE ap_ctrl_none port=return

	int image_data[WIDTH+2][WIDTH+2];
#pragma HLS RESOURCE variable=image_data core=RAM_2P_BRAM
IMG_CREATE:
    for(int p = 0 ; p < WIDTH+2 ; p++){
        for(int q = 0 ; q < WIDTH +2 ; q++){
            if( p ==0 || q ==0 || p ==WIDTH+1 || q ==WIDTH+1)
                image_data[p][q]=0;
            else
                image_data[p][q] = inputImage[(q-1)+(p-1)*WIDTH];
            }
    }
#ifdef INPUT_DEBUG
    printf("input print\n");
    FILE* fp_input = fopen("input.txt","wb");
    for(int m = 0 ; m < WIDTH+2 ; m++){
        for(int n = 0 ; n< WIDTH +2 ; n++){
            fprintf(fp_input, "%d\t", image_data[m][n]);
        }
        fprintf(fp_input, "\n"); 
    }
    fclose(fp_input);
#endif
    int Gx[WIDTH][WIDTH];
#pragma HLS RESOURCE variable=Gx core=RAM_2P_LUTRAM
    int Gy[WIDTH][WIDTH];
#pragma HLS RESOURCE variable=Gy core=RAM_2P_LUTRAM
OUTPUT_CREATE:
    for(int i = 1 ; i < WIDTH+1 ; i++){
        for(int j = 1 ; j < WIDTH+1 ; j++){
            Gx[i-1][j-1] = (image_data[i-1][j-1] + 2* image_data[i-1][j] + image_data[i-1][j+1]) - (image_data[i+1][j-1] + 2* image_data[i+1][j] + image_data[i+1][j+1]);
            Gy[i-1][j-1] = (image_data[i-1][j-1] + 2* image_data[i][j-1] + image_data[i+1][j-1]) - (image_data[i-1][j+1] + 2* image_data[i][j+1] + image_data[i+1][j+1]);
            int gradient = (int)sqrt(Gx[i-1][j-1] * Gx[i-1][j-1] + Gy[i-1][j-1] * Gy[i-1][j-1]);
            if (gradient > EDGE_THRESHOLD) {
                outputImage[(j-1)+WIDTH*(i-1)] = 255;  
            } else {
                outputImage[(j-1)+WIDTH*(i-1)] = 0;    
            }
        }
    }

#ifdef OUTPUT_DEBUG
    printf("out print\n");
    FILE* fp_output = fopen("out.txt","wb");
    for(int mm = 1 ; mm < WIDTH+1 ; mm++){
        for(int nn = 1 ; nn < WIDTH+1 ; nn++){
        	int gradient = (int)sqrt(Gx[mm-1][nn-1] * Gx[mm-1][nn-1] + Gy[mm-1][nn-1] * Gy[mm-1][nn-1]);
            fprintf(fp_output, "%d\n", gradient);
        }
    }
    fclose(fp_output);
#endif
}
