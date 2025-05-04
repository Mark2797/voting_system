// Municipal.h
// Perform a municipal election
// Author: Andy Dang

#ifndef MUNICIPAL_H_
#define MUNICIPAL_H_

#include "Election.h"
#include "Ballots.h"

/**
 * @class Municipal
 * @brief Perform a municipal election
 */
class Municipal : public Election {
    public:
        /// @brief Municipal constructor
        /// @param ballots Pointer to the Ballots that contain the ballots information
        /// @param seats Number of seats to be elected
        Municipal(Ballots* ballots, int seats);

        /// @brief Destructor
        ~Municipal();

        /// @brief Runs a municipal election
        void runElection();

        /// @brief Display the details of the election to the terminal
        void displayElectionDetails();
};

#endif
