// UI.h
// Class acts as user interface that prompts the user for some input
// Author: Mark Tsai

#include <limits>
#include <iostream>

#include "UI.h"

UI::UI() {}

void UI::prompt_user_seatNum(int& seatNum) {
    int num;
    while(true) {
        std::cout << "Please enter a positive integer for the number of seat to be elected: " << std::endl;
        std::cin >> num;
        if (std::cin.fail()) { // input is not an integer
            std::cout << "Invalid input!" << std::endl;
            std::cout << "Please enter a positive integer!" << std::endl;
            std::cin.clear();
            // handle leftover characters in the input buffer
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else if (num <= 0) { // input is not a positive integer
            std::cout << "Invalid input!" << std::endl;
            std::cout << "Please enter a positive integer!" << std::endl;
        } else {
            break;
        }
    }
    seatNum = num;
}

void UI::prompt_user_alg(std::string& alg) {
    int num;
    while(true) {
        std::cout << "Please choose an algorithm from the following options:" << std::endl;
        std::cout << "1. Plurality Algorithm" << std::endl;
        std::cout << "2. Single Transferable Vote (STV) Algorithm" << std::endl;
        std::cout << "Please select by entering the number 1 or 2:" << std::endl;
        std::cin >> num;
        if (std::cin.fail()) { // input is not an integer
            std::cout << "Invalid input!" << std::endl;
            std::cout << "Please enter 1 or 2!" << std::endl;
            std::cin.clear();
            // handle leftover characters in the input buffer
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else if (num != 1 && num != 2) { // input is not a valid option
            std::cout << "Invalid input!" << std::endl;
            std::cout << "Please enter 1 or 2!" << std::endl;
        } else {
            break;
        }
    }
    if (num == 1) {
        alg = "Plurality";
    } else {
        alg = "STV";
    }
}
