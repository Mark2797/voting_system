// testSTV.cpp
// Unit testing for STV class
// Author: Michael Dunn

#include "gtest/gtest.h"

#include "STV.h"

// Use optional to delay initialization
// Google test requires a default constructor
#include <optional>

// Elections must be processed in under 5 minutes
#include <chrono>

class STVTest : public ::testing::Test {
};

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}