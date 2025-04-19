// Election.cpp
// The abstract class for Plurality and STV algorithm inheritance
// Author: Mark Tsai

#include "Election.h"
#include <iostream>


Election::Election(Ballots* ballots, int seats) {
    this->ballots = ballots;
    this->seats = seats;

    for (int i = 0; i < ballots->getCandidateCount(); i++) {        
        // create a new Candidate object to hold their assigned ballots based on the names in the candidate vector
        Candidate newcandidate = Candidate(ballots->getCandidates().at(i));
        this->candidates.push_back(newcandidate);
    }
}

Ballots* Election::getBallots() {
    return this->ballots;
}

int Election::getSeats() {
    return this->seats;
}

std::vector<Candidate> Election::getCandidates() {
    return this->candidates;
}

std::vector<Candidate> Election::getWinners() {
    return this->winners;
}

std::vector<Candidate> Election::getLosers() {
    return this->losers;
}

Election::~Election() {}
