#include "STV.h"
#include <iostream>

STV::STV(Ballots* ballots, int seats) : Election(ballots, seats) {

}


STV::~STV() {}

void STV::runElection() {
    
    int seatsElected = 0;
    droopQuota = (ballots->getBallotCount() / (this->seats + 1)) + 1;
    std::cout << "\nDroop QUOTA: " << droopQuota;

    int voter_choice = 1;

    // traverse through the vector of ballots [first pass]
    for (int ballotId = 0; ballotId < ballots->getBallotCount(); ballotId++) {
        std::vector<int> current_ballot = ballots->getBallot(ballotId);
        unsigned long ballotWinner;
        
        // for each vote in the current ballot
        for (unsigned long candidateNum = 0; candidateNum < current_ballot.size(); candidateNum++) {
            
            // if the current vote is a 1, assign the ballot the candidate that recieved it
            if (current_ballot.at(candidateNum) == voter_choice && candidates.at(candidateNum).getAssignedBallots().size() < droopQuota) {
                candidates.at(candidateNum).assignBallot(ballotId);
                
                // if assigning the ballot means that the candidate hits droop quota, then immediately add them to the winners list
                if (candidates.at(candidateNum).getAssignedBallots().size() == droopQuota) {
                    winners.push_back(candidates.at(candidateNum));
                    seatsElected++;
                }
                ballotWinner = candidateNum;
            }
            
            // if the ballot was going to be assigned to a candidate at droop quota, instead give the ballot to the next option
            else if (current_ballot.at(candidateNum) == voter_choice && candidates.at(candidateNum).getAssignedBallots().size() >= droopQuota) {
                std::cout << "\nCandidate: " << candidates.at(candidateNum).getName() << " has " << candidates.at(candidateNum).getAssignedBallots().size() << " votes, reassigning ballot #" << ballotId;
                for (int currentPick = voter_choice + 1; currentPick < current_ballot.size(); currentPick++) {
                    std::cout << "\nFinding the #" << currentPick << " choice for ballotID: " << ballotId;
                    bool reassigned = false;
                    for (unsigned long candidateNum = 0; candidateNum < current_ballot.size(); candidateNum++) {
                        // if the current vote is a 1, assign the ballot the candidate that recieved it
                        if (current_ballot.at(candidateNum) == currentPick && candidates.at(candidateNum).getAssignedBallots().size() < droopQuota) {
                            candidates.at(candidateNum).assignBallot(ballotId);
                            ballotWinner = candidateNum;
                            std::cout << "\nBallot " << ballotId << " was reassigned to the candidate: " << candidates.at(candidateNum).getName();
                            
                            // if assigning the ballot means that the candidate hits droop quota, then immediately add them to the winners list
                            if (candidates.at(candidateNum).getAssignedBallots().size() == droopQuota) {
                                winners.push_back(candidates.at(candidateNum));
                                seatsElected++;
                            }                                    

                            reassigned = true;
                            break;
                        }
                    }
                    if (reassigned == true) {
                        break;
                    }
                }
            }
        }

            // for (unsigned long candidateNum = 0; candidateNum < current_ballot.size(); candidateNum++) {
            //     // if the current vote is a 1, assign the ballot the candidate that recieved it
            //     if (current_ballot.at(candidateNum) == 2 && candidates.at(candidateNum).getAssignedBallots().size() < droopQuota) {
            //         candidates.at(candidateNum).assignBallot(ballotId);
            //         std::cout << "\nBallot " << ballotId << " was reassigned to the candidate: " << candidates.at(candidateNum).getName();
            //         //reassigned = true;
            //         break;
            //     }
            // }

            // loop through the ballot to find the second (next) choice, and if the next choice already has droop, find the third choice... etc.
        }

    
    // after all of the votes are distributed, if there are still seats that need to be filled
    // while (seatsElected < seats) {

    //     // if seatsElected < seats... the case that there are still seats needed to be filled after the first pass
    //     // find candidate with lowest number of votes - this could be a tie (random)
    //     // redistribute their votes

    //     // Traverse through each candidate and display their recieved ballots [testing]
    //     // for (unsigned long i = 0; i < candidates.size(); i++) {
    //     //     std::cout << "\nCandidate " << candidates.at(i).getName() << " recieved ballot number: ";
    //     //     for (unsigned long j = 0; j < candidates.at(i).getAssignedBallots().size(); j++) {
    //     //         std::cout << candidates.at(i).getAssignedBallots().at(j) << ", ";
    //     //     }
    //     // }
    // }
    
    if (winners.size() < seats) {
        std::cout << "\n" << winners.size() << " candidates were elected, but we needed " << seats << ", here are the winners: ";
    }

    for (unsigned long w = 0; w < winners.size(); w++) {
        std::cout << "\nCandidate " << winners.at(w).getName() << " won with ballot numbers: ";
        for (unsigned long j = 0; j < winners.at(w).getAssignedBallots().size(); j++) {
            std::cout << winners.at(w).getAssignedBallots().at(j) << ", ";
        }
    }

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