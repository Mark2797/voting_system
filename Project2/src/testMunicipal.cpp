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
        std::optional<Municipal> municipal;
        std::optional<Ballots> ballots;
        std::vector<std::string> candidates;
        std::vector<std::vector<int>> ballots_municipal;

        std::optional<Municipal> municipal_two;
        std::optional<Ballots> ballots_two;
        std::vector<std::string> candidates_two;
        std::vector<std::vector<int>> ballots_municipal_two;

        std::optional<Municipal> municipal_three;
        std::optional<Ballots> ballots_three;
        std::vector<std::string> candidates_three;
        std::vector<std::vector<int>> ballots_municipal_three;

        std::optional<Municipal> municipal_four;
        std::optional<Ballots> ballots_four;
        std::vector<std::string> candidates_four;
        std::vector<std::vector<int>> ballots_municipal_four;

        std::optional<Municipal> municipal_five;
        std::optional<Ballots> ballots_five;

        std::optional<Municipal> municipal_six;
        std::optional<Ballots> ballots_six;

    void SetUp() override {
        candidates = {
            "Bill Jones",
            "Alice Mix",
            "Sally Ride",
            "Ahmed Mohamed",
            "Siyang Xiong",
            "Shana Watters"
        };

        candidates_two = {
            "Bill Jones"
        };

        candidates_three = {
            "one",
            "two",
            "three",
            "four",
            "five",
            "six",
            "seven",
            "eight",
            "nine",
            "ten"
        };

        candidates_four = {
            "Chuck Lancaster",
            "Mark Suckerberg",
            "Andrew Hero",
            "Micheal Ashton",
            "Joe Cool"
        };
        
        ballots_municipal = {
            {1, 1, 1, 0, 0, 0},
            {1, 0, 0, 1, 0, 1},
            {0, 1, 0, 0, 1, 0},
            {0, 0, 1, 0, 1, 0},
            {0, 0, 0, 0, 0, 1},
            {1, 0, 0, 1, 1, 0},
            {0, 0, 1, 1, 0, 0},
            {1, 0, 1, 1, 0, 0},
            {0, 1, 0, 0, 1, 1}
        };

        ballots_municipal_two = {
            {1}
        };

        for (int i = 0; i < 100000; i++) {
            ballots_municipal_three.push_back({1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
        }

        ballots_municipal_four = {
            {1, 0, 0, 0, 0},
            {1, 1, 0, 0, 0},
            {1, 1, 1, 0, 0},
            {1, 1, 1, 1, 0},
            {1, 1, 1, 1, 1}
        };

        ballots.emplace(candidates, ballots_municipal, false);
        municipal.emplace(&(ballots.value()), 3);

        ballots_two.emplace(candidates_two, ballots_municipal_two, false);
        municipal_two.emplace(&(ballots_two.value()), 1);

        ballots_three.emplace(candidates_three, ballots_municipal_three, false);
        municipal_three.emplace(&(ballots_three.value()), 10);

        ballots_four.emplace(candidates_four, ballots_municipal_four, false);
        municipal_four.emplace(&(ballots_four.value()), 1);

        ballots_five.emplace(candidates_two, ballots_municipal_two, false);
        municipal_five.emplace(&(ballots_two.value()), 21);

        ballots_six.emplace(candidates, ballots_municipal, false);
        municipal_six.emplace(&(ballots.value()), 2);
    }
};

TEST_F(MunicipalTest, MunicipalConstructorTest) {
    EXPECT_EQ(municipal->getBallots(), &(ballots.value()));
    EXPECT_EQ(municipal->getWinners().size(), 0);
    EXPECT_EQ(municipal->getLosers().size(), 0);
    EXPECT_EQ(municipal->getSeats(), 3);
    EXPECT_EQ(municipal->getCandidates().size(), 6);
    EXPECT_EQ(municipal->getCandidates().at(0).getName(), "Bill Jones");
    EXPECT_EQ(municipal->getCandidates().at(1).getName(), "Alice Mix");
    EXPECT_EQ(municipal->getCandidates().at(2).getName(), "Sally Ride");
    EXPECT_EQ(municipal->getCandidates().at(3).getName(), "Ahmed Mohamed");
    EXPECT_EQ(municipal->getCandidates().at(4).getName(), "Siyang Xiong");
    EXPECT_EQ(municipal->getCandidates().at(5).getName(), "Shana Watters");
    EXPECT_EQ(0, 1);
};

TEST_F(MunicipalTest, RunElectionTest) {
    // project writeup election
    municipal->runElection();
    EXPECT_EQ(municipal->getWinners().size(), 3);
    EXPECT_EQ(municipal->getLosers().size(), 3);
    EXPECT_EQ(municipal->getSeats(), 3);
    EXPECT_EQ(municipal->getCandidates().size(), 6);
    EXPECT_EQ(municipal->getCandidates().at(0).getName(), "Bill Jones");
    EXPECT_EQ(municipal->getCandidates().at(0).getBallotNum(), 4);
    EXPECT_EQ(municipal->getCandidates().at(1).getName(), "Alice Mix");
    EXPECT_EQ(municipal->getCandidates().at(1).getBallotNum(), 3);
    EXPECT_EQ(municipal->getCandidates().at(2).getName(), "Sally Ride");
    EXPECT_EQ(municipal->getCandidates().at(2).getBallotNum(), 4);
    EXPECT_EQ(municipal->getCandidates().at(3).getName(), "Ahmed Mohamed");
    EXPECT_EQ(municipal->getCandidates().at(3).getBallotNum(), 4);
    EXPECT_EQ(municipal->getCandidates().at(4).getName(), "Siyang Xiong");
    EXPECT_EQ(municipal->getCandidates().at(4).getBallotNum(), 4);
    EXPECT_EQ(municipal->getCandidates().at(5).getName(), "Shana Watters");
    EXPECT_EQ(municipal->getCandidates().at(5).getBallotNum(), 3);

    // small election: one seat
    municipal_two->runElection();
    EXPECT_EQ(municipal_two->getWinners().size(), 1);
    EXPECT_EQ(municipal_two->getLosers().size(), 0);
    EXPECT_EQ(municipal_two->getSeats(), 1);
    EXPECT_EQ(municipal_two->getCandidates().size(), 1);
    EXPECT_EQ(municipal_two->getCandidates().at(0).getName(), "Bill Jones");
    EXPECT_EQ(municipal_two->getCandidates().at(0).getBallotNum(), 1);

    // small election: multiple seats
    municipal_five->runElection();
    EXPECT_EQ(municipal_five->getWinners().size(), 1);
    EXPECT_EQ(municipal_five->getLosers().size(), 0);
    EXPECT_EQ(municipal_five->getSeats(), 21);
    EXPECT_EQ(municipal_five->getCandidates().size(), 1);
    EXPECT_EQ(municipal_five->getCandidates().at(0).getName(), "Bill Jones");
    EXPECT_EQ(municipal_five->getCandidates().at(0).getBallotNum(), 1);

    // normal election
    municipal_four->runElection();
    EXPECT_EQ(municipal_four->getWinners().size(), 1);
    EXPECT_EQ(municipal_four->getLosers().size(), 4);
    EXPECT_EQ(municipal_four->getSeats(), 1);
    EXPECT_EQ(municipal_four->getCandidates().size(), 5);
    EXPECT_EQ(municipal_four->getWinners().at(0).getName(), "Chuck Lancaster");
    EXPECT_EQ(municipal_four->getWinners().at(0).getBallotNum(), 5);

    // election with tie
    municipal_six->runElection();
    EXPECT_EQ(municipal_six->getWinners().size(), 2);
    EXPECT_EQ(municipal_six->getLosers().size(), 4);
    EXPECT_EQ(municipal_six->getSeats(), 2);
    EXPECT_EQ(municipal->getCandidates().size(), 6);
    EXPECT_EQ(municipal->getWinners().at(0).getBallotNum(), 4);
    EXPECT_EQ(municipal->getWinners().at(1).getBallotNum(), 4);

    EXPECT_EQ(0, 1);
};

TEST_F(MunicipalTest, DisplayElectionDetailsTest) {
    testing::internal::CaptureStdout();
    municipal->runElection();
    std::string correct = "Election type: Municipal\nNumber of seats: 3\nNumber of ballots: 9\nNumber of candidates: 6\n";
    std::string result = testing::internal::GetCapturedStdout().substr(0, 89);
    EXPECT_EQ(result, correct);
    EXPECT_EQ(0, 1);
};

TEST_F(MunicipalTest, ElectionTimeLimit) {
    auto start = std::chrono::high_resolution_clock::now();
    municipal_three->runElection();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    EXPECT_LT(duration.count() / 1000.0, 5.000);
    EXPECT_EQ(0, 1);
};

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
