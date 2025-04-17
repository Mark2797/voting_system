/// @file main.cpp
/// @brief The main of the software system

// main.cpp
// The main of the software system
// Author: Mark Tsai
// Assume perfect play for the csv file

#include "Driver.h"

/**
 * @brief The main function of the program.
 * @param argc Number of arguments
 * @param argv Arguments
 * @return 0 on successful execution and 1 on failed execution.
 */
int main(int argc, char **argv) {
    Driver driver = Driver();
    driver.run(argc, argv);
}
