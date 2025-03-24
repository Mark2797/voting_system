#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <iomanip>
#include <algorithm>

int main() {
    std::vector<std::string> names1 = {"Chuck Lancaster", "Mark Suckerberg", "Andrew Hero", "Micheal Ashton", "Joe Cool", "Jimmy Donaldson", "John Kennedy", "Patrick Star", "Robot Iam", "Alice Wonder"};
    std::vector<std::string> names2(names1.begin(), names1.begin() + 5);
    std::vector<std::vector<std::string>> names_list(4, names2);
    names_list.insert(names_list.end(), 4, names1);
    std::vector<int> ballots(4, 100);
    ballots.insert(ballots.end(), 4, 100000);
    std::vector<std::string> type = {"S", "P", "S", "P", "S", "P", "S", "P"};
    std::vector<std::string> path;
    for (int n = 1; n <= names_list.size(); n++) {
        path.push_back("test_file" + std::to_string(n) + ".csv");
    }

    auto t0 = std::chrono::high_resolution_clock::now();
    std::random_device generator;
    for (int i = 0; i < path.size(); i++) {
        std::ofstream f(path[i]);
        std::vector<int> length(names_list[i].size());
        for (int j = 0; j < length.size(); j++) {
            length[j] = j + 1;
        }
        std::string string = "";
        for (auto& name : names_list[i]) {
            string += name + ",";
        }
        string.pop_back();
        f << string << "\n";
        if (type[i] == "S") {
            std::vector<std::vector<int>> vals(ballots[i], std::vector<int>(length.size()));
            for (int b = 0; b < ballots[i]; ++b) {
                std::shuffle(length.begin(), length.end(), generator);
                for (int k = 0; k < length.size(); k++) {
                    vals[b][k] = length[k];
                }
            }
            for (auto& row : vals) {
                for (size_t k = 0; k < row.size(); ++k) {
                    f << row[k];
                    if (k < row.size() - 1) f << ",";
                }
                f << "\n";
            }
        } else if (type[i] == "P") {
            std::vector<std::vector<int>> vals(ballots[i], std::vector<int>(length.size(), 0));
            std::uniform_int_distribution<int> distribution(0, length.size() - 1);
            for (int b = 0; b < ballots[i]; b++) {
                int random_index = distribution(generator);
                vals[b][random_index] = 1;
            }
            for (const auto& row : vals) {
                for (size_t k = 0; k < row.size(); ++k) {
                    f << row[k];
                    if (k < row.size() - 1) f << ",";
                }
                f << "\n";
            }
        }
        f.close();
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = t1 - t0;
    std::cout << std::fixed << std::setprecision(2) << diff.count() << " seconds to generate " << path.size() << " test files." << std::endl;

    return 0;
}