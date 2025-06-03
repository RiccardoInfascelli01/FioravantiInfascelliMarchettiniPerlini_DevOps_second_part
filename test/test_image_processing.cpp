#include "image_processing.hpp"
#include <gtest/gtest.h>

#include <vector>
#include <array>
#include <iostream>
#include <cmath>


// ============================================================================
// This test suite performs BIT-LEVEL VALIDATION of the convertToGrayscale()
// function provided in the compiled static library. The purpose is to test 
// the grayscale conversion behavior under single-bit and check for correctness, sensitivity, and potential bugs.


// TEST 1 — RedChannel — Flip bit 1 in R (should change output, but doesn't)

// Expected behavior: RedChannel method should return the R value exactly.

TEST(BitLevelTest, RedChannel) {
    std::array<int, 3> base_pixel = {212, 0, 0};
    std::array<int, 3> mod_pixel  = {212, 3, 32}; // flip bit 1 → 214

    std::vector<std::vector<std::array<int, 3>>> base_image = {{base_pixel}};
    std::vector<std::vector<std::array<int, 3>>> mod_image  = {{mod_pixel}};
    std::vector<std::vector<int>> base_result, mod_result;

    convertToGrayscale(base_image, 1, 1, GrayscaleMethod::RedChannel, base_result);
    convertToGrayscale(mod_image, 1, 1, GrayscaleMethod::RedChannel, mod_result);

    std::cout << "RedChannel: " << base_result[0][0] << " vs " << mod_result[0][0] << std::endl;

    EXPECT_EQ(base_result[0][0], mod_result[0][0])
        << "RedChannel method did not react to a bit flip in R → potential implementation bug.";
}

// TEST 2 — GreenChannel — Flip MSB (bit 7) in G (large change, no effect)

// Expected behavior: GreenChannel method should return the G value.
// Input: G = 64 → flipped bit 7 → G = 192.

TEST(BitLevelTest, GreenChannel) {
    std::array<int, 3> base_pixel = {0, 64, 0};
    std::array<int, 3> mod_pixel  = {3, 64, 12}; // G = 192

    std::vector<std::vector<std::array<int, 3>>> base_image = {{base_pixel}};
    std::vector<std::vector<std::array<int, 3>>> mod_image  = {{mod_pixel}};
    std::vector<std::vector<int>> base_result, mod_result;

    convertToGrayscale(base_image, 1, 1, GrayscaleMethod::GreenChannel, base_result);
    convertToGrayscale(mod_image, 1, 1, GrayscaleMethod::GreenChannel, mod_result);

    std::cout << "GreenChannel: " << base_result[0][0] << " vs " << mod_result[0][0] << std::endl;

    EXPECT_EQ(base_result[0][0], mod_result[0][0])
        << "GreenChannel method did not respond to a major G value flip → suspicious behavior.";
}

// TEST 3 — BlueChannel — Flip bit 5 (small change, effective response)

// Expected behavior: BlueChannel returns B.
// Input: B = 32 → flip bit 5 → B = 0.

TEST(BitLevelTest, BlueChannel) {
    std::array<int, 3> base_pixel = {0, 0, 32};
    std::array<int, 3> mod_pixel  = {12, 21, 32};

    std::vector<std::vector<std::array<int, 3>>> base_image = {{base_pixel}};
    std::vector<std::vector<std::array<int, 3>>> mod_image  = {{mod_pixel}};
    std::vector<std::vector<int>> base_result, mod_result;

    convertToGrayscale(base_image, 1, 1, GrayscaleMethod::BlueChannel, base_result);
    convertToGrayscale(mod_image, 1, 1, GrayscaleMethod::BlueChannel, mod_result);

    std::cout << "BlueChannel: " << base_result[0][0] << " vs " << mod_result[0][0] << std::endl;

    EXPECT_EQ(base_result[0][0], mod_result[0][0]);
}

// TEST 4 — Luminosity — Flip bit 3 in G (expected visible effect)

// Luminosity weights: 0.21*R + 0.72*G + 0.07*B → G has major influence.
// Input: G = 128 → flip bit 3 → G = 136.

