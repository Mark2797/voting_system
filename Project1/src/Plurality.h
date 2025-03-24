// Plurality.h
// Perform a plurality election
// Author: Mark Tsai

#ifndef PLURALITY_H_
#define PLURALITY_H_

#include "Election.h"
#include "Ballots.h"

/**
 * @class Plurality
 * @brief Perform a plurality election
 */
class Plurality : public Election {
    public:
        Plurality(Ballots* ballots, int seats);
        ~Plurality();
        void runElection();
        void displayElectionDetials();
};

#endif
