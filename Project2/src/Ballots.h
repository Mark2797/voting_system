// Ballots.h
// Represents the entire information stores in the input ballot file
// Author: Mark Tsai

#ifndef BALLOTS_H_
#define BALLOTS_H_

#include <vector>
#include <string>

/**
 * @class Ballots
 * @brief Represents the entire information stores in the input ballot file,
 * including candidate names and ballots.
 */
class Ballots {
    public:
        /**
         * @brief Ballots constructor
         * @param candidates Vector of candidate names
         * @param ballots Vector of ballots
         * @param shuffle Whether to shuffle the ballots or not
         */
        Ballots(std::vector<std::string> candidates, std::vector<std::vector<int>> ballots, bool shuffle);

        /**
         * @brief Destructor
         */
        ~Ballots();

        /**
         * @brief Get ballot by index
         * @param index Index of the ballot to get
         * @return Ballot with given index
         */
        std::vector<int> getBallot(int index);

        /**
         * @brief Get all candidate names
         * @return Vector of candidate names
         */
        std::vector<std::string> getCandidates();

        /**
         * @brief Get the number of ballots stored
         * @return Number of ballots
         */
        int getBallotCount();

        /**
         * @brief Get the number of candidate names stored
         * @return Number of candidate names
         */
        int getCandidateCount();

        /**
         * @brief Get whether to shuffle the ballots or not
         * @return Boolean of the shuffle option
         */
        bool getShuffle();

    private:
        /**
         * @brief Shuffle the ballots
         */
        void shuffler();
        int ballotCount;
        int candidateCount;
        std::vector<std::string> candidates;
        std::vector<std::vector<int>> ballots;
        bool shuffle;
};

#endif
