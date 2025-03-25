#include "STV.h"
#include <iostream>
#include <climits>
#include <bits/stdc++.h>
#include <fstream>

static std::string auditFileName;
static std::string outputString;
static std::ofstream file;

static void writeVotesToAudit(std::vector<std::string>& electionProgress, std::vector<Candidate>& candidates) {

    for (unsigned long i = 0; i < candidates.size(); i++) {
        outputString = "\n" + candidates.at(i).getName() + ",";

        for (unsigned long j = 0; j < candidates.at(i).getAssignedBallots().size(); j++) {
            //std::cout << candidates.at(i).getAssignedBallots().at(j) << ", ";
            outputString += std::to_string(candidates.at(i).getAssignedBallots().at(j)) + ",";
        }
        electionProgress.push_back(outputString);
    }
}

static void displayWinnersLosers(std::vector<std::string>& electionProgress, std::vector<Candidate>& winners, std::vector<Candidate>& losers) {
    outputString = "\nWinners: ";
    for (unsigned long i = 0; i < winners.size(); i++) {
        outputString += winners.at(i).getName() + ",";
    }
    electionProgress.push_back(outputString);

    outputString = "\nLosers: ";
    for (unsigned long i = 0; i < losers.size(); i++) {
        outputString += losers.at(i).getName() + ",";
    }
    electionProgress.push_back(outputString);
}

static int findLoserChoice(std::vector<Candidate>& candidates, std::vector<int>& currentBallot, Candidate*& redistributionLoser) {
    int loserChoice;
    for (int j = 0; j < static_cast<int>(currentBallot.size()); j++) {
        if (candidates.at(j).getName() == redistributionLoser->getName()) {
            loserChoice = currentBallot.at(j);
        }
    }
    return loserChoice;
}

static bool notLoser(Candidate& candidate, std::vector<Candidate>& losers) {
    for (unsigned int i = 0; i < losers.size(); i++) {
        if (losers.at(i).getName() == candidate.getName()) {
            return false;
        }
    }
    return true;
}

static Candidate* tieBreaker(std::vector<std::string>& electionProgress, std::vector<Candidate*>& lowestVoteCandidates) {

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(lowestVoteCandidates.begin(), lowestVoteCandidates.end(), g);
    
    Candidate* redistributionLoser = lowestVoteCandidates.at(0);
        
    outputString = "\nThere was a tie between " + lowestVoteCandidates.at(0)->getName() + " and " + lowestVoteCandidates.at(1)->getName(); 
    outputString += "\nLoser was randomly selected to be: " + lowestVoteCandidates.at(0)->getName();

    return redistributionLoser;
} 