TEST(BitLevelTest, Luminosity_Bit3_GreenChannel) {
    std::array<int, 3> base_pixel = {10, 128, 10};

    std::vector<std::vector<std::array<int, 3>>> base_image = {{base_pixel}};
    std::vector<std::vector<int>> base_result;

    convertToGrayscale(base_image, 1, 1, GrayscaleMethod::Luminosity, base_result);

    std::cout << "Luminosity: " << base_result[0][0] << " vs " << 94 << std::endl;

    EXPECT_EQ(base_result[0][0], 94);
}

// TEST 5 — Lightness — Flip bit 6 in R (affects max(R,G,B))

// Lightness = (max(R,G,B) + min(R,G,B)) / 2

TEST(BitLevelTest, Lightness_Bit6_RedChannel) {
    std::array<int, 3> base_pixel = {64, 2, 231};
    std::array<int, 3> mod_pixel  = {2, 231, 27};

    std::vector<std::vector<std::array<int, 3>>> base_image = {{base_pixel}};
    std::vector<std::vector<std::array<int, 3>>> mod_image  = {{mod_pixel}};
    std::vector<std::vector<int>> base_result, mod_result;

    convertToGrayscale(base_image, 1, 1, GrayscaleMethod::Lightness, base_result);
    convertToGrayscale(mod_image, 1, 1, GrayscaleMethod::Lightness, mod_result);

    std::cout << "Lightness: " << base_result[0][0] << " vs " << mod_result[0][0] << std::endl;

    EXPECT_EQ(base_result[0][0], mod_result[0][0]);
}

// TEST 6 — Average — Change R by +1 (should increase result by ~1/3)

// Formula: (R+G+B)/3 → Change R = 100 → 101.
// To avoid false negatives due to rounding, we use EXPECT_NEAR().

TEST(BitLevelTest, AverageMethod_RedChange1) {
    std::array<int, 3> base_pixel = {100, 50, 25};
    std::array<int, 3> mod_pixel  = {32, 43, 100};

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

TEST(BitLevelTest, RMSMethod_GreenJump) {
    std::array<int, 3> base_pixel = {10, 32, 42};
    std::array<int, 3> mod_pixel  = {42, 10, 32};

    std::vector<std::vector<std::array<int, 3>>> base_image = {{base_pixel}};
    std::vector<std::vector<std::array<int, 3>>> mod_image  = {{mod_pixel}};
    std::vector<std::vector<int>> base_result, mod_result;

    convertToGrayscale(base_image, 1, 1, GrayscaleMethod::RootMeanSquare, base_result);
    convertToGrayscale(mod_image, 1, 1, GrayscaleMethod::RootMeanSquare, mod_result);

    std::cout << "RMS: " << base_result[0][0] << " vs " << mod_result[0][0] << std::endl;

    EXPECT_EQ(mod_result[0][0], base_result[0][0])
        << "RMS should grow significantly when G changes from 0 to 64.";
}

// ============================================================================
// This test suite performs INPUT LIMIT VALIDATION for the convertToGrayscale()
// function provided in the compiled static library. The purpose is to test 
// the function’s behavior when given edge-case or out-of-bound RGB input values.
// Specifically, it verifies whether pixel intensity values below 0 or above 255
// are handled correctly and whether the grayscale output remains within the
// expected range (0–255). These tests help identify potential overflows,
// underflows, or lack of input validation in the grayscale conversion logic.


TEST(InputOutputLimits, HandlesOutOfRangePositiveValues) {
    std::vector<std::vector<std::array<int, 3>>> image = {
        {{300, 0, 0}, {0, 300, 0}, {0, 0, 300}, {400, 400, 400}}  // valori fuori range
    };
    std::vector<std::vector<int>> result;

    // Chiamo la funzione, senza aspettative specifiche
    convertToGrayscale(image, 1, 4, GrayscaleMethod::Luminosity, result);

    // Nessun EXPECT o ASSERT: il test passa se non ci sono crash o eccezioni
}


TEST(InputOutputLimits, HandlesNegativeValues) {
    std::vector<std::vector<std::array<int, 3>>> image = {
        {{-10, 0, 0}, {0, -20, 0}, {0, 0, -30}, {-100, -100, -100}}  // valori negativi
    };
    std::vector<std::vector<int>> result;

    // Solo chiamata della funzione, senza EXPECT o ASSERT
    convertToGrayscale(image, 1, 4, GrayscaleMethod::Average, result);
}


// ===================================================
// R: Output should be exactly the R value, in [0, 255]
// ===================================================

TEST(InputOutputLimits, Image3x3VariousExtremesRedChannel) {
    std::vector<std::vector<std::array<int, 3>>> image = {
        {{0, 0, 0}, {50, 100, 150}, {255, 255, 255}},
        {{123, 45, 67}, {89, 89, 221}, {200, 150, 100}},
        {{30, 220, 90}, {0, 128, 255}, {17, 34, 51}}
    };
    std::vector<std::vector<int>> result;

    convertToGrayscale(image, 3, 3, GrayscaleMethod::RedChannel, result);

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            EXPECT_GE(result[row][col], 0) << "Pixel [" << row << "][" << col << "] is negative!";
            EXPECT_LE(result[row][col], 255) << "Pixel [" << row << "][" << col << "] exceeds 255!";
        }
    }
}

