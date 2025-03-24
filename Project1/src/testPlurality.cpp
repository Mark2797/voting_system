// testPlurality.cpp
// Unit testing for Plurality class
// Author: Andy Dang

#include "gtest/gtest.h"

#include "Plurality.h"

// Use optional to delay initialization
// Google test requires a default constructor
#include <optional>

// Elections must be processed in under 5 minutes
#include <chrono>

class PluralityTest : public ::testing::Test {
    protected:
        std::optional<Plurality> plurality;
        std::optional<Ballots> ballots;
        std::vector<std::string> candidates;
        std::vector<std::vector<int>> ballots_plurality;
        std::optional<Plurality> plurality_two;
        std::optional<Ballots> ballots_two;
        std::vector<std::string> candidates_two;
        std::vector<std::vector<int>> ballots_plurality_two;
    
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
        candidates_two = {
            "one",
            "two",
            "three",
            "four",
            "five",
            "six",
            "seven",
            "eight",
            "nine",
            "ten",
        };
        ballots_plurality_two = {};
        for (int i = 0; i < 100000; i = i + 10) {
            ballots_plurality_two.push_back({1, 0, 0, 0, 0, 0, 0, 0, 0, 0});
            ballots_plurality_two.push_back({0, 1, 0, 0, 0, 0, 0, 0, 0, 0});
            ballots_plurality_two.push_back({0, 0, 1, 0, 0, 0, 0, 0, 0, 0});
            ballots_plurality_two.push_back({0, 0, 0, 1, 0, 0, 0, 0, 0, 0});
            ballots_plurality_two.push_back({0, 0, 0, 0, 1, 0, 0, 0, 0, 0});
            ballots_plurality_two.push_back({0, 0, 0, 0, 0, 1, 0, 0, 0, 0});
            ballots_plurality_two.push_back({0, 0, 0, 0, 0, 0, 1, 0, 0, 0});
            ballots_plurality_two.push_back({0, 0, 0, 0, 0, 0, 0, 1, 0, 0});
            ballots_plurality_two.push_back({0, 0, 0, 0, 0, 0, 0, 0, 1, 0});
            ballots_plurality_two.push_back({0, 0, 0, 0, 0, 0, 0, 0, 0, 1});
        }
        ballots_two.emplace(candidates_two, ballots_plurality_two, false);
        plurality_two.emplace(&(ballots_two.value()), 5);
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
    plurality->runElection();
    EXPECT_EQ(plurality->getBallots(), &(ballots.value()));
    EXPECT_EQ(plurality->getSeats(), 2);
    for (long unsigned int i = 0; i < plurality->getCandidates().size(); i++) {
        EXPECT_EQ(plurality->getCandidates().at(i).getName(), candidates.at(i));
    }
    EXPECT_EQ(plurality->getWinners().size(), 2);
    EXPECT_EQ(plurality->getWinners().at(0).getName(), "Bill Jones");
    EXPECT_EQ(plurality->getWinners().at(1).getName(), "Alice Mix");
    EXPECT_EQ(plurality->getLosers().size(), 4);
}

TEST_F(PluralityTest, DisplayElectionDetailsTest) {
    testing::internal::CaptureStdout();
    plurality->runElection();
    plurality->displayElectionDetails();
    std::string correct = "Election type: Plurality\nNumber of seats: 2\nNumber of ballots: 11\nNumber of candidates: 6\nWinners:\nBill Jones\nAlice Mix\nLosers:\nSally Ride\nAhmed Mohamed\nSiyang Xiong\nPreeti Banerjee\nPercentage of votes:\nBill Jones (45.45%)\nAlice Mix (18.18%)\nSally Ride (9.09%)\nAhmed Mohamed (9.09%)\nSiyang Xiong (9.09%)\nPreeti Banerjee (9.09%)\n";
    EXPECT_EQ(testing::internal::GetCapturedStdout(), correct);
}

TEST_F(PluralityTest, ElectionTimeLimit) {
    auto start = std::chrono::high_resolution_clock::now();
    plurality_two->runElection();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    EXPECT_LT(duration.count() / 1000.0, 5.000);
    EXPECT_EQ(plurality_two->getBallots(), &(ballots_two.value()));
    EXPECT_EQ(plurality_two->getSeats(), 5);
    for (long unsigned int i = 0; i < plurality_two->getCandidates().size(); i++) {
        EXPECT_EQ(plurality_two->getCandidates().at(i).getName(), candidates_two.at(i));
    }
    EXPECT_EQ(plurality_two->getWinners().size(), 5);
    EXPECT_EQ(plurality_two->getLosers().size(), 5);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

