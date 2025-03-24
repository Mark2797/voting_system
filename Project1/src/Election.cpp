// Election.cpp
// The abstract class for Plurality and STV algorithm inheritance
// Author: Mark Tsai

#include "Election.h"

Election::Election(Ballots* ballots, int seats) {
    this->ballots = ballots;
    this->seats = seats;
}

Election::~Election() {}