static Candidate* findLoser(std::vector<std::string>& electionProgress, std::vector<Candidate>& candidates, std::vector<Candidate>& losers) {
    // create a vector to store candidates that are tied for lowest votes, and keep track of the current lowest vote number with lowestVotes    
    std::vector<Candidate*> lowestVoteCandidates;
    unsigned long lowestVotes = LONG_MAX;
    
    // traverse through the candidate list and find the candidate with the lowest number of ballots
    for (unsigned long i = 0; i < candidates.size(); i++) {

        // if after the first pass a candidate didn't recieve any votes, add them to the losers list... their votes cannot be redistributed because they didn't get any
        if (candidates.at(i).getAssignedBallots().size() == 0) {
            
            // before adding the candidate to the losers vector, check if they're already there
            if (notLoser(candidates.at(i), losers) == true) {
                losers.push_back(candidates.at(i));
                outputString = "\nAdding " + candidates.at(i).getName() + " to the losers list because they recieved " + std::to_string(candidates.at(i).getAssignedBallots().size()) + " ballots.";
                electionProgress.push_back(outputString);
            }
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

    Candidate* redistributionLoser = lowestVoteCandidates.at(0);
        
    // if there were more than one 'losers' with the same number of losing votes (not including candidates that didn't recieve ANY votes)
    if (lowestVoteCandidates.size() > 1) {
        redistributionLoser = tieBreaker(electionProgress, lowestVoteCandidates);
    }

    return redistributionLoser;
}

static void promptAuditFilename() {
    std::cout << "Please enter a filename for the audit file:" << std::endl;
    std::cout << "Please also include the .csv extension:" << std::endl;
    std::cin >> auditFileName;
    while(true) {
        file.open(auditFileName);
        if (std::cin.fail()) {
            std::cout << "Invalid input!" << std::endl;
            std::cout << "Please enter a filename for the audit file:" << std::endl;
            std::cout << "Please also include the .csv extension:" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } 
        else if (!file.is_open()) {
            std::cout << "Invalid file name" << std::endl;
            std::cout << "Following are the common mistakes:" << std::endl;
            std::cout << "1. Wrong file name" << std::endl;
            std::cout << "2. .csv extension is not included" << std::endl;
            std::cout << "Please re-enter the file name:" << std::endl;
            std::getline(std::cin, auditFileName);
        }
        else {
            break;
        }
    }
}

STV::STV(Ballots* ballots, int seats) : Election(ballots, seats) {

}

STV::~STV() {}

void STV::runElection() {
    
    promptAuditFilename();

    int seatsElected = 0;
    droopQuota = (ballots->getBallotCount() / (this->seats + 1)) + 1;
    //std::cout << "\nDroop QUOTA: " << droopQuota;
    int voter_choice = 1;
    int voteDistributionRound = 1;

    outputString = "Vote Distribution Round #" + std::to_string(voteDistributionRound);
    electionProgress.push_back(outputString);

    // traverse through the vector of ballots [first pass]
    for (int ballotId = 0; ballotId < ballots->getBallotCount(); ballotId++) {
        std::vector<int> current_ballot = ballots->getBallot(ballotId);
        
        // for each vote in the current ballot
        for (unsigned long candidateNum = 0; candidateNum < current_ballot.size(); candidateNum++) {
            
            // if the current vote is a 1, assign the ballot the candidate that recieved it
            if (current_ballot.at(candidateNum) == voter_choice &&  static_cast<int>(candidates.at(candidateNum).getAssignedBallots().size()) < droopQuota) {
                candidates.at(candidateNum).assignBallot(ballotId);
                
                // if assigning the ballot means that the candidate hits droop quota, then immediately add them to the winners list
                if (static_cast<int>(candidates.at(candidateNum).getAssignedBallots().size()) == droopQuota) {
                    outputString = "\n" + candidates.at(candidateNum).getName() + " hit Droop quota- adding to winners list";
                    electionProgress.push_back(outputString);
                    
                    winners.push_back(candidates.at(candidateNum));
                    seatsElected++;
                }
            }
            
            // if the ballot was going to be assigned to a candidate at droop quota, instead give the ballot to the next option
            else if (static_cast<int>(current_ballot.at(candidateNum)) == voter_choice && static_cast<int>(candidates.at(candidateNum).getAssignedBallots().size()) >= droopQuota) {
                outputString = "\n" + candidates.at(candidateNum).getName() + " already won a seat, reassigning ballot #" + std::to_string(ballotId);
                electionProgress.push_back(outputString);
                // if the vote was going to be given to the 1st candidate but they're at droop, then look for the (first + 1) choice etc.
                
                for (int currentPick = voter_choice + 1; currentPick < static_cast<int>(current_ballot.size()); currentPick++) {
                    //std::cout << "\nFinding the #" << currentPick << " choice for ballotID: " << ballotId;
                    bool reassigned = false;
                    
                    // traverse through the ballot and find which candidate corresponds to the vote priority... if we're on 1st choice give to the first candidate
                    for (unsigned long candidateNum = 0; candidateNum < current_ballot.size(); candidateNum++) {
                        
                        // if the current vote is a 1, assign the ballot to the candidate that recieved it
                        if (static_cast<int>(current_ballot.at(candidateNum)) == currentPick &&  static_cast<int>(candidates.at(candidateNum).getAssignedBallots().size()) < droopQuota) {
                            candidates.at(candidateNum).assignBallot(ballotId);
                            outputString = "\nBallot " + std::to_string(ballotId) + " was reassigned to the candidate: " + candidates.at(candidateNum).getName();
                            electionProgress.push_back(outputString);

                            // if assigning the ballot means that the candidate hits droop quota, then immediately add them to the winners list
                            if (static_cast<int>(candidates.at(candidateNum).getAssignedBallots().size()) == droopQuota) {
                                // maybe remove them from the candidates- HOWEVER, because the candidates vector doesn't have ids, the voter's choices require them to still be in the vector
                                outputString = "\n" + candidates.at(candidateNum).getName() + " hit Droop quota- adding to winners list";
                                electionProgress.push_back(outputString);
                                
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

    writeVotesToAudit(electionProgress, candidates);
    displayWinnersLosers(electionProgress, winners, losers);

    // after all of the votes are distributed, if there are still seats that need to be filled
    while (seatsElected < seats) {
        voteDistributionRound++;

        outputString = "\n\nVote Distribution Round #" + std::to_string(voteDistributionRound);
        electionProgress.push_back(outputString);

        // find the loser that is having their ballots redistribuited
        Candidate* redistributionLoser = findLoser(electionProgress, candidates, losers);
       
        // for each of the losing candidates ballots
        for (int i = 0; i < static_cast<int>(redistributionLoser->getAssignedBallots().size()); i++) {

            // find what number choice we're on, in the case that the ballot has already been assigned to its 2nd/3rd/etc choice or if the 2nd/3rd/etc choice has already hit Droop
            // this might need to be the LAST added ballot
            int currentBallotID = redistributionLoser->getAssignedBallots().at(i);
            redistributionLoser->removeBallot(currentBallotID);

            std::vector<int> currentBallot = ballots->getBallot(currentBallotID);

            int loserChoice = findLoserChoice(candidates, currentBallot, redistributionLoser);
            //std::cout << "\nCandidate " << redistributionLoser->getName() << " was ballot #" << currentBallotID << "'s " << loserChoice << "st pick";


            
            // choice #, i.e. looking for the 2nd choice after the first choice has lost
            for (int currentPick = loserChoice + 1; currentPick <  static_cast<int>(currentBallot.size()) + 1; currentPick++) {
                //std::cout << "\n[inside 2nd round]Finding the #" << currentPick << " choice for ballotID: " << currentBallotID;
                bool reassigned = false;
                
                // same code as above for assigning the vote to the current pick
                for (unsigned long candidateNum = 0; candidateNum < currentBallot.size(); candidateNum++) {
                    
                    // if the current vote is a 1, assign the ballot the candidate that recieved it
                    if ( static_cast<int>(currentBallot.at(candidateNum)) == currentPick &&  static_cast<int>(candidates.at(candidateNum).getAssignedBallots().size()) < droopQuota) {
                        candidates.at(candidateNum).assignBallot(currentBallotID);
                        //std::cout << "\nBallot " << currentBallotID << " was reassigned to the candidate: " << candidates.at(candidateNum).getName();
                        outputString = "\n" + redistributionLoser->getName() + " lost- redistributing ballot #" + std::to_string(currentBallotID) + " to " + candidates.at(candidateNum).getName();
                        electionProgress.push_back(outputString);

                        // if the vote is given to a loser, then remove them from the losers list because they're now in the running
                        if (notLoser(candidates.at(candidateNum), losers) == false) {
                            for (unsigned long i = 0; i < losers.size(); i++) {
                                if (losers.at(i).getName() == candidates.at(candidateNum).getName()) {
                                    losers.erase(losers.begin() + i);
                                }
                            }
                        }

                        // if assigning the ballot means that the candidate hits droop quota, then immediately add them to the winners list
                        if (static_cast<int>(candidates.at(candidateNum).getAssignedBallots().size()) == droopQuota) {
                            // maybe remove them from the candidates- HOWEVER, because the candidates vector doesn't have ids, the voter's choices require them to still be in the vector
                            //std::cout << "\nAssigning ballot #" << currentBallotID << " means that " << candidates.at(candidateNum).getName() << " hit Droop Quota... adding them to winners list.";
                            winners.push_back(candidates.at(candidateNum));
                            seatsElected++;
                        }                                    

                        // once the ballot is reassigned, break out of the assignment loop
                        reassigned = true;
                        break;
                    }
                }
                
                // if the ballot is already reassigned, no need to keep looking for next choices, eg. 3rd, 4th...
                if (reassigned == true) {
                    break;
                }
            }
        }

        writeVotesToAudit(electionProgress, candidates);
        displayWinnersLosers(electionProgress, winners, losers);
    }
    
    // code has been created to not hit this conditional statment, but here for testing [testing]
    if (static_cast<int>(winners.size()) < seats) {
        std::cout << "\n" << winners.size() << " candidates were elected, but we needed " << seats << ", here are the winners: ";
    }

    displayElectionDetails();
    outputAuditFile();
}

void STV::displayElectionDetails() {
    std::cout << "Election type: STV" << std::endl;
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

void STV::outputAuditFile() {

    for (unsigned long i = 0; i < electionProgress.size(); i++) {
        file << electionProgress.at(i);
    }
    file.close();
    std::cout << "\nOutput file succesfully created!";
}
