// testMunicipal.cpp
// Unit testing for Municipal class
// Author: Andy Dang

#include "gtest/gtest.h"

#include "Municipal.h"
//
// Use optional to delay initialization
// Google test requires a default constructor
#include <optional>

// Elections must be processed in under 5 minutes
#include <chrono>

class MunicipalTest : public ::testing::Test {
    protected:
        std::vector<std::optional<Municipal>> municipal;
        std::vector<std::optional<Ballots>> ballots;
        std::vector<std::vector<std::string>> candidates;
        std::vector<std::vector<std::vector<int>>> ballots_municipal;
        std::optional<Ballots> b;
        std::optional<Municipal> m;

    void SetUp() override {
        candidates = {
            {
            "Bill Jones",
            "Alice Mix",
            "Sally Ride",
            "Ahmed Mohamed",
            "Siyang Xiong",
            "Shana Watters"
            }
        };
        
        ballots_municipal = {
            {
            {1, 1, 1, 0, 0, 0},
            {1, 0, 0, 1, 0, 1},
            {0, 1, 0, 0, 1, 0},
            {0, 0, 1, 0, 1, 0},
            {0, 0, 0, 0, 0, 1},
            {1, 0, 0, 1, 1, 0},
            {0, 0, 1, 1, 0, 0},
            {1, 0, 1, 1, 0, 0},
            {0, 1, 0, 0, 1, 1}
            }
        };

        for (int i = 0; i < 1; i++) {
            b.emplace(candidates.at(i), ballots_municipal.at(i), false);
            m.emplace(&(b.value()), 3);
            ballots.push_back(b);
            municipal.push_back(m);
        }
    }

    void TearDown() override {
        for (int i = 0; i < 1; i++) {
            ballots.at(i).reset();
            municipal.at(i).reset();
        }
    }
};

TEST_F(MunicipalTest, RunElectionTest) {
    municipal.at(0)->runElection();
    EXPECT_EQ(municipal.at(0)->getWinners().size(), 3);
    EXPECT_EQ(municipal.at(0)->getCandidates().at(0).getName(), "Bill Jones");
    EXPECT_EQ(municipal.at(0)->getCandidates().at(0).getBallotNum(), 4);
    EXPECT_EQ(municipal.at(0)->getCandidates().at(1).getName(), "Alice Mix");
    EXPECT_EQ(municipal.at(0)->getCandidates().at(1).getBallotNum(), 3);
    EXPECT_EQ(municipal.at(0)->getCandidates().at(2).getName(), "Sally Ride");
    EXPECT_EQ(municipal.at(0)->getCandidates().at(2).getBallotNum(), 4);
    EXPECT_EQ(municipal.at(0)->getCandidates().at(3).getName(), "Ahmed Mohamed");
    EXPECT_EQ(municipal.at(0)->getCandidates().at(3).getBallotNum(), 4);
    EXPECT_EQ(municipal.at(0)->getCandidates().at(4).getName(), "Siyang Xiong");
    EXPECT_EQ(municipal.at(0)->getCandidates().at(4).getBallotNum(), 4);
    EXPECT_EQ(municipal.at(0)->getCandidates().at(5).getName(), "Shana Watters");
    EXPECT_EQ(municipal.at(0)->getCandidates().at(5).getBallotNum(), 3);
};

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
