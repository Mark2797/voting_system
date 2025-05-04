#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <cmath>

using namespace std;

// Created by Michael Dunn
// April 23rd, 2025
// Purpose is to create random MV CSV files to test for.

int main() {

    // create the timer and randomizer
    auto t0 = chrono::high_resolution_clock::now();
    random_device generator;
    mt19937 rd(generator());

    // initialize ballot, seat and candidate amounts
    vector<int> ballot_amt = {1, 20, 100, 1000, 100000};
    vector<int> seat_amt = {1, 1, 3, 5, 10}; // seat amounts provided by manan
    vector <int> candidate_amt = {1, 2, 5, 10, 20};

    if (ballot_amt.size() != seat_amt.size() && seat_amt.size() != candidate_amt.size()) {
        cout << "\nNumber of tests should match number of seats, candidates and ballots#";
        return -1;
    }
    
    // Each list of names and each ballot_amt.
    vector<string> names = {"Chuck Lancaster", "Mark Suckerberg", "Andrew Hero", 
        "Micheal Ashton", "Joe Cool", "Jimmy Donaldson", "John Kennedy", 
        "Patrick Star", "Robot Iam", "Alice Wonder", "Barack Obama", "James Lancaster", "Haley Welsh", 
        "James Blake", "Johnny Bravo", "Adam Sandler", "Holly Summers", "Polly Cracker", "Albert Wesk", "Leon Kennedy"};

    vector<vector<string>> names_list;

    // create the candidate name lists, using the ranges provided by candidate_amt
    for (int i = 0; i < candidate_amt.size(); i++) {
        vector<string> names_addition(names.begin(), names.begin() + candidate_amt.at(i));
        names_list.push_back(names_addition);
    }

    // create the filepath vector
    vector<string> path;
    for (int n = 0; n < candidate_amt.size(); n++) {
        path.push_back("../MV" + to_string((n + 1)) + ".csv");
    }

    // for each of the filepaths (each test, ex. MV1, MV2... MVn)
    for (int i = 0; i < path.size(); i++) {

        ofstream f(path[i]); // Start output stream
        vector<int> length(candidate_amt.at(i)); // could be names_list[i].size OR it could be candidate_amt.at(i)... either way
        iota(length.begin(), length.end(), 1); // fills "length" vector with ascending numbers [1, 2, ..., length.size]

        // get the header out of the way
        string str = "MV\n" + to_string(seat_amt.at(i)) + "\n" + to_string(candidate_amt.at(i)) + "\n" + to_string(ballot_amt.at(i)) + "\n";
        
        // traverse names list and add the name to the 4th row
        for (string name : names_list[i]) {
            str += name + ",";
        }

        str.pop_back();
        f << str << "\n";
        str = "";
        
        // because we can have any number of '1' votes, create the distirbution that informs the vector filling
        uniform_int_distribution<> distribution(0, length.size());

        //cout << "\nbalamt i: " << ballot_amt[i];
        for (int b = 0; b < ballot_amt[i]; b++) {

            // calculate the number of 'non-votes', if there are fewer than 2 votes we can't have all voters be non-voters
            int not_voted = distribution(rd);
            if (ballot_amt[i] < 2) {
                not_voted = 0;
            }

            // create a copy of the length vector to contain the current (b) ballot information
            vector<int> copy(length);

            // fill the # of non-votes with 0s... and shuffle the vector
            fill(copy.end() - not_voted, copy.end(), 0);
            shuffle(copy.begin(), copy.end(), rd);

            // then add the ballot information to the csv (or the str string rather)
            for (int k = 0; k < copy.size(); k++) {

                // if there isn't a 0 in the ballot, then add a '1', otherwise clear the 0 and add a comma
                if (copy[k] != 0) {
                    str += to_string(1) + ",";
                }
                else {
                    str += ",";
                }
            }
            
            // push that ballot into the csv
            str.pop_back();
            f << str << '\n';
            str = "";
        }

        f.close();
    }

    auto t1 = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = t1 - t0;
    cout << fixed << setprecision(2) << diff.count() << " seconds to generate " << path.size() << " test files." << endl;
    // Prints out total number of files & total time taken to generate files.

    return 0;
}

