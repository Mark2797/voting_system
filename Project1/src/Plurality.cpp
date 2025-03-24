// Plurality.cpp
// Perform a plurality election
// Author: Andy Dang

#include "Plurality.h"
#include <iostream>
#include <cstdlib>
#include <bits/stdc++.h>
#include <vector>

Plurality::Plurality(Ballots* ballots, int seats) : Election(ballots, seats) {
    // std::cout << "1p" << std::endl;
    this->candidates = {};
    // std::cout << "2p" << std::endl;
    for (long unsigned int i = 0; i < this->ballots->getCandidates().size(); i++) {
        this->candidates.push_back(Candidate(this->ballots->getCandidates().at(i)));
    }
    // std::cout << "3p" << std::endl;
    this->winners = {};
    // std::cout << "4p" << std::endl;
    this->losers = {};
    // std::cout << "5p" << std::endl;
}

Plurality::~Plurality() {
    // std::cout << "1d" << std::endl;
    // if (this->ballots) {
    //     std::cout << "1.1d" << std::endl;
    //     delete this->ballots;
    //     std::cout << "1.2d" << std::endl;
    // } else {
    //     std::cout << "1.3d" << std::endl;
    // }
    // std::cout << "2d" << std::endl;
}

void Plurality::runElection() {
    // std::cout << "6p" << std::endl;
    // iterate through the ballots
    // find the vote that each ballot is assigned to
    // add the vote to the corresponding candidate's vote count
    for (int i = 0; i < this->ballots->getBallotCount(); i++) {
        // std::cout << "6.1p" << std::endl;
        std::vector<int> currentBallot = this->ballots->getBallot(i);
        // (this->ballots->getBallot(i).begin(), this->ballots->getBallot(i).end());
        // std::cout << "6.2p" << std::endl;
        auto voteIterator = std::find(currentBallot.begin(), currentBallot.end(), 1);
        // std::cout << "6.3p" << std::endl;
        int voteIndex = distance(currentBallot.begin(), voteIterator);
        // std::cout << "6.4p" << std::endl;
        this->candidates.at(voteIndex).assignBallot(i);
        // std::cout << "6.5p" << std::endl;
        // std::cout << "6.6p" << std::endl;
    }
    // std::cout << "7p" << std::endl;

    // fill and sort losers vector
    this->losers = this->candidates;
    std::sort(this->losers.begin(), this->losers.end(), [&] (Candidate& a, Candidate& b) {
        return a.getBallotNum() < b.getBallotNum();
    });
    // std::cout << "8p" << std::endl;
    // a vector of potential winners is needed in case of ties
    std::vector<Candidate> potentialWinners;
    int winnerIndex = this->losers.size() - 1;
    int mostVotes = this->losers.at(winnerIndex).getBallotNum();
    // std::cout << "9p" << std::endl;
    // iterate thorugh the seats
    for (int i = 0; i < this->seats; i++) {
        // std::cout << "9.1p" << std::endl;
        if (winnerIndex >= 0) {
            mostVotes = this->losers.at(winnerIndex).getBallotNum();
        }
        // std::cout << "9.2p" << std::endl;
        // populates the potential winners vector
        if (potentialWinners.size() == 0) {
            // std::cout << "9.2.1p" << std::endl;
            while (winnerIndex >= 0 && this->losers.at(winnerIndex).getBallotNum() == mostVotes) {
                // std::cout << "9.2.1.1p" << std::endl;
                potentialWinners.push_back(this->losers.at(winnerIndex));
                // std::cout << "9.2.1.2p" << std::endl;
                winnerIndex--;
                // std::cout << "9.2.1.3p" << std::endl;
            }
            // std::cout << "9.2.2p" << std::endl;
        }
        // std::cout << "9.3p" << std::endl;
        // check for ties
        // winners vector should be correct after the code block executes for the final time
        if (potentialWinners.size() > 1) {
            // std::cout << "9.3.1p" << std::endl;
            std::random_device rd;
            // std::cout << "9.3.2p" << std::endl;
            std::mt19937 gen(rd());
            // std::cout << "9.3.3p" << std::endl;
            std::uniform_int_distribution<> distrib(0, potentialWinners.size() - 1);
            // std::cout << "9.3.4p" << std::endl;
            int chosenIndex = distrib(gen);
            // std::cout << "9.3.5p" << std::endl;
            this->winners.push_back(potentialWinners.at(chosenIndex));
            // std::cout << "9.3.6p" << std::endl;
            potentialWinners.erase(potentialWinners.begin() + chosenIndex);
            // std::cout << "9.3.7p" << std::endl;
        } else {
            // std::cout << "9.3.8p" << std::endl;
            this->winners.push_back(potentialWinners.at(0));
            // std::cout << "9.3.9p" << std::endl;
            potentialWinners.pop_back();
            // std::cout << "9.3.0p" << std::endl;
        }
    }
    // std::cout << "10p" << std::endl;
    // update losers vector
    this->losers.erase(this->losers.begin() + winnerIndex + 1, this->losers.end());
    if (potentialWinners.size() > 0) {
        this->losers.insert(this->losers.end(), potentialWinners.begin(), potentialWinners.end());
    }
    // std::cout << "11p" << std::endl;
    // int mostVotes = 0;
    // std::vector<Candidate> winner;
    // for (int j = 0; j < this->candidates.size(); j++) {
    //     Candidate currentCandidate = this->candidates.at(j);
    //     if (currentCandidate->getBallotNum() > mostVotes) {
    //         winner.clear();
    //         winner.push_back(currentCandidate);
    //         mostVotes = currentCandidate->getBallotNum();
    //     } else if (currentCandidate->getBallotNum() == mostVotes) {
    //         winner.push_back(currentCandidate);
    //     }
    // }
    // this->winners.push_back(winner.at(0));
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
