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

Election::~Election() {}
