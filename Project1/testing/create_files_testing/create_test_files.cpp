#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <iomanip>
#include <numeric>
#include <algorithm>

using namespace std;

// Created by Manan Chaturvedi
// March 24th, 2025
// Purpose is to create random CSV files to test for.

int main() {
    vector<string> names1 = {"Chuck Lancaster", "Mark Suckerberg", "Andrew Hero", "Micheal Ashton", "Joe Cool", "Jimmy Donaldson", "John Kennedy", "Patrick Star", "Robot Iam", "Alice Wonder"};
    vector<string> names2(names1.begin(), names1.begin() + 5);
    vector<vector<string>> names_list(4, names2);
    names_list.insert(names_list.end(), 4, names1);
    vector<int> ballots(4, 100);
    ballots.insert(ballots.end(), 4, 100000);
    vector<string> type = {"S", "P", "S", "P", "S", "P", "S", "P"};
    vector<string> path;
    for (int n = 1; n <= names_list.size(); n++) {
        path.push_back("test_file" + to_string(n) + ".csv");
    }

    auto t0 = chrono::high_resolution_clock::now();
    random_device generator;
    mt19937 rd(generator());
    for (int i = 0; i < path.size(); i++) {
        ofstream f(path[i]);
        vector<int> length(names_list[i].size());
        iota(length.begin(), length.end(), 1);
        string str = "";
        for (string name : names_list[i]) {
            str += name + ",";
        }
        str.pop_back();
        f << str << "\n";
        str = "";
        if (type[i] == "S") {
            vector<vector<int>> vals(ballots[i], vector<int>(length.size()));
            for (int b = 0; b < ballots[i]; b++) {
                shuffle(length.begin(), length.end(), rd);
                for (int k = 0; k < length.size(); k++) {
                    str += to_string(length[k]) + ",";
                }
                str.pop_back();
                f << str << '\n';
                str = "";
            }
        } else if (type[i] == "P") {
            vector<vector<int>> vals(ballots[i], vector<int>(length.size(), 0));
            uniform_int_distribution<int> distribution(0, length.size() - 1);
            for (int b = 0; b < ballots[i]; b++) {
                int random_index = distribution(rd);
                vals[b][random_index] = 1;
            }
            for (vector<int> row : vals) {
                for (int k = 0; k < row.size(); k++) {
                    f << row[k];
                    if (k < row.size() - 1) f << ",";
                }
                f << "\n";
            }
        }
        f.close();
    }
    auto t1 = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = t1 - t0;
    cout << fixed << setprecision(2) << diff.count() << " seconds to generate " << path.size() << " test files." << endl;

    return 0;
}