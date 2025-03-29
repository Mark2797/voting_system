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

// Created by Manan Chaturvedi
// March 24th, 2025
// Purpose is to create random CSV files to test for.

int main() {
    int test_value = 2;
    vector<int> ballot_amt = {10, 20, 100, 1000, 100000};
    // To keep it easier to change, this value corresponds to the total number of tests for
    // Each list of names and each ballot_amt.
    vector<string> names = {"Chuck Lancaster", "Mark Suckerberg", "Andrew Hero", 
        "Micheal Ashton", "Joe Cool", "Jimmy Donaldson", "John Kennedy", 
        "Patrick Star", "Robot Iam", "Alice Wonder", "Barack Obama", "James Lancaster", "Haley Welsh", 
        "James Blake", "Johnny Bravo", "Adam Sandler", "Holly Summers", "Polly Cracker", "Albert Wesk", "Leon Kennedy"};
    // Add or subtract names from here - keep in mind that this will be the total number
    // of candidates to be chosen as well.
    vector<string> names1(names.begin(), names.end());
    // First names vector taking from the entire names vector.
    vector<string> names2(names.begin(), names.begin() + 10);
    // A second names vector set to be a subset of the original names vector.
    // Ideally could be changed to a whole new vector, but kept this because it's easier.
    vector<string> names3(names.begin(), names.begin() + 5);
    // A third names vector set to be a subset of the original names vector.
    vector<string> names4(names.begin(), names.begin() + 2);
    // A fourth names vector set to be a subset of the original names vector.
    vector<string> names5(names.begin(), names.begin() + 1);
    // A fifth names vector set to be a subset of the original names vector.
    vector<vector<string>> names_list(test_value, names5);
    // This vector is dedicated to keeping track of all of the name lists to refer back to.
    names_list.insert(names_list.end(), test_value, names4);
    names_list.insert(names_list.end(), test_value, names3);
    names_list.insert(names_list.end(), test_value, names2);
    names_list.insert(names_list.end(), test_value, names);
    vector<int> ballots(names_list.size());
    // This refers to the total number of ballots. The right value can be changed without
    // concern, however the left value should be alligned with the total amount of 
    // tests we want to run.
    for (int i = 0; i < names_list.size(); i++) {
        ballots.at(i) = ballot_amt[ceil(i / test_value)];
    }
    // Using a loop with ceiling divsion to push the total number of ballots.
    static vector<string> types = {"S", "P"};
    vector<string> type_list(names_list.size());
    for (int i = 0; i < names_list.size(); i++) {
        type_list.at(i) = types[i % 2];
    }
    // Interchanging types of algorithms to run. 
    // DO NOT TOUCH TYPES. IT SHOULD ONLY HAVE S AND P.
    vector<string> path;
    for (int n = 1; n <= names_list.size(); n++) {
        path.push_back("../test_file" + to_string(n) + ".csv");
    }
    // Creates the total amount of test files as csv files to output.

    // DO NOT EDIT CODE BELOW.

    auto t0 = chrono::high_resolution_clock::now();
    random_device generator;
    mt19937 rd(generator());
    // Creates a random generator based on hardware.
    for (int i = 0; i < path.size(); i++) {
        ofstream f(path[i]); // Start output stream
        vector<int> length(names_list[i].size());
        iota(length.begin(), length.end(), 1); // Gets length vector ready for being randomized
        string str = "";
        for (string name : names_list[i]) {
            str += name + ",";
        }
        str.pop_back();
        f << str << "\n";
        str = "";
        // Prints out names at the top of the file.
        if (type_list[i] == "S") {
            // STV ALGORITHM
            // Randomzies order & number of non-votes, then writes to the file.
            uniform_int_distribution<> distribution(0, length.size() / 2);
            vector<vector<int>> vals(ballots[i], vector<int>(length.size()));
            for (int b = 0; b < ballots[i]; b++) {
                int not_voted = distribution(rd);
                vector<int> copy(length);
                fill(copy.end() - not_voted, copy.end(), 0);
                shuffle(copy.begin(), copy.end(), rd);
                for (int k = 0; k < copy.size(); k++) {
                    if (copy[k] != 0) {
                        str += to_string(copy[k]) + ",";
                    }
                    else {
                        str += ",";
                    }
                }
                str.pop_back();
                f << str << '\n';
                str = "";
            }
        } else if (type_list[i] == "P") {
            // PLURALITY ALGORITHM
            // Ranomizes what candidate (index) gets the vote, then writes to the file.
            vector<vector<int>> vals(ballots[i], vector<int>(length.size()));
            uniform_int_distribution<int> distribution(0, length.size() - 1);
            for (vector<int> row : vals) {
                int random_index = distribution(rd);
                row[random_index] = 1;
                for (int k = 0; k < row.size(); k++) {
                    if (row[k] == 1){
                        str += to_string(row[k]) + ",";
                    }
                    else {
                        str += ",";
                    }
                }
                str.pop_back();
                f << str << '\n';
                str = "";
            }
        }
        f.close();
    }
    auto t1 = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = t1 - t0;
    cout << fixed << setprecision(2) << diff.count() << " seconds to generate " << path.size() << " test files." << endl;
    // Prints out total number of files & total time taken to generate files.

    return 0;
}