// FileHandler.h
// Class that handle the input file
// Author: Mark Tsai

#include <iostream>
#include <sstream>

#include "FileHandler.h"

FileHandler::FileHandler() {}

void FileHandler::open_file(std::ifstream& file) {
    std::string file_name;
    std::cout << "Please enter the csv file name that contains the candidates and ballots" << std::endl;
    std::cout << "Please also include the .csv extension:" << std::endl;
    std::cin >> file_name;
    while (true) {
        file.open(file_name);
        if (!file.is_open()) { // file is not opened
            std::cout << "Invalid file name" << std::endl;
            std::cout << "Following are the common mistakes:" << std::endl;
            std::cout << "1. Wrong file name" << std::endl;
            std::cout << "2. File is not in the directory" << std::endl;
            std::cout << "3. .csv extension is not included" << std::endl;
            std::cout << "Please re-enter the file name:" << std::endl;
            std::getline(std::cin, file_name);
        } else { // file is opened
            break;
        }
    }
}

void FileHandler::read_file(std::ifstream& file, std::vector<std::string>& candidates, std::vector<std::vector<int>>& ballots_vector) {
    std::string line;
    
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
        unsigned long count = 0;
        while (count != candidates.size()) {
            //std::getline(ss, value, ',');
            // Prevent \n or \r at the end of the line
            std::getline(ss, value, ',');
            value.erase(value.find_last_not_of("\r\n") + 1);
            if (!value.empty()) {
                ballot.push_back(std::stoi(value));// 0 represents empty slot
                count++;
            } else {
                ballot.push_back(0); 
                count++;
            }
        }
        ballots_vector.push_back(ballot);
        count = 0;
    }
    
    file.close();
}

void FileHandler::read_file(std::ifstream& file, std::vector<std::string>& candidates, std::vector<std::vector<int>>& ballots_vector, std::string& alg, int& seatNum, int& candidateNum, int& ballotNum) {
    std::string line;

    // First line is election voting algo type
    std::getline(file, line);
    alg = line.erase(line.find_last_not_of("\r\n") + 1);;
    // Second line is number of seats
    std::getline(file, line);
    seatNum = std::stoi(line);
    // Third line is number of candidates
    std::getline(file, line);
    candidateNum = std::stoi(line);
    // Forth line is number of ballots
    std::getline(file, line);
    ballotNum = std::stoi(line);
    // The rest of the file is the ballot information
    read_file(file, candidates, ballots_vector);
}
