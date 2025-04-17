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
};

#endif
