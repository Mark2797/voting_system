// Driver.cpp
// Driver of the election that preprocess information and kick start the election
// Author: Mark Tsai

#include <iostream>

#include "Driver.h"
#include "FileHandler.h"
#include "UI.h"
#include "Ballots.h"
#include "Election.h"
#include "Plurality.h"
#include "STV.h"

Driver::Driver() {}

void Driver::run(int argc, char **argv) {
    // Check shuffle flag
    bool shuffle;
    if (shuffleOffFlag(argc, argv, shuffle) != 0) {
        return;
    }

    FileHandler fh = FileHandler();
    // Try opening file until a valid file name is provided
    std::ifstream file;
    fh.open_file(file);
    std::vector<std::string> candidates;
    std::vector<std::vector<int>> ballots_vector;
    fh.read_file(file, candidates, ballots_vector);
    Ballots ballots(candidates, ballots_vector, shuffle);

    UI ui = UI();
    // Get number of seat and algorithm choice from user
    int seatNum;
    std::string alg;
    ui.prompt_user_seatNum(seatNum);
    ui.prompt_user_alg(alg);

    // Create Election object and start the elction with collected information
    Election* election;
    if (alg == "STV") {
        election = new STV(&ballots, seatNum);
    }
    else {
        election = new Plurality(&ballots, seatNum);
    }
    election->runElection();
    delete election;
}

int Driver::shuffleOffFlag(int argc, char **argv, bool &shuffle) {
    shuffle = true;
    if (argc > 2) { // too many arguments
        std::cout << "Too many arguments" << std::endl;
        return 1;
    } else if (argc == 2) {
        std::string first_arg = std::string(argv[1]);
        // check for shuffle-off flag
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
