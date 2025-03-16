// main.cpp
// The main of the software system
// Author: Mark Tsai

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <limits>

#include "Ballots.h"

/**
 * @brief Take arguments and check for shuffle-off flag
 * @param argc Number of arguments
 * @param argv Arguments
 * @param shuffle Boolean of shuffle indicating whether to shuffle or not
 * @return 0 on success 1 on error
 */
int shuffleOffFlag(int argc, char **argv, bool &shuffle) {
    shuffle = true;
    if (argc > 2) {
        std::cout << "Too many arguments" << std::endl;
        return 1;
    } else if (argc == 2) {
        std::string first_arg = std::string(argv[1]);
        // shuffle-off flag
        if (first_arg.compare("shuffle-off") != 0) {
            std::cout << "Invalid flag" << std::endl;
            return 1;
        } else {
            std::cout << "Shuffle is off" << std::endl;
            shuffle = false;
        }
    }
    return 0;
}

/**
 * @brief Open a ballot file with the correct file name or else keep prompting
 * @param file The file variable to store opened file
 */
void open_file(std::ifstream& file) {
    std::string file_name;
    std::cout << "Please enter the csv file name that contains the candidates and ballots" << std::endl;
    std::cout << "Please also include the .csv extension:" << std::endl;
    std::cin >> file_name;
    while (true) {
        file.open(file_name);
        if (!file.is_open()) {
            std::cout << "Invalid file name" << std::endl;
            std::cout << "Following are the common mistakes:" << std::endl;
            std::cout << "1. Wrong file name" << std::endl;
            std::cout << "2. File is not in the directory" << std::endl;
            std::cout << "3. .csv extension is not included" << std::endl;
            std::cout << "Please re-enter the file name:" << std::endl;
            std::getline(std::cin, file_name);
        } else {
            break;
        }
    }
}

/**
 * @brief Read the ballot file
 * @param file The file variable that stores the opened file
 * @param shuffle Whether shuffle the ballots or not
 * @return A Ballots class ballots that contain the ballot file information
 */
Ballots read_file(std::ifstream& file, bool shuffle) {
    std::string line;
    std::vector<std::string> candidates;
    std::vector<std::vector<int>> ballots_vector;
    
    // Read the first line to get candidate names
    if (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name;
        while (std::getline(ss, name, ',')) {
            // Prevent \n or \r at the end of the line
            name.erase(name.find_last_not_of("\r\n") + 1);
            candidates.push_back(name);
        }
    }

    // Read the rest of the line as ballots
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::vector<int> ballot;
        std::string value;
        while (std::getline(ss, value, ',')) {
            // Prevent \n or \r at the end of the line
            value.erase(value.find_last_not_of("\r\n") + 1);
            if (value.empty()) {
                ballot.push_back(0); // 0 represents empty slot
            } else {
                ballot.push_back(std::stoi(value));
            }
        }
        ballots_vector.push_back(ballot);
    }
    
    file.close();
    Ballots ballots(candidates, ballots_vector, shuffle);
    return ballots;
}

/**
 * @brief Prompt user for seat number and algorithm
 * @param seatNum Number of seat to be elected
 * @param alg Algorithm to use
 */
void prompt_user_seatNum(int& seatNum) {
    int num;
    while(true) {
        std::cout << "Please enter a positive integer for the number of seat to be elected: " << std::endl;
        std::cin >> num;
        if (std::cin.fail()) {
            std::cout << "Invalid input!" << std::endl;
            std::cout << "Please enter a positive integer!" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else if (num <= 0) {
            std::cout << "Invalid input!" << std::endl;
            std::cout << "Please enter a positive integer!" << std::endl;
        } else {
            break;
        }
    }
    seatNum = num;
}

/**
 * @brief Prompt user for seat number and algorithm
 * @param seatNum Number of seat to be elected
 * @param alg Algorithm to use
 */
void prompt_user_alg(std::string& alg) {
    int num;
    while(true) {
        std::cout << "Please choose an algorithm from the following options:" << std::endl;
        std::cout << "1. Plurality Algorithm" << std::endl;
        std::cout << "2. Single Transferable Vote (STV) Algorithm" << std::endl;
        std::cout << "Please select by entering the number 1 or 2:" << std::endl;
        std::cin >> num;
        if (std::cin.fail()) {
            std::cout << "Invalid input!" << std::endl;
            std::cout << "Please enter 1 or 2!" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else if (num != 1 && num != 2) {
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

// Only include main() if not being tested
#ifndef TESTING

/**
 * @brief The main function of the program.
 * @return 0 on successful execution and 1 on failed execution.
 */
int main(int argc, char **argv) {

    // Check shuffle flag
    bool shuffle;
    if (shuffleOffFlag(argc, argv, shuffle) != 0) {
        return 1;
    }

    // Try opening file until a valid file name is provided
    std::ifstream file;
    open_file(file);
    
    // Get number of seat and algorithm choice from user
    int seatNum;
    std::string alg;
    prompt_user_seatNum(seatNum);
    prompt_user_alg(alg);

    // Just for testing, printing out seat number and algorithm
    std::cout << "Number of seat: " << seatNum << std::endl;
    std::cout << "Algorithm: " << alg << std::endl;

    // Read the file and create a Ballot object
    Ballots ballots = read_file(file, shuffle);

    // Just for testing, printing out the content in ballots
    for (int i = 0; i < ballots.getCandidateCount(); i++) {
        std::cout << ballots.getCandidates().at(i) << std::endl;
    }

    for (int i = 0; i < ballots.getBallotCount(); i++) {
        std::vector<int> ballot = ballots.getBallot(i);
        for (int j = 0; j < ballots.getCandidateCount(); j++) {
            std::cout << ballot.at(j) << " ";
        }
        std::cout << std::endl;
    }
    
    return 0;
}

#endif
