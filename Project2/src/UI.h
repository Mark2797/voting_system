// UI.h
// Class acts as user interface that prompts the user for some input
// Author: Mark Tsai

#ifndef UI_H_
#define UI_H_

#include <string>

/**
 * @class UI
 * @brief Class acts as user interface that prompts the user for some input
 */
class UI {
    public:
        /**
         * @brief Constructor
         */
        UI();

        /**
         * @brief Prompt user for seat number and algorithm
         * @param seatNum Number of seat to be elected
         */
        void prompt_user_seatNum(int& seatNum);

        /**
         * @brief Prompt user for seat number and algorithm
         * @param alg Algorithm to use
         */
        void prompt_user_alg(std::string& alg);
};

#endif
