// testCandidate.cpp
// Unit testing for Candidate class
// Author: Mark Tsai

#include "gtest/gtest.h"

#include "Candidate.h"

// Use optional to delay initialization
// Google test requires a default constructor
#include <optional>

class CandidateTest : public ::testing::Test {
    protected:
        std::string name = "Manan";
        std::vector<std::vector<int>> ballotsVector;
        std::optional<Candidate> oneCandidate;
        std::vector<int> idVector;

    void SetUp() override {
        ballotsVector = {{1, 2, 3, 0}, {0, 0, 1, 2}, {0, 1, 2, 3}, {2, 3, 4, 1}};
        oneCandidate.emplace(name);
        idVector = {0, 1, 2, 3};
    }
};

TEST_F(CandidateTest, AssignBallotTest) {
    std::vector<int> id_temp;
    for (int i = 0; i < static_cast<int>(ballotsVector.size()); i++) {
        oneCandidate->assignBallot(i);
        id_temp.push_back(i);
        EXPECT_EQ(oneCandidate->getAssignedBallots(), id_temp);
    }
}

TEST_F(CandidateTest, GetAssignBallotTest) {
    for (int i = 0; i < static_cast<int>(ballotsVector.size()); i++) {
        oneCandidate->assignBallot(i);
    }
    EXPECT_EQ(oneCandidate->getAssignedBallots(), idVector);
}

TEST_F(CandidateTest, GetBallotNumTest) {
    for (int i = 0; i < static_cast<int>(ballotsVector.size()); i++) {
        oneCandidate->assignBallot(i);
    }
    EXPECT_EQ(oneCandidate->getBallotNum(), idVector.size());
}

TEST_F(CandidateTest, GetNameTest) {
    EXPECT_EQ(oneCandidate->getName(), name);
}



int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

