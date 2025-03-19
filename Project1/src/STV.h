// Plurality.h
// Perform a single transferable vote (STV) election
// Author: Mark Tsai

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
        STV(Ballots* ballots, int seats);
        ~STV();
        void runElection() override;
        void displayElectionDetials();
        void outputAuditFile();
    private:
        int droopQuota;
        std::vector<std::string> electionProgress;
        Ballots* stv_ballots;
};

#endif
