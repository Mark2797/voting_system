// Election.h
// The abstract class for Plurality and STV algorithm inheritance
// Author: Mark Tsai

#ifndef ELECTION_H_
#define ELECTION_H_

#include <vector>

#include "Ballots.h"
#include "Candidate.h"

/**
 * @class Election
 * @brief The abstract class for Plurality and STV algorithm inheritance
 */
class Election {
    public:
        /**
         * @brief Election Constructor
         * @param ballots Pointer to the Ballots that contain the ballots information
         * @param seats Number of seats to be elected
         */
        Election(Ballots* ballots, int seats);

        /**
         * @brief Destructor
         */
        virtual ~Election() = 0;

        /**
         * @brief Display the details of the election to the terminal
         */
        virtual void displayElectionDetails() = 0;

        /**
         * @brief Run the election with an voting algorithm
         */
        virtual void runElection() = 0;

        /**
         * @brief Get the pointer to the Ballots* containing all ballot information
         * @return Ballot* object
         */
        virtual Ballots* getBallots();
        
        /**
         * @brief Get total seat number
         * @return int with total number of seats
         */
        virtual int getSeats();

        /**
         * @brief Get vector of Candidate objects for all candidates in the election
         * @return vector<Candidate> containing Candidate objects for each candidate
         */
        virtual std::vector<Candidate> getCandidates();
        
        /**
         * @brief Get vector of Candidate objects containing winners of the election
         * @return vector<Candidate> containing Candidate objects for each candidate
         */
        virtual std::vector<Candidate> getWinners();
        
        /**
         * @brief Get vector of Candidate objects containing losers of the election
         * @return vector<Candidate> containing Candidate objects for each candidate
         */
        virtual std::vector<Candidate> getLosers();

    protected:
        Ballots* ballots;
        int seats;
        std::vector<Candidate> candidates;
        std::vector<Candidate> winners;
        std::vector<Candidate> losers;
};

#endif