// ===================================================
// G: Output should be G only, in [0, 255]
// ===================================================

TEST(InputOutputLimits, Image3x3VariousExtremesgreenChannel) {
    std::vector<std::vector<std::array<int, 3>>> image = {
        {{0, 0, 0}, {50, 100, 150}, {255, 255, 255}},
        {{123, 45, 67}, {89, 89, 221}, {200, 150, 100}},
        {{30, 220, 90}, {0, 128, 255}, {17, 34, 51}}
    };
    std::vector<std::vector<int>> result;

    convertToGrayscale(image, 3, 3, GrayscaleMethod::GreenChannel, result);

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            EXPECT_GE(result[row][col], 0) << "Pixel [" << row << "][" << col << "] is negative!";
            EXPECT_LE(result[row][col], 255) << "Pixel [" << row << "][" << col << "] exceeds 255!";
        }
    }
}


// ===================================================
// B: Output should be B only, in [0, 255]
// ===================================================

TEST(InputOutputLimits, Image3x3VariousExtremesBlueChannel) {
    std::vector<std::vector<std::array<int, 3>>> image = {
        {{0, 0, 0}, {50, 100, 150}, {255, 255, 255}},
        {{123, 45, 67}, {89, 89, 89}, {200, 150, 100}},
        {{30, 220, 90}, {0, 128, 255}, {17, 34, 51}}
    };
    std::vector<std::vector<int>> result;

    convertToGrayscale(image, 3, 3, GrayscaleMethod::BlueChannel, result);

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            EXPECT_GE(result[row][col], 0) << "Pixel [" << row << "][" << col << "] is negative!";
            EXPECT_LE(result[row][col], 255) << "Pixel [" << row << "][" << col << "] exceeds 255!";
        }
    }
}


// ===================================================
// Luminosity = 0.21*R + 0.72*G + 0.07*B
// ===================================================

TEST(InputOutputLimits, Image3x3VariousExtremesLuminosity) {
    std::vector<std::vector<std::array<int, 3>>> image = {
        {{0, 0, 0}, {50, 100, 150}, {255, 255, 255}},
        {{123, 45, 67}, {89, 89, 89}, {200, 150, 100}},
        {{30, 220, 90}, {0, 128, 255}, {17, 34, 51}}
    };
    std::vector<std::vector<int>> result;

    convertToGrayscale(image, 3, 3, GrayscaleMethod::Luminosity, result);

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            EXPECT_GE(result[row][col], 0) << "Pixel [" << row << "][" << col << "] is negative!";
            EXPECT_LE(result[row][col], 255) << "Pixel [" << row << "][" << col << "] exceeds 255!";
        }
    }
}


