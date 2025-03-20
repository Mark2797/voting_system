#include "STV.h"
#include <iostream>

STV::STV(Ballots* ballots, int seats) : Election(ballots, seats) {

}


STV::~STV() {}

void STV::runElection() {
    
    int seatsElected = 0;
    droopQuota = ballots->getBallotCount() / (this->seats + 1);
    int voter_choice = 1;
    
    // Traverse through the vector of ballots
    // for (int ballotId = 0; ballotId < ballots->getBallotCount(); ballotId++) {
    //     std::vector<int> current_ballot = ballots->getBallot(ballotId);
    //     // For each vote in the current ballot
    //     for (unsigned long candidateNum = 0; candidateNum < current_ballot.size(); candidateNum++) {
    //         // If the current vote is a 1, assign the ballot the candidate that recieved it
    //         if (current_ballot.at(candidateNum) == voter_choice) {
    //             candidates.at(candidateNum).assignBallot(ballotId);
    //         }
    //     }
    // }

    // traverse through the vector of ballots
    for (int ballotId = 0; ballotId < ballots->getBallotCount(); ballotId++) {
        std::vector<int> current_ballot = ballots->getBallot(ballotId);
        unsigned long ballotWinner;
        // for each vote in the current ballot
        for (unsigned long candidateNum = 0; candidateNum < current_ballot.size(); candidateNum++) {
            // if the current vote is a 1, assign the ballot the candidate that recieved it
            if (current_ballot.at(candidateNum) == voter_choice) {
                candidates.at(candidateNum).assignBallot(ballotId);
                ballotWinner = candidateNum;
            }
        }

        if (candidates.at(ballotWinner).getAssignedBallots().size() == droopQuota) {
            winners.push_back(candidates.at(ballotWinner));
            seatsElected++;
        }
        else if (candidates.at(ballotWinner).getAssignedBallots().size() > droopQuota) {
            // if the candidate has ballots {1, 3, 5, 7, 9} and only 3 votes are needed, ballots {7, 9} need to have their votes redistributed
            // the for loop should be within this conditional, so that the rest of the vote redistribution doesn't happen until this is solved
            

            // loop through the ballot to find the second (next) choice, and if the next choice already has droop, find the third choice... etc.
        }

        // after the ballot is assigned, see if any of the candidates reached droop because of it... this could be more efficient if we only check
        // // the candidate that recieved the most recent ballot, but C++ is efficient and it'll be okay :)
        // for (unsigned long i = 0; i < candidates.size(); i++) {
        //     if (candidates.at(i).getAssignedBallots().size() == droopQuota) {
        //         winners.push_back(candidates.at(i));
        //         seatsElected++;
        //     }
        //     else if (candidates.at(i).getAssignedBallots().size() > droopQuota) {
        //         // if the candidate has ballots {1, 3, 5, 7, 9} and only 3 votes are needed, ballots {7, 9} need to have their votes redistributed
        //         // the for loop should be within this conditional, so that the rest of the vote redistribution doesn't happen until this is solved
        //     }
        // }

    }
    
    // while there are still seats that need to be filled after all ballots are distributed
    while (seatsElected < seats) {

        // if seatsElected < seats... the case that there are still seats needed to be filled after the first pass
        // find candidate with lowest number of votes - this could be a tie (random)
        // redistribute their votes

        // Traverse through each candidate and display their recieved ballots [testing]
        // for (unsigned long i = 0; i < candidates.size(); i++) {
        //     std::cout << "\nCandidate " << candidates.at(i).getName() << " recieved ballot number: ";
        //     for (unsigned long j = 0; j < candidates.at(i).getAssignedBallots().size(); j++) {
        //         std::cout << candidates.at(i).getAssignedBallots().at(j) << ", ";
        //     }
        // }
    }

    for (unsigned long w = 0; w < seats; w++) {
        std::cout << "\nCandidate " << winners.at(w).getName() << " won with ballot numbers: ";
        for (unsigned long j = 0; j < winners.at(w).getAssignedBallots().size(); j++) {
            std::cout << winners.at(w).getAssignedBallots().at(j) << ", ";
        }
    }

    std::cout << "\nDroop QUOTA: " << droopQuota;

    
}

void STV::displayElectionDetials() {

}
void STV::outputAuditFile() {

}

/*
private:
int droopQuota;
std::vector<std::string> electionProgress;
Ballots* stv_ballots;

*/