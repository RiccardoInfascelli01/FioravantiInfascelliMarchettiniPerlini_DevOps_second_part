#include "image_processing.hpp"
#include <gtest/gtest.h>
#include <cstdlib>
#include <vector>
#include <array>
#include <iostream>
#include <algorithm>
void generateMatrixPair(int height, int width, 
                       std::vector<std::vector<std::array<int, 3>>>& image1,
                       std::vector<std::vector<std::array<int, 3>>>& image2,
                       int comp1 = -1, int comp2 = -1) {
    
    // Resize the matrices
    image1.resize(height, std::vector<std::array<int, 3>>(width));
    image2.resize(height, std::vector<std::array<int, 3>>(width));
    
    // Generate random values for the first matrix
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            image1[i][j][0] = std::rand() % 256; // R value between 0 and 255
            image1[i][j][1] = std::rand() % 256; // G value between 0 and 255
            image1[i][j][2] = std::rand() % 256; // B value between 0 and 255
        }
    }
    
    // Copy to second matrix
    image2 = image1;
    
    // Determine which components to swap
    if (comp1 == -1 || comp2 == -1) {
        // Choose two different random components
        comp1 = std::rand() % 3;
        do {
            comp2 = std::rand() % 3;
        } while (comp2 == comp1);
    }
    
    // Validate component indices
    if (comp1 < 0 || comp1 > 2 || comp2 < 0 || comp2 > 2 || comp1 == comp2) {
        throw std::invalid_argument("Component indices must be 0, 1, or 2 and must be different");
    }
    
    // Swap the specified components in the second matrix
    for (auto& row : image2) {
        for (auto& pixel : row) {
            std::swap(pixel[comp1], pixel[comp2]);
        }
    }
}
TEST(GrayscaleTest, RedChannelConversion) {
    std::vector<std::vector<std::array<int, 3>>> image = {{{255, 0, 0}}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 1, GrayscaleMethod::RedChannel, result);
    EXPECT_EQ(result[0][0], 255); // Red channel only
}

TEST(GrayscaleTest, LuminosityConversion) {
    std::vector<std::vector<std::array<int, 3>>> image = {{{255, 255, 255}}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 1, GrayscaleMethod::Luminosity, result);
    EXPECT_EQ(result[0][0], 255); // White pixel, should remain max gray
}

TEST(GrayscaleTest, AverageConversion) {
    std::vector<std::vector<std::array<int, 3>>> image = {{{30, 90, 150}}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 1, GrayscaleMethod::Average, result);
    EXPECT_EQ(result[0][0], (30 + 90 + 150) / 3); // Arithmetic mean
}
TEST(MetamorphicTest, MetamorphicTest_Average){
    const int height = 50;
    const int width = 50;

    std::vector<std::vector<int>> result1;
    std::vector<std::vector<int>> result2;
    std::vector<std::vector<std::array<int, 3>>> image1;
    std::vector<std::vector<std::array<int, 3>>> image2;

    // Generate matrices with randomly chosen components swapped
    generateMatrixPair(height, width, image1, image2);

    convertToGrayscale(image1, width, height, GrayscaleMethod::Average, result1);
    convertToGrayscale(image2, width, height, GrayscaleMethod::Average, result2);
    EXPECT_EQ(result1, result2);
}

TEST(MetamorphicTests, MetamorphicTest_Average){
    const int height = 50;
    const int width = 50;

    std::vector<std::vector<int>> result1;
    std::vector<std::vector<int>> result2;
    std::vector<std::vector<std::array<int, 3>>> image1;
    std::vector<std::vector<std::array<int, 3>>> image2;

    // Generate matrices with randomly chosen components swapped
    generateMatrixPair(height, width, image1, image2);

    convertToGrayscale(image1, width, height, GrayscaleMethod::Average, result1);
    convertToGrayscale(image2, width, height, GrayscaleMethod::Average, result2);
    EXPECT_EQ(result1, result2);
}

TEST(MetamorphicTests, MetamorphicTest_Lightness){
    const int height = 50;
    const int width = 50;

    std::vector<std::vector<int>> result1;
    std::vector<std::vector<int>> result2;
    std::vector<std::vector<std::array<int, 3>>> image1;
    std::vector<std::vector<std::array<int, 3>>> image2;

    // Generate matrices with randomly chosen components swapped
    generateMatrixPair(height, width, image1, image2);

    convertToGrayscale(image1, width, height, GrayscaleMethod::Lightness, result1);
    convertToGrayscale(image2, width, height, GrayscaleMethod::Lightness, result2);
    EXPECT_EQ(result1, result2);
}

TEST(MetamorphicTest, MetamorphicTest_RMS){
    const int height = 50;
    const int width = 50;

    std::vector<std::vector<int>> result1;
    std::vector<std::vector<int>> result2;
    std::vector<std::vector<std::array<int, 3>>> image1;
    std::vector<std::vector<std::array<int, 3>>> image2;

    // Generate matrices with randomly chosen components swapped
    generateMatrixPair(height, width, image1, image2);

    convertToGrayscale(image1, width, height, GrayscaleMethod::RootMeanSquare, result1);
    convertToGrayscale(image2, width, height, GrayscaleMethod::RootMeanSquare, result2);
    EXPECT_EQ(result1, result2);
}
TEST(MetamorphicTest, MetamorphicTest_RedChannel){
    const int height = 50;
    const int width = 50;

    std::vector<std::vector<int>> result1;
    std::vector<std::vector<int>> result2;
    std::vector<std::vector<std::array<int, 3>>> image1;
    std::vector<std::vector<std::array<int, 3>>> image2;

    // Generate matrices with randomly chosen components swapped
    generateMatrixPair(height, width, image1, image2,1,2);

    convertToGrayscale(image1, width, height, GrayscaleMethod::RedChannel, result1);
    convertToGrayscale(image2, width, height, GrayscaleMethod::RedChannel, result2);
    EXPECT_EQ(result1, result2);
}
TEST(MetamorphicTest, MetamorphicTest_BlueChannel){
    const int height = 50;
    const int width = 50;

    std::vector<std::vector<int>> result1;
    std::vector<std::vector<int>> result2;
    std::vector<std::vector<std::array<int, 3>>> image1;
    std::vector<std::vector<std::array<int, 3>>> image2;

    // Generate matrices with randomly chosen components swapped
    generateMatrixPair(height, width, image1, image2,0,1);

    convertToGrayscale(image1, width, height, GrayscaleMethod::BlueChannel, result1);
    convertToGrayscale(image2, width, height, GrayscaleMethod::BlueChannel, result2);
    EXPECT_EQ(result1, result2);
}
TEST(MetamorphicTest, MetamorphicTest_GreenChannel){
    const int height = 50;
    const int width = 50;

    std::vector<std::vector<int>> result1;
    std::vector<std::vector<int>> result2;
    std::vector<std::vector<std::array<int, 3>>> image1;
    std::vector<std::vector<std::array<int, 3>>> image2;

    // Generate matrices with randomly chosen components swapped
    generateMatrixPair(height, width, image1, image2,0,2);

    convertToGrayscale(image1, width, height, GrayscaleMethod::GreenChannel, result1);
    convertToGrayscale(image2, width, height, GrayscaleMethod::GreenChannel, result2);
    EXPECT_EQ(result1, result2);
}