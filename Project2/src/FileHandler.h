// FileHandler.h
// Class that handle the input file
// Author: Mark Tsai

#ifndef FILEHANDLER_H_
#define FILEHANDLER_H_

#include <fstream>
#include <string>
#include <vector>

/**
 * @class FileHandler
 * @brief Class that handle the input file
 */
class FileHandler {
    public:
        /**
         * @brief Constructor
         */
        FileHandler();

        /**
         * @brief Open a ballot file with the correct file name or else keep prompting
         * @param file The file variable to store opened file
         */
        void open_file(std::ifstream& file);

        /**
         * @brief Read the ballot file
         * @param file The file variable that stores the opened file
         * @param candidates Vector that stores the candidate names
         * @param ballots_vector Vector that stores the ballot information
         */
        void read_file(std::ifstream& file, std::vector<std::string>& candidates, std::vector<std::vector<int>>& ballots_vector);

        /**
         * @brief Read the ballot file including its headers
         * @param file The file variable that stores the opened file
         * @param candidates Vector that stores the candidate names
         * @param ballots_vector Vector that stores the ballot information
         * @param alg Algorithm to use
         * @param seatNum Number of seat to be elected
         * @param candidateNum Number of candidates
         * @param ballotNum Number of ballot
         */
        void read_file(std::ifstream& file, std::vector<std::string>& candidates, std::vector<std::vector<int>>& ballots_vector, std::string& alg, int& seatNum, int& candidateNum, int& ballotNum);

        /**
         * @brief Read the ballot file including its headers from single file or multiple files
         * @param candidates Vector that stores the candidate names
         * @param ballots_vector Vector that stores the ballot information
         * @param alg Algorithm to use
         * @param seatNum Number of seat to be elected
         * @param candidateNum Number of candidates
         * @param ballotNum Number of ballot
         * @return 0 on success 1 on error
         */
        int multiple_files(std::vector<std::string>& candidates, std::vector<std::vector<int>>& ballots_vector, std::string& alg, int& seatNum, int& candidateNum, int& ballotNum);
};

#endif
