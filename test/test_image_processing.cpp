#include "image_processing.hpp"
#include <gtest/gtest.h>
#include <cstdlib>
#include <vector>
#include <array>
#include <iostream>
#include <algorithm>

#include <cmath>



// ============================================================================
// This test suite performs BIT-LEVEL VALIDATION of the convertToGrayscale()
// function provided in the compiled static library. The purpose is to test 
// the grayscale conversion behavior under single-bit changes in RGB input
// channels and check for correctness, sensitivity, and potential bugs.


// TEST 1 — RedChannel — Flip bit 1 in R (should change output, but doesn't)

// Expected behavior: RedChannel method should return the R value exactly.
// Input: R = 212 → flipped bit 1 → R = 214.
// Actual output: 0 vs 0 ⇒ Bug suspected: R is ignored or incorrectly handled.
TEST(BitLevelTest, RedChannel_Bit1_Flip) {
    std::array<int, 3> base_pixel = {212, 0, 0};
    std::array<int, 3> mod_pixel  = {212 ^ (1 << 1), 0, 0}; // flip bit 1 → 214

    std::vector<std::vector<std::array<int, 3>>> base_image = {{base_pixel}};
    std::vector<std::vector<std::array<int, 3>>> mod_image  = {{mod_pixel}};
    std::vector<std::vector<int>> base_result, mod_result;

    convertToGrayscale(base_image, 1, 1, GrayscaleMethod::RedChannel, base_result);
    convertToGrayscale(mod_image, 1, 1, GrayscaleMethod::RedChannel, mod_result);

    std::cout << "RedChannel: " << base_result[0][0] << " vs " << mod_result[0][0] << std::endl;

    EXPECT_NE(base_result[0][0], mod_result[0][0])
        << "RedChannel method did not react to a bit flip in R → potential implementation bug.";
}

// TEST 2 — GreenChannel — Flip MSB (bit 7) in G (large change, no effect)

// Expected behavior: GreenChannel method should return the G value.
// Input: G = 64 → flipped bit 7 → G = 192.
// Actual output: 220 vs 220 ⇒ Bug: either hardcoded value or ignored channel.
TEST(BitLevelTest, GreenChannel_Bit7_Flip) {
    std::array<int, 3> base_pixel = {0, 64, 0};
    std::array<int, 3> mod_pixel  = {0, 64 ^ (1 << 7), 0}; // G = 192

    std::vector<std::vector<std::array<int, 3>>> base_image = {{base_pixel}};
    std::vector<std::vector<std::array<int, 3>>> mod_image  = {{mod_pixel}};
    std::vector<std::vector<int>> base_result, mod_result;

    convertToGrayscale(base_image, 1, 1, GrayscaleMethod::GreenChannel, base_result);
    convertToGrayscale(mod_image, 1, 1, GrayscaleMethod::GreenChannel, mod_result);

    std::cout << "GreenChannel: " << base_result[0][0] << " vs " << mod_result[0][0] << std::endl;

    EXPECT_NE(base_result[0][0], mod_result[0][0])
        << "GreenChannel method did not respond to a major G value flip → suspicious behavior.";
}

// TEST 3 — BlueChannel — Flip bit 5 (small change, effective response)

// Expected behavior: BlueChannel returns B.
// Input: B = 32 → flip bit 5 → B = 0.
// Output: 32 vs 10 ⇒ correct behavior. This method seems implemented correctly.
TEST(BitLevelTest, BlueChannel_Bit5_Flip) {
    std::array<int, 3> base_pixel = {0, 0, 32};
    std::array<int, 3> mod_pixel  = {0, 0, 0};

    std::vector<std::vector<std::array<int, 3>>> base_image = {{base_pixel}};
    std::vector<std::vector<std::array<int, 3>>> mod_image  = {{mod_pixel}};
    std::vector<std::vector<int>> base_result, mod_result;

    convertToGrayscale(base_image, 1, 1, GrayscaleMethod::BlueChannel, base_result);
    convertToGrayscale(mod_image, 1, 1, GrayscaleMethod::BlueChannel, mod_result);

    std::cout << "BlueChannel: " << base_result[0][0] << " vs " << mod_result[0][0] << std::endl;

    EXPECT_NE(base_result[0][0], mod_result[0][0]);
}

// TEST 4 — Luminosity — Flip bit 3 in G (expected visible effect)

