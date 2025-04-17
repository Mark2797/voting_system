// STV.h
// Perform a single transferable vote (STV) election
// Author: Mark Tsai, Michael Dunn

#ifndef STV_H_
#define STV_H_

#include <vector>
#include <string>

#include "Election.h"
#include "Ballots.h"

/**
 * @class STV
 * @brief Perform a single transferable vote (STV) election
 */
class STV : public Election {
    public:
        /**
         * @brief STV Constructor
         * @param ballots Pointer to the Ballots that contain the ballots information
         * @param seats Number of seats to be elected
         */
        STV(Ballots* ballots, int seats);
        
        /**
         * @brief STV destructor
         */
        ~STV();

        /**
         * @brief Runs an STV election using the STV algorithm 
         */
        void runElection() override;

        /**
         * @brief Displays election information
         */
        void displayElectionDetails();

        /**
         * @brief Creates an audit file that contains election information after each vote distribution round
         */
        void outputAuditFile();

        /**
         * @brief Get the pointer to the Ballots* containing all ballot information
         * @return Ballot* object
         */
        Ballots* getBallots();
        
        /**
         * @brief Get total seat number
         * @return int with total number of seats
         */
        int getSeats();
        
        /**
         * @brief Get vector of Candidate objects for all candidates in the election
         * @return vector<Candidate> containing Candidate objects for each candidate
         */
        std::vector<Candidate> getCandidates();
        
        /**
         * @brief Get vector of Candidate objects containing winners of the election
         * @return vector<Candidate> containing Candidate objects for each candidate
         */
        std::vector<Candidate> getWinners();
        
        /**
         * @brief Get vector of Candidate objects containing losers of the election
         * @return vector<Candidate> containing Candidate objects for each candidate
         */
        std::vector<Candidate> getLosers();

        /**
         * @brief get the Droop quota of the election that is calculated and stored
         * @return int that corresponds to the Droop quota 
         */
        int getDroopQuota();

    private:
        int droopQuota;
        std::vector<std::string> electionProgress;
        Ballots* stv_ballots;
};

#endif
