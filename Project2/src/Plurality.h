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
        /// @brief Plurality constructor
        /// @param ballots Pointer to the Ballots that contain the ballots information
        /// @param seats Number of seats to be elected
        Plurality(Ballots* ballots, int seats);

        /// @brief Destructor
        ~Plurality();

        /// @brief Runs a plurality election
        void runElection();

        /// @brief Display the details of the election to the terminal
        void displayElectionDetails();
};

#endif