// Luminosity weights: 0.21*R + 0.72*G + 0.07*B → G has major influence.
// Input: G = 128 → flip bit 3 → G = 136.
// Output: value increased slightly ⇒ correct behavior.
TEST(BitLevelTest, Luminosity_Bit3_GreenChannel) {
    std::array<int, 3> base_pixel = {10, 128, 10};
    std::array<int, 3> mod_pixel  = {10, 136, 10};

    std::vector<std::vector<std::array<int, 3>>> base_image = {{base_pixel}};
    std::vector<std::vector<std::array<int, 3>>> mod_image  = {{mod_pixel}};
    std::vector<std::vector<int>> base_result, mod_result;

    convertToGrayscale(base_image, 1, 1, GrayscaleMethod::Luminosity, base_result);
    convertToGrayscale(mod_image, 1, 1, GrayscaleMethod::Luminosity, mod_result);

    std::cout << "Luminosity: " << base_result[0][0] << " vs " << mod_result[0][0] << std::endl;

    EXPECT_NE(base_result[0][0], mod_result[0][0]);
}

// TEST 5 — Lightness — Flip bit 6 in R (affects max(R,G,B))

// Lightness = (max(R,G,B) + min(R,G,B)) / 2
// Input: R = 64 → flip bit 6 → R = 0 ⇒ major difference expected.
TEST(BitLevelTest, Lightness_Bit6_RedChannel) {
    std::array<int, 3> base_pixel = {64, 10, 10};
    std::array<int, 3> mod_pixel  = {0, 10, 10};

    std::vector<std::vector<std::array<int, 3>>> base_image = {{base_pixel}};
    std::vector<std::vector<std::array<int, 3>>> mod_image  = {{mod_pixel}};
    std::vector<std::vector<int>> base_result, mod_result;

    convertToGrayscale(base_image, 1, 1, GrayscaleMethod::Lightness, base_result);
    convertToGrayscale(mod_image, 1, 1, GrayscaleMethod::Lightness, mod_result);

    std::cout << "Lightness: " << base_result[0][0] << " vs " << mod_result[0][0] << std::endl;

    EXPECT_NE(base_result[0][0], mod_result[0][0]);
}

// TEST 6 — Average — Change R by +1 (should increase result by ~1/3)

// Formula: (R+G+B)/3 → Change R = 100 → 101.
// Output should increase ~1/3 (i.e. ~+1 after integer rounding).
// To avoid false negatives due to rounding, we use EXPECT_NEAR().
TEST(BitLevelTest, AverageMethod_RedChange1) {
    std::array<int, 3> base_pixel = {100, 50, 50};
    std::array<int, 3> mod_pixel  = {101, 50, 50};

    std::vector<std::vector<std::array<int, 3>>> base_image = {{base_pixel}};
    std::vector<std::vector<std::array<int, 3>>> mod_image  = {{mod_pixel}};
    std::vector<std::vector<int>> base_result, mod_result;

    convertToGrayscale(base_image, 1, 1, GrayscaleMethod::Average, base_result);
    convertToGrayscale(mod_image, 1, 1, GrayscaleMethod::Average, mod_result);

    std::cout << "Average: " << base_result[0][0] << " vs " << mod_result[0][0] << std::endl;

    EXPECT_NEAR(mod_result[0][0], base_result[0][0] + 1, 1)
        << "Average method should react to small changes proportionally (~1/3 of delta).";
}

// TEST 7 — RMS — Nonlinear behavior under jump in G

// RMS = sqrt((R² + G² + B²)/3) → jump in G from 0 to 64 should increase RMS.
// This test ensures RMS is correctly sensitive to larger component values.
TEST(BitLevelTest, RMSMethod_GreenJump) {
    std::array<int, 3> base_pixel = {10, 0, 10};
    std::array<int, 3> mod_pixel  = {10, 64, 10};

    std::vector<std::vector<std::array<int, 3>>> base_image = {{base_pixel}};
    std::vector<std::vector<std::array<int, 3>>> mod_image  = {{mod_pixel}};
    std::vector<std::vector<int>> base_result, mod_result;

    convertToGrayscale(base_image, 1, 1, GrayscaleMethod::RootMeanSquare, base_result);
    convertToGrayscale(mod_image, 1, 1, GrayscaleMethod::RootMeanSquare, mod_result);

    std::cout << "RMS: " << base_result[0][0] << " vs " << mod_result[0][0] << std::endl;

    EXPECT_GT(mod_result[0][0], base_result[0][0])
        << "RMS should grow significantly when G changes from 0 to 64.";
}

// ===================================================
// R: Output should be exactly the R value, in [0, 255]
// ===================================================

TEST(RedChannel_Limits, R_InRange) {
    std::array<int, 3> pixel = {255, 0, 0};
    std::vector<std::vector<std::array<int, 3>>> image = {{pixel}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 1, GrayscaleMethod::RedChannel, result);
    EXPECT_GE(result[0][0], 0);
    EXPECT_LE(result[0][0], 255);
}

