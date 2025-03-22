#include "STV.h"
#include <iostream>
#include <climits>
#include <bits/stdc++.h>


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
                                // maybe remove them from the candidates- HOWEVER, because the candidates vector doesn't have ids, the voter's choices require them to still be in the vector
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
    }

    
    // after all of the votes are distributed, if there are still seats that need to be filled
    while (seatsElected < seats) {

        // before each redistribution round, traverse through each candidate and display their recieved ballots [testing]
        for (unsigned long i = 0; i < candidates.size(); i++) {
            std::cout << "\nCandidate " << candidates.at(i).getName() << " recieved ballot number: ";
            for (unsigned long j = 0; j < candidates.at(i).getAssignedBallots().size(); j++) {
                std::cout << candidates.at(i).getAssignedBallots().at(j) << ", ";
            }
        }

        // create a vector to store candidates that are tied for lowest votes, and keep track of the current lowest vote number with lowestVotes
        std::vector<Candidate*> lowestVoteCandidates;
        unsigned long lowestVotes = LONG_MAX;
        
        // traverse through the candidate list and find the candidate with the lowest number of ballots
        for (unsigned long i = 0; i < candidates.size(); i++) {
            //std::cout << "\nCandidate " << candidates.at(i).getName() << " recieved " << candidates.at(i).getAssignedBallots().size() << " number of ballots";

            // if after the first pass a candidate didn't recieve any votes, add them to the losers list... their votes cannot be redistributed because they didn't get any
            if (candidates.at(i).getAssignedBallots().size() == 0) {
                losers.push_back(candidates.at(i));
                std::cout << "\nAdding " << candidates.at(i).getName() << " to the losers list because they recieved " << candidates.at(i).getAssignedBallots().size() << " ballots.";
            }
            
            // if there's a tie in losers, add them to the tie list
            else if (candidates.at(i).getAssignedBallots().size() == lowestVotes) {
                lowestVoteCandidates.push_back(&candidates.at(i));
                lowestVotes = candidates.at(i).getAssignedBallots().size(); //technically not needed bc lowestvotes will already be this value but :)
            }
            
            // If a 'new' losing amount of ballots is achieved, then clear out the losingCandidates vector and add the NEW loser
            else if (candidates.at(i).getAssignedBallots().size() < lowestVotes && candidates.at(i).getAssignedBallots().size() > 0) {
                lowestVoteCandidates.clear();
                lowestVoteCandidates.push_back(&candidates.at(i));
                lowestVotes = candidates.at(i).getAssignedBallots().size();
            }            
        }        

        Candidate* redistributionLoser;
        
        // if there were more than one 'losers' with the same number of losing votes (not including candidates that didn't recieve ANY votes)
        if (lowestVoteCandidates.size() > 1) {
            // tied losers case... random selection, the loser that ends up in the first slot of the losers list is the one that has their votes redistributed
            std::cout << "\nThere was a tie between " << lowestVoteCandidates.at(0)->getName() << " and " << lowestVoteCandidates.at(1)->getName(); 
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(lowestVoteCandidates.begin(), lowestVoteCandidates.end(), g);
            std::cout << "\nLoser was randomly selected to be: " << lowestVoteCandidates.at(0)->getName();
            break;
        }

        
        std::cout << "\nThe candidate with the lowest votes was: " << lowestVoteCandidates.at(0)->getName() << ", they recieved ballot: " << lowestVoteCandidates.at(0)->getAssignedBallots().size() << " ballots. Redistrbuting...";
        redistributionLoser = lowestVoteCandidates.at(0);            
        
        // for each of the losing candidates ballots
        for (int i = 0; i < redistributionLoser->getAssignedBallots().size(); i++) {

            // find what number choice we're on, in the case that the ballot has already been assigned to its 2nd/3rd/etc choice or if the 2nd/3rd/etc choice has already hit Droop
            int currentBallotID = redistributionLoser->getAssignedBallots().at(i);
            redistributionLoser->removeBallot(currentBallotID);
            std::vector<int> currentBallot = ballots->getBallot(currentBallotID);
            int loserChoice;
            for (int j = 0; j < currentBallot.size(); j++) {
                if (candidates.at(j).getName() == redistributionLoser->getName()) {
                    loserChoice = currentBallot.at(j);
                }
            }
            std::cout << "\nCandidate " << redistributionLoser->getName() << " was ballot #" << currentBallotID << "'s " << loserChoice << "st pick";
            
            // choice #, i.e. looking for the 2nd choice after the first choice has lost
            for (int currentPick = loserChoice + 1; currentPick < currentBallot.size() + 1; currentPick++) {
                std::cout << "\n[inside 2nd round]Finding the #" << currentPick << " choice for ballotID: " << currentBallotID;
                bool reassigned = false;
                
                // same code as above for assigning the vote to the current pick
                for (unsigned long candidateNum = 0; candidateNum < currentBallot.size(); candidateNum++) {
                    
                    // if the current vote is a 1, assign the ballot the candidate that recieved it
                    if (currentBallot.at(candidateNum) == currentPick && candidates.at(candidateNum).getAssignedBallots().size() < droopQuota) {
                        candidates.at(candidateNum).assignBallot(currentBallotID);
                        std::cout << "\nBallot " << currentBallotID << " was reassigned to the candidate: " << candidates.at(candidateNum).getName();
                        
                        // if assigning the ballot means that the candidate hits droop quota, then immediately add them to the winners list
                        if (candidates.at(candidateNum).getAssignedBallots().size() == droopQuota) {
                            // maybe remove them from the candidates- HOWEVER, because the candidates vector doesn't have ids, the voter's choices require them to still be in the vector
                            std::cout << "\nAssigning ballot #" << currentBallotID << " means that " << candidates.at(candidateNum).getName() << " hit Droop Quota... adding them to winners list.";
                            winners.push_back(candidates.at(candidateNum));
                            seatsElected++;
                        }                                    

                        // once the ballot is reassigned, break out of the assignment loop
                        reassigned = true;
                        break;
                    }
                }
                
                if (reassigned == true) {
                    break;
                }
            }
        }

        // traverse through each candidate and display their recieved ballots after redistribution [testing]
        for (unsigned long i = 0; i < candidates.size(); i++) {
            std::cout << "\nCandidate " << candidates.at(i).getName() << " recieved ballot number: ";
            for (unsigned long j = 0; j < candidates.at(i).getAssignedBallots().size(); j++) {
                std::cout << candidates.at(i).getAssignedBallots().at(j) << ", ";
            }
        }
    }
    
    // code has been created to not hit this conditional statment, but here for testing [testing]
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