// Plurality.cpp
// Perform a plurality election
// Author: Andy Dang

#include "Plurality.h"
#include <iostream>
#include <cstdlib>
#include <bits/stdc++.h>
#include <algorithm>

// assigns ballots to the corresponding candidates
static void countVotes(Ballots*& ballots, std::vector<Candidate>& candidates) {
    for (int i = 0; i < ballots->getBallotCount(); i++) {
        std::vector<int> currentBallot = ballots->getBallot(i);
        auto voteIterator = std::find(currentBallot.begin(), currentBallot.end(), 1);
        int voteIndex = distance(currentBallot.begin(), voteIterator);
        candidates.at(voteIndex).assignBallot(i);
    }
}

// finds the winners of this plurality election and populates this->winners
static void determineWinners(int& winnerIndex, int& mostVotes, std::vector<Candidate>& losers, std::vector<Candidate>& potentialWinners, std::vector<Candidate>& winners) {
    if (winnerIndex >= 0) {
        mostVotes = losers.at(winnerIndex).getBallotNum();
    }

    // populates the potential winners vector
    if (potentialWinners.size() == 0) {
        while (winnerIndex >= 0 && losers.at(winnerIndex).getBallotNum() == mostVotes) {
            potentialWinners.push_back(losers.at(winnerIndex));
            winnerIndex--;
        }
    }

    // check for ties
    // winners vector should be correct after the code block executes for the final time
    if (potentialWinners.size() > 1) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, potentialWinners.size() - 1);
        int chosenIndex = distrib(gen);
        winners.push_back(potentialWinners.at(chosenIndex));
        potentialWinners.erase(potentialWinners.begin() + chosenIndex);
    } else {
        winners.push_back(potentialWinners.at(0));
        potentialWinners.pop_back();
    }
}

Plurality::Plurality(Ballots* ballots, int seats) : Election(ballots, seats) {
    this->candidates = {};
    for (long unsigned int i = 0; i < this->ballots->getCandidates().size(); i++) {
        this->candidates.push_back(Candidate(this->ballots->getCandidates().at(i)));
    }
    this->winners = {};
    this->losers = {};
}

Plurality::~Plurality() {}

void Plurality::runElection() {
    countVotes(this->ballots, this->candidates);

    // fill and sort losers vector by least votes to most votes
    this->losers = this->candidates;
    std::sort(this->losers.begin(), this->losers.end(), [&] (Candidate& a, Candidate& b) {
        return a.getBallotNum() < b.getBallotNum();
    });

    // a vector of potential winners is needed in case of ties
    std::vector<Candidate> potentialWinners;
    int winnerIndex = this->losers.size() - 1;
    int mostVotes = this->losers.at(winnerIndex).getBallotNum();

    // iterate through the seats, finding a winner for each seat
    for (long unsigned int i = 0; i < std::min(static_cast<long unsigned int>(this->seats), this->candidates.size()); i++) {
        determineWinners(winnerIndex, mostVotes, this->losers, potentialWinners, this->winners);
    }

    // remove winners from losers vector
    this->losers.erase(this->losers.begin() + winnerIndex + 1, this->losers.end());
    if (potentialWinners.size() > 0) {
        this->losers.insert(this->losers.end(), potentialWinners.begin(), potentialWinners.end());
    }
}

void Plurality::displayElectionDetails() {
    std::cout << "Election type: Plurality" << std::endl;
    std::cout << "Number of seats: " << std::to_string(this->seats) << std::endl;
    std::cout << "Number of ballots: " << std::to_string(this->ballots->getBallotCount()) << std::endl;
    std::cout << "Number of candidates: " << std::to_string(this->candidates.size()) << std::endl;
    std::cout << "Winners:" << std::endl;
    for (long unsigned int i = 0; i < this->winners.size(); i++) {
        std::cout << this->winners.at(i).getName() << std::endl;
    }
    std::cout << "Losers:" << std::endl;
    for (long unsigned int i = 0; i < this->losers.size(); i++) {
        std::cout << this->losers.at(i).getName() << std::endl;
    }
    std::cout << "Percentage of votes:" << std::endl;
    for (long unsigned int i = 0; i < this->candidates.size(); i++) {
        double percentage = this->candidates.at(i).getBallotNum() * 100.0 / this->ballots->getBallotCount();
        std::cout << this->candidates.at(i).getName() << " (" << std::fixed << std::setprecision(2) << percentage << "%)" << std::endl;
    }
}

Ballots* Plurality::getBallots() {
    return this->ballots;
}

int Plurality::getSeats() {
    return this->seats;
}

std::vector<Candidate> Plurality::getCandidates() {
    return this->candidates;
}

std::vector<Candidate> Plurality::getWinners() {
    return this->winners;
}

std::vector<Candidate> Plurality::getLosers() {
    return this->losers;
}
