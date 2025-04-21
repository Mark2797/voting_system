// Driver.h
// Driver of the election that preprocess information and kick start the election
// Author: Mark Tsai

#ifndef DRIVER_H_
#define DRIVER_H_

#include "Election.h"
#include "Ballots.h"

/**
 * @class Driver
 * @brief Driver of the election that preprocess information and kick start the election
 */
class Driver {
    public:
        /**
         * @brief Constructor
         */
        Driver();

        /**
         * @brief Start the election process
         * @param argc Number of arguments
         * @param argv Arguments
         */
        void run(int argc, char **argv, Election*& election, Ballots*& ballots);

        /**
         * @brief Take arguments and check for shuffle-off flag
         * @param argc Number of arguments
         * @param argv Arguments
         * @param shuffle Boolean of shuffle indicating whether to shuffle or not
         * @return 0 on success 1 on error
         */
        int shuffleOffFlag(int argc, char **argv, bool &shuffle);
};

#endif
