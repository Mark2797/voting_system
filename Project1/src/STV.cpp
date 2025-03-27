// STV.cpp
// Performs an STV election
// Author: Michael Dunn

#include "STV.h"
#include <iostream>
#include <climits>
#include <bits/stdc++.h>
#include <fstream>

static std::string auditFileName;
static std::string outputString;
static std::ofstream file;

// write out each candidate and what ballot IDs they recieved... this causes slowdown but may be nice for auditing
static void writeVotesToAudit(std::vector<std::string>& electionProgress, std::vector<Candidate>& candidates) {

    for (int i = 0; i < static_cast<int>(candidates.size()); i++) {
        outputString = "\n" + candidates.at(i).getName() + ",";

        for (int j = 0; j < static_cast<int>(candidates.at(i).getAssignedBallots().size()); j++) {
            outputString += std::to_string(candidates.at(i).getAssignedBallots().at(j)) + ",";
        }
        electionProgress.push_back(outputString);
    }
}

// display the winners and losers at the time of being called
static void displayWinnersLosers(std::vector<std::string>& electionProgress, std::vector<Candidate>& winners, std::vector<Candidate>& losers) {
    outputString = "\nWinners: ";
    for (int i = 0; i < static_cast<int>(winners.size()); i++) {
        outputString += winners.at(i).getName() + ",";
    }
    electionProgress.push_back(outputString);

    outputString = "\nLosers: ";
    for (int i = 0; i < static_cast<int>(losers.size()); i++) {
        outputString += losers.at(i).getName() + ",";
    }
    electionProgress.push_back(outputString);
}

// finds what number choice the loser was given a ballot... so if the loser was the ballot's 2nd choice, this function will return 2
static int findLoserChoice(std::vector<Candidate>& candidates, std::vector<int>& currentBallot, int& loserId) {
    int loserChoice;
    for (int j = 0; j < static_cast<int>(currentBallot.size()); j++) {
        if (candidates.at(j).getName() == candidates.at(loserId).getName()) {
            loserChoice = currentBallot.at(j);
        }
    }
    return loserChoice;
}

// when given a candidate and a list, find whether that candidate appears in the list (i.e winners and losers)
static bool isInList(Candidate& candidate, std::vector<Candidate>& list) {
    for (int i = 0; i < static_cast<int>(list.size()); i++) {
        if (list.at(i).getName() == candidate.getName()) {
            return true;
        }
    }
    return false;
}

// shuffles the vector of tied losers and returns the loser that ends up at the front of the vector (randomly)
static int tieBreaker(std::vector<std::string>& electionProgress, std::vector<int>& lowestVoteCandidates) {

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(lowestVoteCandidates.begin(), lowestVoteCandidates.end(), g);
    
    int redistributionLoser = lowestVoteCandidates.at(0);

    return redistributionLoser;
} 

// assign ballot to a candidate and see if that ballot pushes the candidate into droop and therefore add to winners
static void assignBallot(std::vector<std::string>& electionProgress, int& candidateNum, int& ballotId, int& droopQuota, int& seatsElected, std::vector<Candidate>& candidates, std::vector<Candidate>& winners) { 
    // if the current vote is a 1, assign the ballot the candidate that recieved it
        candidates.at(candidateNum).assignBallot(ballotId);
        
        // if assigning the ballot means that the candidate hits droop quota, then immediately add them to the winners list
        if (static_cast<int>(candidates.at(candidateNum).getAssignedBallots().size()) == droopQuota) {
            outputString = "\n" + candidates.at(candidateNum).getName() + " hit Droop quota- adding to winners list";
            electionProgress.push_back(outputString);
            
            winners.push_back(candidates.at(candidateNum));
            seatsElected++;
        }
}

