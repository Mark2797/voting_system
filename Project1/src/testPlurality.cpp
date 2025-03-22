// testPlurality.cpp
// Unit testing for Plurality class
// Author: Andy Dang

#include "gtest/gtest.h"

#include "Plurality.h"

// Use optional to delay initialization
// Google test requires a default constructor
#include <optional>

class PluralityTest : public ::testing::Test {
    protected:
        std::optional<Plurality> plurality;
        std::optional<Ballots> ballots;
        std::vector<std::string> candidates;
        std::vector<std::vector<int>> ballots_plurality;
    
    void SetUp() override {
        candidates = {
            "Bill Jones",
            "Alice Mix",
            "Sally Ride",
            "Ahmed Mohamed",
            "Siyang Xiong",
            "Preeti Banerjee"
        };
        ballots_plurality = {
            {1, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 0},
            {0, 0, 1, 0, 0, 0},
            {0, 0, 0, 1, 0, 0},
            {0, 0, 0, 0, 1, 0},
            {0, 0, 0, 0, 0, 1}
        };
        ballots.emplace(candidates, ballots_plurality, false);
        plurality.emplace(&(ballots.value()), 2);
    }
};

TEST_F(PluralityTest, PluralityConstructorTest) {
    EXPECT_EQ(plurality->getBallots(), &(ballots.value()));
    EXPECT_EQ(plurality->getSeats(), 2);
    for (long unsigned int i = 0; i < plurality->getCandidates().size(); i++) {
        EXPECT_EQ(plurality->getCandidates().at(i).getName(), candidates.at(i));
    }
    EXPECT_EQ(plurality->getWinners().size(), 0);
    EXPECT_EQ(plurality->getLosers().size(), 0);
}

TEST_F(PluralityTest, RunElectionTest) {
    testing::internal::CaptureStdout();
    plurality->runElection();
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "\n");
}

TEST_F(PluralityTest, DisplayElectionDetailsTest) {
    testing::internal::CaptureStdout();
    plurality->displayElectionDetails();
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "\n");
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

