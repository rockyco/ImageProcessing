#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "sobelFilter.hpp"


int main() {
    hls::stream<int> inputImage;
    hls::stream<int> outputImage;

    // Read input image from file
    std::ifstream inputFile("pic_in.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open input file." << std::endl;
        return 1;
    }
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        int pixel;
        inputFile >> pixel;
        inputImage << pixel;
    }
    inputFile.close();

    // Run hardware Sobel filter
    sobelFilter(inputImage, outputImage);


    // Compare output with golden reference and write output image to file
    std::ofstream outputFile("../../../../pic_out.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open output file." << std::endl;
        return 1;
    }

    int err_cnt = 0;
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        int outputPixel;
        outputPixel = outputImage.read();
        outputFile << outputPixel << std::endl;
    }
    outputFile.close();

    if (err_cnt == 0) {
        std::cout << "Test passed!" << std::endl;
    } else {
        std::cout << "Error count: " << err_cnt << std::endl;
    }

    return 0;
}
