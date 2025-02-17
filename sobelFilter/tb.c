#include "sobel.h"
#include "input.h"
#include "output.h"


int main(){

	    int outputImage[WIDTH * HEIGHT];
	    
	    sobelEdgeDetection(inputImage, outputImage,50);

	    int error = 0;

	    for(int i = 0 ; i < WIDTH * HEIGHT ; i++){
	    	if(outputImage[i]!=outputImage_compare[i]){
	    		error = error + 1;
	    		printf("the %d number output is %d and we expect %d \n",i,outputImage[i],outputImage_compare[i]);
	    	}
	    }


	    if(error == 0)
	    	printf("test passed\n");
	    else
	    	printf("error = %d",error);

	return 0;
}