TEST(RedChannel_Limits, R_NotNegative) {
    std::array<int, 3> pixel = {0, 100, 100};
    std::vector<std::vector<std::array<int, 3>>> image = {{pixel}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 1, GrayscaleMethod::RedChannel, result);
    EXPECT_GE(result[0][0], 0);
}

// ===================================================
// G: Output should be G only, in [0, 255]
// ===================================================

TEST(GreenChannel_Limits, G_InRange) {
    std::array<int, 3> pixel = {0, 255, 0};
    std::vector<std::vector<std::array<int, 3>>> image = {{pixel}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 1, GrayscaleMethod::GreenChannel, result);
    EXPECT_GE(result[0][0], 0);
    EXPECT_LE(result[0][0], 255);
}

TEST(GreenChannel_Limits, G_NotNegative) {
    std::array<int, 3> pixel = {100, 0, 100};
    std::vector<std::vector<std::array<int, 3>>> image = {{pixel}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 1, GrayscaleMethod::GreenChannel, result);
    EXPECT_GE(result[0][0], 0);
}

// ===================================================
// B: Output should be B only, in [0, 255]
// ===================================================

TEST(BlueChannel_Limits, B_InRange) {
    std::array<int, 3> pixel = {0, 0, 255};
    std::vector<std::vector<std::array<int, 3>>> image = {{pixel}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 1, GrayscaleMethod::BlueChannel, result);
    EXPECT_GE(result[0][0], 0);
    EXPECT_LE(result[0][0], 255);
}

TEST(BlueChannel_Limits, B_NotNegative) {
    std::array<int, 3> pixel = {100, 100, 0};
    std::vector<std::vector<std::array<int, 3>>> image = {{pixel}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 1, GrayscaleMethod::BlueChannel, result);
    EXPECT_GE(result[0][0], 0);
}

// ===================================================
// Luminosity = 0.21*R + 0.72*G + 0.07*B
// ===================================================

TEST(Luminosity_Limits, InRange) {
    std::array<int, 3> pixel = {255, 255, 255};
    std::vector<std::vector<std::array<int, 3>>> image = {{pixel}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 1, GrayscaleMethod::Luminosity, result);
    EXPECT_GE(result[0][0], 0);
    EXPECT_LE(result[0][0], 255);
}

TEST(Luminosity_Limits, NotNegative) {
    std::array<int, 3> pixel = {0, 0, 0};
    std::vector<std::vector<std::array<int, 3>>> image = {{pixel}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 1, GrayscaleMethod::Luminosity, result);
    EXPECT_GE(result[0][0], 0);
}

// ===================================================
// Lightness = (max(R,G,B) + min(R,G,B)) / 2
// ===================================================

TEST(Lightness_Limits, InRange) {
    std::array<int, 3> pixel = {255, 0, 255};
    std::vector<std::vector<std::array<int, 3>>> image = {{pixel}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 1, GrayscaleMethod::Lightness, result);
    EXPECT_GE(result[0][0], 0);
    EXPECT_LE(result[0][0], 255);
}

TEST(Lightness_Limits, NotNegative) {
    std::array<int, 3> pixel = {0, 0, 0};
    std::vector<std::vector<std::array<int, 3>>> image = {{pixel}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 1, GrayscaleMethod::Lightness, result);
    EXPECT_GE(result[0][0], 0);
}

// ===================================================
// Average = (R + G + B) / 3
// ===================================================

TEST(Average_Limits, InRange) {
    std::array<int, 3> pixel = {255, 255, 255};
    std::vector<std::vector<std::array<int, 3>>> image = {{pixel}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 1, GrayscaleMethod::Average, result);
    EXPECT_GE(result[0][0], 0);
    EXPECT_LE(result[0][0], 255);
}

TEST(Average_Limits, NotNegative) {
    std::array<int, 3> pixel = {0, 0, 0};
    std::vector<std::vector<std::array<int, 3>>> image = {{pixel}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 1, GrayscaleMethod::Average, result);
    EXPECT_GE(result[0][0], 0);
}

// ===================================================
// RMS = sqrt((R^2 + G^2 + B^2) / 3)
// ===================================================

TEST(RMS_Limits, InRange) {
    std::array<int, 3> pixel = {255, 255, 255};
    std::vector<std::vector<std::array<int, 3>>> image = {{pixel}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 1, GrayscaleMethod::RootMeanSquare, result);
    EXPECT_GE(result[0][0], 0);
    EXPECT_LE(result[0][0], 255);
}

TEST(RMS_Limits, NotNegative) {
    std::array<int, 3> pixel = {0, 0, 0};
    std::vector<std::vector<std::array<int, 3>>> image = {{pixel}};
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 1, 1, GrayscaleMethod::RootMeanSquare, result);
    EXPECT_GE(result[0][0], 0);
}











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