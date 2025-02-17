#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hls_stream.h"

#define WIDTH 640
#define HEIGHT 480

typedef int data_t;

void sobelFilter(hls::stream<data_t>& inputImage, hls::stream<data_t>& outputImage);
