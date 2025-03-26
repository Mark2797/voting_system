// Candidate.cpp
// Represents one candidate in the election
// Author: Mark Tsai

#include "Candidate.h"
#include <iostream>


Candidate::Candidate(std::string name) {
    this->name = name;
}

Candidate::~Candidate() {}

void Candidate::assignBallot(int ballotID) {
    assignedBallots.push_back(ballotID);
    ballotNum++;
}

void Candidate::removeBallot(int ballotID) {
    // for (unsigned long i = 0; i < assignedBallots.size(); i++) {
    //     if (assignedBallots.at(i) == ballotID) {
    //         assignedBallots.erase(assignedBallots.begin() + i);
    //         //std::cout << "\n[inside candidate]Ballot #" << ballotID << " removed from " << this->name;
    //     }
    // }
    assignedBallots.clear();
    // std::cout << "\n[inside candidate]Candidate " << this->name << "'s new ballots: "; 
    // for (int i = 0; i < assignedBallots.size(); i++) {
    //     std::cout << assignedBallots.at(i) << ", ";
    // }
    ballotNum--;
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