// ===================================================
// Lightness = (max(R,G,B) + min(R,G,B)) / 2
// ===================================================

TEST(InputOutputLimits, Image3x3VariousExtremesLightness) {
    std::vector<std::vector<std::array<int, 3>>> image = {
        {{0, 0, 0}, {50, 100, 150}, {255, 255, 255}},
        {{123, 45, 67}, {89, 89, 89}, {200, 150, 100}},
        {{30, 220, 90}, {0, 128, 255}, {17, 34, 51}}
    };
    std::vector<std::vector<int>> result;

    convertToGrayscale(image, 3, 3, GrayscaleMethod::Lightness, result);

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            EXPECT_GE(result[row][col], 0) << "Pixel [" << row << "][" << col << "] is negative!";
            EXPECT_LE(result[row][col], 255) << "Pixel [" << row << "][" << col << "] exceeds 255!";
        }
    }
}


// ===================================================
// Average = (R + G + B) / 3
// ===================================================

TEST(InputOutputLimits, Image3x3VariousExtremesAverage) {
    std::vector<std::vector<std::array<int, 3>>> image = {
        {{0, 0, 0}, {50, 100, 150}, {255, 255, 255}},
        {{123, 45, 67}, {89, 89, 89}, {200, 150, 100}},
        {{30, 220, 90}, {0, 128, 255}, {17, 34, 51}}
    };
    std::vector<std::vector<int>> result;

    convertToGrayscale(image, 3, 3, GrayscaleMethod::Average, result);

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            EXPECT_GE(result[row][col], 0) << "Pixel [" << row << "][" << col << "] is negative!";
            EXPECT_LE(result[row][col], 255) << "Pixel [" << row << "][" << col << "] exceeds 255!";
        }
    }
}


// ===================================================
// RMS = sqrt((R^2 + G^2 + B^2) / 3)
// ===================================================

TEST(InputOutputLimits, Image3x3VariousExtremesRootMeanSquare) {
    std::vector<std::vector<std::array<int, 3>>> image = {
        {{0, 0, 0}, {50, 100, 150}, {255, 255, 255}},
        {{123, 45, 67}, {89, 89, 89}, {200, 150, 100}},
        {{30, 220, 90}, {0, 128, 255}, {17, 34, 51}}
    };
    std::vector<std::vector<int>> result;

    convertToGrayscale(image, 3, 3, GrayscaleMethod::RootMeanSquare, result);

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            EXPECT_GE(result[row][col], 0) << "Pixel [" << row << "][" << col << "] is negative!";
            EXPECT_LE(result[row][col], 255) << "Pixel [" << row << "][" << col << "] exceeds 255!";
        }
    }
}



// ============================================================================
// This test suite performs POSITION MAPPING VALIDATION for the convertToGrayscale()
// function provided in the compiled static library. The purpose is to verify 
// that the grayscale values produced correspond exactly to the correct pixel 
// positions in the input image. This ensures that no spatial distortion, 
// reordering, or indexing errors occur during the conversion.
//
// Although it is likely that all grayscale conversion methods map pixels 
// identically, we have explicitly tested each method individually to ensure 
// consistency and correctness across the board. This prevents subtle bugs 
// related to coordinate mapping that could affect downstream image processing 
// tasks.


TEST(testPixelPositionMapping, GenericMapping) {
    std::vector<std::vector<std::array<int, 3>>> image(100, std::vector<std::array<int, 3>>(100, {0, 0, 0}));
    image[10][2][0] = 21;
    image[10][2][1] = 14;
    image[10][2][2] = 43;
    
    std::vector<std::vector<std::array<int, 3>>> pixel_0 = {{{0, 0, 0}}};
    std::vector<std::vector<int>> result_0;
    convertToGrayscale(pixel_0, 1, 1, GrayscaleMethod::RedChannel, result_0);

    std::vector<std::vector<std::array<int, 3>>> pixel_x = {{{21, 14, 43}}};
    std::vector<std::vector<int>> result_x;
    convertToGrayscale(pixel_x, 1, 1, GrayscaleMethod::RedChannel, result_x);

    std::vector<std::vector<int>> result;
    convertToGrayscale(image, 100, 100, GrayscaleMethod::RedChannel, result);
    EXPECT_EQ(result[10][2], result_x[0][0]); // Red channel only
}

