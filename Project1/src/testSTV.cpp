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
    protected:
        std::optional<STV> stv;
        std::optional<Ballots> ballots;
        std::vector<std::string> candidates;
        std::vector<std::vector<int>> ballots_stv;
    
    void SetUp() override {
        candidates = {
            "Bill Jones",
            "Alice Mix",
            "Sally Ride",
            "Ahmed Mohamed",
            "Siyang Xiong",
            "Preeti Banerjee"
        };
        ballots_stv = {
            {1, 0, 2, 0, 3, 0},
            {3, 2, 1, 4, 6, 5},
            {1, 2, 0, 0, 3, 4},
            {4, 1, 0, 2, 0, 3},
            {1, 2, 3, 4, 5, 6},
            {2, 3, 4, 5, 6, 1},
            {0, 0, 0, 1, 2, 3}
        };
    }
};


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}