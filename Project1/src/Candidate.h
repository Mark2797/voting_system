// Candidate.h
// Represents one candidate in the election
// Author: Mark Tsai

#ifndef CANDIDATE_H_
#define CANDIDATE_H_

#include <vector>
#include <string>

/**
 * @class Candidate
 * @brief Represents one candidate in the election,
 * storing the name and the ballots assigned (ID).
 */
class Candidate {
    public:
        /**
         * @brief Candidate Constructor
         * @param name Name of the candidate
         */
        Candidate(std::string name);

        /**
         * @brief Destructor
         */
        ~Candidate();

        /**
         * @brief Assign the ballot to the candidate by its ID
         * @param ballotID Ballot's ID
         */
        void assignBallot(int ballotID);
        
        /**
         * @brief Remove the ballot to the candidate by its ID
         * @param ballotID Ballot's ID
         */
        void removeBallot(int ballotID);

        /**
         * @brief Get candidate's name
         * @return Candidate's name
         */
        std::string getName();

        /**
         * @brief Get all ballots (ID) assigned to the candidate
         * @return Vector of ballot IDs
         */
        std::vector<int> getAssignedBallots();

        /**
         * @brief Get total number of ballots assigned to the candidate
         * @return Number of assigned ballots
         */
        int getBallotNum();
    private:
        std::string name;
        std::vector<int> assignedBallots;
        int ballotNum = 0;
};

#endif
