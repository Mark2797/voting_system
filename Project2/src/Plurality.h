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

        /// @brief Getter function for ballots
        /// @return the ballots attribute
        Ballots* getBallots();

        /// @brief Getter function for seats
        /// @return the seats attribute
        int getSeats();

        /// @brief Getter function for candidates
        /// @return the ballots candidates
        std::vector<Candidate> getCandidates();

        /// @brief Getter function for winners
        /// @return the ballots winners
        std::vector<Candidate> getWinners();

        /// @brief Getter function for losers
        /// @return the ballots losers
        std::vector<Candidate> getLosers();
};

#endif