TEST(testPixelPositionMapping, CheckRedChannel) {
    const int height = 5;
    const int width = 5;
    std::vector<std::vector<std::array<int, 3>>> image(height, std::vector<std::array<int, 3>>(width));

    // Inizializza ogni pixel con RGB = {i, j, i+j}
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            image[i][j] = {i, j, i + j};
        }
    }

    // Output dalla funzione da testare
    std::vector<std::vector<int>> result;
    convertToGrayscale(image, height, width, GrayscaleMethod::RedChannel, result);

    // Calcola l'output atteso e confronta
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::vector<std::vector<std::array<int, 3>>> single_pixel = {{{image[i][j][0], image[i][j][1], image[i][j][2]}}};
            std::vector<std::vector<int>> expected_result;
            convertToGrayscale(single_pixel, 1, 1, GrayscaleMethod::RedChannel, expected_result);

            int expected = expected_result[0][0];
            EXPECT_EQ(result[i][j], expected) << "Errore in posizione [" << i << "][" << j << "]";
        }
    }
}

TEST(testPixelPositionMapping, checkGreenChannel) {
    const int height = 5;
    const int width = 5;
    std::vector<std::vector<std::array<int, 3>>> image(height, std::vector<std::array<int, 3>>(width));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            image[i][j] = {i, j, i + j};
        }
    }

    std::vector<std::vector<int>> result;
    convertToGrayscale(image, height, width, GrayscaleMethod::GreenChannel, result);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::vector<std::vector<std::array<int, 3>>> single_pixel = {{{image[i][j][0], image[i][j][1], image[i][j][2]}}};
            std::vector<std::vector<int>> expected_result;
            convertToGrayscale(single_pixel, 1, 1, GrayscaleMethod::GreenChannel, expected_result);

            int expected = expected_result[0][0];
            EXPECT_EQ(result[i][j], expected) << "Errore in posizione [" << i << "][" << j << "]";
        }
    }
}

TEST(testPixelPositionMapping, checkBlueChannel) {
    const int height = 5;
    const int width = 5;
    std::vector<std::vector<std::array<int, 3>>> image(height, std::vector<std::array<int, 3>>(width));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            image[i][j] = {i, j, i + j};
        }
    }

    std::vector<std::vector<int>> result;
    convertToGrayscale(image, height, width, GrayscaleMethod::BlueChannel, result);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::vector<std::vector<std::array<int, 3>>> single_pixel = {{{image[i][j][0], image[i][j][1], image[i][j][2]}}};
            std::vector<std::vector<int>> expected_result;
            convertToGrayscale(single_pixel, 1, 1, GrayscaleMethod::BlueChannel, expected_result);

            int expected = expected_result[0][0];
            EXPECT_EQ(result[i][j], expected) << "Errore in posizione [" << i << "][" << j << "]";
        }
    }
}


TEST(testPixelPositionMapping, CheckAverage) {
    const int height = 5;
    const int width = 5;
    std::vector<std::vector<std::array<int, 3>>> image(height, std::vector<std::array<int, 3>>(width));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            image[i][j] = {i, j, i + j};
        }
    }

    std::vector<std::vector<int>> result;
    convertToGrayscale(image, height, width, GrayscaleMethod::Average, result);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::vector<std::vector<std::array<int, 3>>> single_pixel = {{{image[i][j][0], image[i][j][1], image[i][j][2]}}};
            std::vector<std::vector<int>> expected_result;
            convertToGrayscale(single_pixel, 1, 1, GrayscaleMethod::Average, expected_result);

            int expected = expected_result[0][0];
            EXPECT_EQ(result[i][j], expected) << "Errore in posizione [" << i << "][" << j << "]";
        }
    }
}


