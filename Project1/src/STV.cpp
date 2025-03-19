#include "STV.h"
#include <iostream>

STV::STV(Ballots* ballots, int seats) : Election(ballots, seats) {
}


STV::~STV() {}

void STV::runElection() override {

    droopQuota = ballots->getBallotCount() / (this->seats + 1);
    int voter_choice = 1;

    for (int ballotId = 0; ballotId < ballots->getBallotCount(); ballotId++) {
        std::vector<int> current_ballot = ballots->getBallot(ballotId);
        for (int candidateNum = 0; candidateNum < current_ballot.size(); candidateNum++) {
            if (current_ballot.at(candidateNum) == voter_choice) {
                candidates.at(candidateNum).assignBallot(ballotId);
            }
        }
    }

    std::cout << "\nDroop QUOTA: " << droopQuota;
    for (int i = 0; i < candidates.size(); i++) {
        std::vector<int> bruh = candidates.at(i).getAssignedBallots();
        for (int j = 0; j < bruh.size(); j++) {
            std::cout << "\nCandidate " << j << " assigned ballotID: " << bruh.at(j);
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