// finds the candidate with the fewest (non-zero) votes and returns their ID
static int findLoser(std::vector<std::string>& electionProgress, std::vector<Candidate>& candidates, std::vector<Candidate>& losers) {
    
    // create a vector to store candidates that are tied for lowest votes, and keep track of the current lowest vote number with lowestVotes    
    std::vector<int> lowestVoteCandidates;
    int lowestVotes = INT_MAX;
    int redistributionLoser = -99;
    
    // traverse through the candidate list and find the candidate with the lowest number of ballots
    for (int i = 0; i < static_cast<int>(candidates.size()); i++) {

        // if after the first pass a candidate didn't recieve any votes, add them to the losers list... their votes cannot be redistributed because they didn't get any
        if (candidates.at(i).getAssignedBallots().size() == 0 && isInList(candidates.at(i), losers) == false) {
            losers.push_back(candidates.at(i));
            outputString = "\nAdding " + candidates.at(i).getName() + " to the losers list because they recieved " + std::to_string(candidates.at(i).getAssignedBallots().size()) + " ballots.";
            electionProgress.push_back(outputString);  
        }
        
        // if there's a tie in losers and they're not already, add them to the tie list
        else if (static_cast<int>(candidates.at(i).getAssignedBallots().size()) == lowestVotes && isInList(candidates.at(i), losers) == false) {
            lowestVoteCandidates.push_back(i);
            lowestVotes = candidates.at(i).getAssignedBallots().size(); //technically not needed bc lowestvotes will already be this value but :)
        }
        
        // If a 'new' losing amount of ballots is achieved, then clear out the losingCandidates vector and add the NEW loser
        else if (static_cast<int>(candidates.at(i).getAssignedBallots().size()) < lowestVotes && candidates.at(i).getAssignedBallots().size() > 0 && isInList(candidates.at(i), losers) == false) {
            lowestVoteCandidates.clear();
            lowestVoteCandidates.push_back(i);
            lowestVotes = candidates.at(i).getAssignedBallots().size();
        }
    }       
        
    // if there were more than one 'losers' with the same number of losing votes (not including candidates that didn't recieve ANY votes)
    if (lowestVoteCandidates.size() > 1) {
        redistributionLoser = tieBreaker(electionProgress, lowestVoteCandidates);
        outputString = "\nTie between: ";
        for (int i = 0; i < static_cast<int>(lowestVoteCandidates.size()); i++) {
            outputString += candidates.at(lowestVoteCandidates.at(i)).getName() + ", ";
        }
        outputString += "- loser selected to be: " + candidates.at(redistributionLoser).getName();
        electionProgress.push_back(outputString);  
        return redistributionLoser;
    }
    
    // if there aren't any more candidates to redistribute their votes
    if (lowestVoteCandidates.size() == 0) {
        return -1;
    }

    // in the normal case, just return the only candidate in the loser vector
    redistributionLoser = lowestVoteCandidates.at(0);
    return redistributionLoser;
}

