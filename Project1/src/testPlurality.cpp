// testPlurality.cpp
// Unit testing for Plurality class
// Author: Andy Dang

// #include "gtest/gtest.h"

// #include "Plurality.h"

// // Use optional to delay initialization
// // Google test requires a default constructor
// #include <optional>

// class PluralityTest : public ::testing::Test {
//     protected:
//         Plurality* plurality;
//         int seats = 2;
//         std::vector<std::string> shuffleCandidatesVector;
//         std::vector<std::vector<int>> shuffleBallotsVector;
//         std::optional<Ballots> shuffleBallots;
    
//     void SetUp() override {
//         shuffleCandidatesVector = {"Alex", "Steve", "Mario", "Luigi", "Bowser", "Samus"};
//         shuffleBallotsVector = {{1, 2, 3, 0, 5, 4}, {0, 0, 1, 2, 4, 3}, {0, 1, 2, 3, 0, 0}, {2, 3, 4, 1, 0, 5}, {2, 3, 4, 1, 6, 5}, {3, 0, 2, 0, 1, 0}};
//         shuffleBallots.emplace(shuffleCandidatesVector, shuffleBallotsVector, true);
//     }
// };

// TEST_F(PluralityTest, PluralityConstructorTest) {
//     plurality = new Plurality(reinterpret_cast<Ballots*>(&shuffleBallots), seats);
//     EXPECT_EQ(reinterpret_cast<Ballots*>(&shuffleBallots), plurality->ballots);
//     EXPECT_EQ(seats, plurality->seats);
// }

// int main(int argc, char **argv) {
//     testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }

