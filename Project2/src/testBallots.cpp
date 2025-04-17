// testBallots.cpp
// Unit testing for Ballots class
// Author: Mark Tsai

#include "gtest/gtest.h"

#include "Ballots.h"

// Use optional to delay initialization
// Google test requires a default constructor
#include <optional>

class BallotTest : public ::testing::Test {
    protected:
        std::vector<std::string> notShuffleCandidatesVector;
        std::vector<std::vector<int>> notShuffleBallotsVector;
        std::optional<Ballots> notShuffleBallots;
        std::vector<std::string> shuffleCandidatesVector;
        std::vector<std::vector<int>> shuffleBallotsVector;
        std::optional<Ballots> shuffleBallots;
    
    void SetUp() override {
        notShuffleCandidatesVector = {"Andy", "Manan", "Mark", "Micheal"};
        notShuffleBallotsVector = {{1, 2, 3, 0}, {0, 0, 1, 2}, {0, 1, 2, 3}, {2, 3, 4, 1}};
        notShuffleBallots.emplace(notShuffleCandidatesVector, notShuffleBallotsVector, false);
        shuffleCandidatesVector = {"Alex", "Steve", "Mario", "Luigi", "Bowser", "Samus"};
        shuffleBallotsVector = {{1, 2, 3, 0, 5, 4}, {0, 0, 1, 2, 4, 3}, {0, 1, 2, 3, 0, 0}, {2, 3, 4, 1, 0, 5}, {2, 3, 4, 1, 6, 5}, {3, 0, 2, 0, 1}};
        shuffleBallots.emplace(shuffleCandidatesVector, shuffleBallotsVector, true);
    }
};

TEST_F(BallotTest, ConstructorNoShuffleTest) {   
    std::vector<std::vector<int>> ballots_temp;
    for (int i = 0; i < notShuffleBallots->getBallotCount(); i++) {
        ballots_temp.push_back(notShuffleBallots->getBallot(i));
    }
    EXPECT_EQ(ballots_temp, notShuffleBallotsVector);
}

TEST_F(BallotTest, ConstructorShuffleTest) {
    std::vector<std::vector<int>> ballots_temp;
    for (int i = 0; i < shuffleBallots->getBallotCount(); i++) {
        ballots_temp.push_back(shuffleBallots->getBallot(i));
    }
    EXPECT_NE(ballots_temp, shuffleBallotsVector);
}

TEST_F(BallotTest, GetBallotTest) {   
    for (int i = 0; i < notShuffleBallots->getBallotCount(); i++) {
        EXPECT_EQ(notShuffleBallots->getBallot(i), notShuffleBallotsVector.at(i));
    }
    EXPECT_THROW(notShuffleBallots->getBallot(-10), std::out_of_range);
    EXPECT_THROW(notShuffleBallots->getBallot(notShuffleBallots->getBallotCount()), std::out_of_range);
    EXPECT_THROW(notShuffleBallots->getBallot(notShuffleBallots->getBallotCount() + 1000), std::out_of_range);
}

TEST_F(BallotTest, GetCandidateTest) {   
    EXPECT_EQ(notShuffleBallots->getCandidates(), notShuffleCandidatesVector);
    EXPECT_EQ(shuffleBallots->getCandidates(), shuffleCandidatesVector);
}

TEST_F(BallotTest, GetBallotCountTest) {   
    EXPECT_EQ(notShuffleBallots->getBallotCount(), notShuffleBallotsVector.size());
    EXPECT_EQ(shuffleBallots->getBallotCount(), shuffleBallotsVector.size());
}

TEST_F(BallotTest, GetCandidateCountTest) {   
    EXPECT_EQ(notShuffleBallots->getCandidateCount(), notShuffleCandidatesVector.size());
    EXPECT_EQ(shuffleBallots->getCandidateCount(), shuffleCandidatesVector.size());
}

TEST_F(BallotTest, GetShuffleTest) {   
    EXPECT_EQ(notShuffleBallots->getShuffle(), false);
    EXPECT_EQ(shuffleBallots->getShuffle(), true);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