static void promptAuditFilename() {
    std::cout << "Please enter a filename for the audit file:" << std::endl;
    std::cout << "Please also include the .txt extension:" << std::endl;
    std::cin >> auditFileName;
    while(true) {
        file.open(auditFileName);
        if (std::cin.fail()) {
            std::cout << "Invalid input!" << std::endl;
            std::cout << "Please enter a filename for the audit file:" << std::endl;
            std::cout << "Please also include the .txt extension:" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } 
        else if (!file.is_open()) {
            std::cout << "Invalid file name" << std::endl;
            std::cout << "Following are the common mistakes:" << std::endl;
            std::cout << "1. Wrong file name" << std::endl;
            std::cout << "2. .txt extension is not included" << std::endl;
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
    int voter_choice = 1;
    bool reassigned = false;

    outputString = "[STV Election]: Droop: " + std::to_string(droopQuota) + ", Seats: " + std::to_string(seats) + ", Candidates: " + std::to_string(candidates.size());
    electionProgress.push_back(outputString);

    // traverse through the vector of ballots [first pass]
    for (int ballotId = 0; ballotId < ballots->getBallotCount(); ballotId++) {
        std::vector<int> current_ballot = ballots->getBallot(ballotId);
        
        // for each vote in the current ballot
        for (int candidateNum = 0; candidateNum < static_cast<int>(current_ballot.size()); candidateNum++) {
            
            // if the current vote is a 1, assign the ballot the candidate that recieved it
            if (current_ballot.at(candidateNum) == voter_choice && static_cast<int>(candidates.at(candidateNum).getAssignedBallots().size()) < droopQuota) {   
                assignBallot(electionProgress, candidateNum, ballotId, droopQuota, seatsElected, candidates, winners);
            }
            
            // if the ballot was going to be assigned to a candidate at droop quota, instead give the ballot to the next option
            else if (static_cast<int>(current_ballot.at(candidateNum)) == voter_choice && static_cast<int>(candidates.at(candidateNum).getAssignedBallots().size()) >= droopQuota) {
                outputString = "\n" + candidates.at(candidateNum).getName() + " already won a seat, reassigning ballot #" + std::to_string(ballotId);
                electionProgress.push_back(outputString);
                // if the vote was going to be given to the 1st candidate but they're at droop, then look for the (first + 1) choice etc.
                
                for (int currentPick = voter_choice + 1; currentPick < static_cast<int>(current_ballot.size()); currentPick++) {
                    reassigned = false;
                    
                    // traverse through the ballot and find which candidate corresponds to the vote priority... if we're on 1st choice give to the first candidate
                    for (int candidateNum = 0; candidateNum < static_cast<int>(current_ballot.size()); candidateNum++) {
                        
                        // if the current vote is a 1, assign the ballot to the candidate that recieved it
                        if (static_cast<int>(current_ballot.at(candidateNum)) == currentPick &&  static_cast<int>(candidates.at(candidateNum).getAssignedBallots().size()) < droopQuota) {
                            
                            // assign the ballot, and once the ballot is reassigned, break out of the assignment loop      
                            assignBallot(electionProgress, candidateNum, ballotId, droopQuota, seatsElected, candidates, winners);
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
    int voteRedistributionRound = 1;
    while (seatsElected < seats) {
        
        // find the loser that is having their ballots redistribuited
        int loserId = findLoser(electionProgress, candidates, losers);
        
        // in the case that there are more seats to elect but no more candidates can have their votes redistributed
        if (loserId == -1 || winners.size() == candidates.size()) {
            break;
        }
        
        outputString = "\n\nVote Redistribution Round #" + std::to_string(voteRedistributionRound);
        electionProgress.push_back(outputString);
        
        if (isInList(candidates.at(loserId), losers) == false) {
            losers.push_back(candidates.at(loserId));
        }
        
        // for each of the losing candidates ballots
        for (int i = 0; i < static_cast<int>(candidates.at(loserId).getAssignedBallots().size()); i++) {
            
            reassigned = false;
            int currentBallotID = candidates.at(loserId).getAssignedBallots().at(i);
            std::vector<int> currentBallot = ballots->getBallot(currentBallotID);
            int loserChoice = findLoserChoice(candidates, currentBallot, loserId);
                    
            // choice #, i.e. looking for the 2nd choice after the first choice has lost
            for (int currentPick = loserChoice + 1; currentPick < static_cast<int>(currentBallot.size()) + 1; currentPick++) {
                outputString = "\nReassigning ballot: " + std::to_string(currentBallotID) + " looking for the #" + std::to_string(currentPick) + " choice.";

                // for each candidate ranking, look for the candidate that matches the current choice search
                for (int candidateNum = 0; candidateNum < static_cast<int>(currentBallot.size()); candidateNum++) {
                    
                    // if the current vote is a 1, assign the ballot the candidate that recieved it
                    if (static_cast<int>(currentBallot.at(candidateNum)) == currentPick &&  static_cast<int>(candidates.at(candidateNum).getAssignedBallots().size()) < droopQuota && isInList(candidates.at(candidateNum), losers) == false) {
                        outputString += "- it was: " + candidates.at(candidateNum).getName();
                        electionProgress.push_back(outputString);
                        // assign the ballot, and once the ballot is reassigned, break out of the assignment loop      
                        assignBallot(electionProgress, candidateNum, currentBallotID, droopQuota, seatsElected, candidates, winners);
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

        // remove the redistributed ballots from the loser
        candidates.at(loserId).clearAssignedBallots();

        writeVotesToAudit(electionProgress, candidates);
        displayWinnersLosers(electionProgress, winners, losers);
        voteRedistributionRound++;
    }
    
    outputString = "\n\nFinal Election Results:";
    electionProgress.push_back(outputString);

    // in the case that there are more seats than candidates
    if (seatsElected < seats) {
        outputString = "\nOnly " + std::to_string(seatsElected) + " candidates were elected but " + std::to_string(seats) + " were needed.";
        outputString += "\nWinners are output in order below:";
        electionProgress.push_back(outputString);
    }

    // after all seats are elected, all candidates who haven't won a seat are deemed losers
    for (unsigned long i = 0; i < candidates.size(); i++) {  
        if (isInList(candidates.at(i), winners) == false && isInList(candidates.at(i), losers) == false) {
            losers.push_back(candidates.at(i));
        }
    }
    
    displayWinnersLosers(electionProgress, winners, losers);

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
}

void STV::outputAuditFile() {
    for (unsigned long i = 0; i < electionProgress.size(); i++) {
        file << electionProgress.at(i);
    }
    file.close();
}

Ballots* STV::getBallots() {
    return this->ballots;
}

int STV::getSeats() {
    return this->seats;
}

std::vector<Candidate> STV::getCandidates() {
    return this->candidates;
}

std::vector<Candidate> STV::getWinners() {
    return this->winners;
}

std::vector<Candidate> STV::getLosers() {
    return this->losers;
}

int STV::getDroopQuota() {
    return this->droopQuota;
}