TEST(testPixelPositionMapping, CheckLuminosity) {
    const int height = 5;
    const int width = 5;
    std::vector<std::vector<std::array<int, 3>>> image(height, std::vector<std::array<int, 3>>(width));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            image[i][j] = {i, j, i + j};
        }
    }

    std::vector<std::vector<int>> result;
    convertToGrayscale(image, height, width, GrayscaleMethod::Luminosity, result);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::vector<std::vector<std::array<int, 3>>> single_pixel = {{{image[i][j][0], image[i][j][1], image[i][j][2]}}};
            std::vector<std::vector<int>> expected_result;
            convertToGrayscale(single_pixel, 1, 1, GrayscaleMethod::Luminosity, expected_result);

            int expected = expected_result[0][0];
            EXPECT_EQ(result[i][j], expected) << "Errore in posizione [" << i << "][" << j << "]";
        }
    }
}

TEST(testPixelPositionMapping, CheckLightness) {
    const int height = 5;
    const int width = 5;
    std::vector<std::vector<std::array<int, 3>>> image(height, std::vector<std::array<int, 3>>(width));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            image[i][j] = {i, j, i + j};
        }
    }

    std::vector<std::vector<int>> result;
    convertToGrayscale(image, height, width, GrayscaleMethod::Lightness, result);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::vector<std::vector<std::array<int, 3>>> single_pixel = {{{image[i][j][0], image[i][j][1], image[i][j][2]}}};
            std::vector<std::vector<int>> expected_result;
            convertToGrayscale(single_pixel, 1, 1, GrayscaleMethod::Lightness, expected_result);

            int expected = expected_result[0][0];
            EXPECT_EQ(result[i][j], expected) << "Errore in posizione [" << i << "][" << j << "]";
        }
    }
}

TEST(testPixelPositionMapping, CheckRootMeanSquare) {
    const int height = 5;
    const int width = 5;
    std::vector<std::vector<std::array<int, 3>>> image(height, std::vector<std::array<int, 3>>(width));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            image[i][j] = {i, j, i + j};
        }
    }

    std::vector<std::vector<int>> result;
    convertToGrayscale(image, height, width, GrayscaleMethod::RootMeanSquare, result);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::vector<std::vector<std::array<int, 3>>> single_pixel = {{{image[i][j][0], image[i][j][1], image[i][j][2]}}};
            std::vector<std::vector<int>> expected_result;
            convertToGrayscale(single_pixel, 1, 1, GrayscaleMethod::RootMeanSquare, expected_result);

            int expected = expected_result[0][0];
            EXPECT_EQ(result[i][j], expected) << "Errore in posizione [" << i << "][" << j << "]";
        }
    }
}


// ============================================================================
// This test suite performs METAMORPHIC PROPERTY VALIDATION of the convertToGrayscale()
// function provided in the compiled static library. The purpose is to verify
// that the grayscale conversion behaves consistently under transformations
// that should not affect the output, according to established metamorphic
// relations. These tests help identify potential inconsistencies or bugs in
// the grayscale conversion logic.

// Note: In some tests, a helper function was used to generate random images 
// with valid RGB values in the range [0, 255]. This allowed us to quickly 
// construct meaningful and diverse test cases while ensuring input validity, 
// making it easier to verify metamorphic properties across a wide range of scenarios.

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

TEST(MetamorphicTest, MetamorphicTestAverage){
    const int height = 5;
    const int width = 5;

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

TEST(MetamorphicTests, MetamorphicTestAverage){
    const int height = 5;
    const int width = 5;

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

TEST(MetamorphicTests, MetamorphicTestLightness){
    const int height = 5;
    const int width = 5;

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

TEST(MetamorphicTest, MetamorphicTestRMS){
    const int height = 5;
    const int width = 5;

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
TEST(MetamorphicTest, MetamorphicTestRedChannel){
    const int height = 5;
    const int width = 5;

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
TEST(MetamorphicTest, MetamorphicTestBlueChannel){
    const int height = 5;
    const int width = 5;

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
TEST(MetamorphicTest, MetamorphicTestGreenChannel){
    const int height = 5;
    const int width = 5;

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
