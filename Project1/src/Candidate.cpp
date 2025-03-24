// Candidate.cpp
// Represents one candidate in the election
// Author: Mark Tsai

#include "Candidate.h"

Candidate::Candidate(std::string name) {
    this->name = name;
}

Candidate::~Candidate() {}

void Candidate::assignBallot(int ballotID) {
    assignedBallots.push_back(ballotID);
    ballotNum++;
}

std::string Candidate::getName() {
    return name;
}

std::vector<int> Candidate::getAssignedBallots() {
    return assignedBallots;
}

int Candidate::getBallotNum() {
    return ballotNum;
}
