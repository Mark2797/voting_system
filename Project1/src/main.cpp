// main.cpp
// The main of the software system
// Author: Mark Tsai

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>

#include "Ballots.h"

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
            std::cin >> file_name;
        } else {
            break;
        }
    }
}

Ballots read_file(std::ifstream& file, bool shuffle) {
    std::string line;
    std::vector<std::string> candidates;
    std::vector<std::vector<int>> ballots_vector;
    
    // Read the first line to get candidate names
    if (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name;
        while (std::getline(ss, name, ',')) {
            candidates.push_back(name);
        }
    }

    // Read the rest of the line as ballots
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::vector<int> ballot;
        std::string value;
        while (std::getline(ss, value, ',')) {
            if (value.empty()) {
                ballot.push_back(0); // 0 represents empty slot
            } else {
                if (isdigit(value[0])) { // Prevent newline at the end of the line
                    ballot.push_back(std::stoi(value));
                } else {
                    ballot.push_back(0); // Last slot in the ballot could be empty with newline
                }
            }
        }
        ballots_vector.push_back(ballot);
    }

    Ballots ballots(candidates, ballots_vector, shuffle);

    return ballots;
}

int main(int argc, char **argv) {

    // Check shuffle flag
    bool shuffle = false;
    if (argc > 2) {
        std::cout << "Too many arguments" << std::endl;
        return 1;
    } else if (argc == 2) {
        std::string first_arg = std::string(argv[1]);
        // shuffle-off flag
        if (first_arg.compare("shuffle-off") != 0) {
            std::cout << "Invalid flag" << std::endl;
        } else {
            std::cout << "Shuffle is off" << std::endl;
            shuffle = true;
        }
    }

    // Try opening file until a valid file name is provided
    std::ifstream file;
    open_file(file);

    // Read the file and create a Ballot object
    Ballots ballots = read_file(file, shuffle);

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