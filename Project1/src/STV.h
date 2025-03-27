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

        Ballots* getBallots();
        
        int getSeats();
        
        std::vector<Candidate> getCandidates();
        
        std::vector<Candidate> getWinners();
        
        std::vector<Candidate> getLosers();

        int getDroopQuota();

    private:
        int droopQuota;
        std::vector<std::string> electionProgress;
        Ballots* stv_ballots;
};

#endif
