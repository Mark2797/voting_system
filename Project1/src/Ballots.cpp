// Ballots.cpp
// Represents the entire information stores in the input ballot file
// Author: Mark Tsai
// Assume perfect play for the csv file

#include <bits/stdc++.h>

#include "Ballots.h"

Ballots::Ballots(std::vector<std::string> candidates, std::vector<std::vector<int>> ballots, bool shuffle) {
    this->candidates = candidates;
    this->ballots = ballots;
    this->shuffle = shuffle;
    candidateCount = candidates.size();
    ballotCount = ballots.size();
    if (shuffle) shuffler();
}

Ballots::~Ballots() {}

void Ballots::shuffler() {
    // Initialize random number generator
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(ballots.begin(), ballots.end(), g);
}

std::vector<int> Ballots::getBallot(int index) {
    return ballots.at(index);
}

std::vector<std::string> Ballots::getCandidates() {
    return candidates;
}

int Ballots::getBallotCount() {
    return ballotCount;
}

int Ballots::getCandidateCount() {
    return candidateCount;
}

bool Ballots::getShuffle() {
    return shuffle;
}
