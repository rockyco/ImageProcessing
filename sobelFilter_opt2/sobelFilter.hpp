#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hls_stream.h"

#define WIDTH 640
#define HEIGHT 480

typedef uint8_t data_t;

void sobelFilter(hls::stream<int>& inputImage, hls::stream<int>& outputImage);
