// testMain.cpp
// Unit testing for Main.cpp
// Author: Mark Tsai

#include "gtest/gtest.h"

#include <unistd.h>
#include <fcntl.h>
#include <fstream>

#include "Ballots.h"

// Use optional to delay initialization
// Google test requires a default constructor
#include <optional>

extern void open_file(std::ifstream& file);
extern Ballots read_file(std::ifstream& file, bool shuffle);

void userInput(std::vector<std::string> input, int null_stream) {
    // Use pipe as user input
    int pipe_fds[2];
    pipe(pipe_fds);
    for (const auto& str : input) {
        write(pipe_fds[1], str.c_str(), str.length());
    }
    close(pipe_fds[1]);
    dup2(pipe_fds[0], STDIN_FILENO);
    close(pipe_fds[0]);
    dup2(null_stream, STDOUT_FILENO);
}

void restore_fd(int old_stdout, int old_stdin) {
    // restore the original FILENO
    dup2(old_stdout, STDOUT_FILENO);
    dup2(old_stdin, STDIN_FILENO);
}

class MainTest : public ::testing::Test {
    protected:
        std::vector<std::string> candidates;
        std::vector<std::vector<int>> ballots_plurality;
        std::vector<std::vector<int>> ballots_stv;
        std::vector<std::string> plurality_file_name;
        std::vector<std::string> stv_file_name;
        std::vector<std::string> bad_file_name;
        int old_stdout;
        int old_stdin;
        int null_fd;
    void SetUp() override {
        candidates = {
            "Bill Jones",
            "Alice Mix",
            "Sally Ride",
            "Ahmed Mohamed",
            "Siyang Xiong",
            "Preeti Banerjee"
        };
        ballots_plurality = {
            {1, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 0},
            {0, 0, 1, 0, 0, 0},
            {0, 0, 0, 1, 0, 0},
            {0, 0, 0, 0, 1, 0},
            {0, 0, 0, 0, 0, 1}
        };
        ballots_stv = {
            {1, 0, 2, 0, 3, 0},
            {3, 2, 1, 4, 6, 5},
            {1, 2, 0, 0, 3, 4},
            {4, 1, 0, 2, 0, 3},
            {1, 2, 3, 4, 5, 6},
            {2, 3, 4, 5, 6, 1},
            {0, 0, 0, 1, 2, 3}
        };
        plurality_file_name = {"../testing/plurality.csv\n"};
        stv_file_name = {"../testing/stv.csv\n"};
        bad_file_name = {"12343\n", "../testing/asdasd/\n", ".csv\n", "../testing/stv.csv\n"};
        old_stdout = dup(STDOUT_FILENO);
        old_stdin = dup(STDIN_FILENO);
        null_fd = open("/dev/null", O_WRONLY);
    }

    void TearDown() override {
        close(null_fd);
    }
};

TEST_F(MainTest, OpenFileTest) {
    // Test input with bad inputs and last correct input
    userInput(bad_file_name, null_fd);
    std::ifstream file_bad;
    open_file(file_bad);
    EXPECT_TRUE(file_bad.is_open());
    file_bad.close();
    restore_fd(old_stdout, old_stdin);

    // Test with corret input
    userInput(plurality_file_name, null_fd);
    std::ifstream file_plurality;
    open_file(file_plurality);
    EXPECT_TRUE(file_plurality.is_open());
    file_plurality.close();
    restore_fd(old_stdout, old_stdin);

    // Test with corret input
    userInput(stv_file_name, null_fd);
    std::ifstream file_stv;
    open_file(file_stv);
    EXPECT_TRUE(file_stv.is_open());
    file_stv.close();
    restore_fd(old_stdout, old_stdin);
}

TEST_F(MainTest, ReadFileTest) {
    // Read the plurality csv file
    userInput(plurality_file_name, null_fd);
    std::ifstream file_plurality;
    open_file(file_plurality);
    Ballots ballots_plurality_temp = read_file(file_plurality, false);
    file_plurality.close();
    restore_fd(old_stdout, old_stdin);
    EXPECT_EQ(ballots_plurality_temp.getCandidates(), candidates);
    for (int i = 0; i < static_cast<int>(ballots_plurality.size()); i++) {
        EXPECT_EQ(ballots_plurality_temp.getBallot(i), ballots_plurality.at(i));
    }

    // Read the stv csv file
    userInput(stv_file_name, null_fd);
    std::ifstream file_stv;
    open_file(file_stv);
    Ballots ballots_stv_temp = read_file(file_stv, false);
    file_stv.close();
    restore_fd(old_stdout, old_stdin);
    EXPECT_EQ(ballots_stv_temp.getCandidates(), candidates);
    for (int i = 0; i < static_cast<int>(ballots_stv.size()); i++) {
        EXPECT_EQ(ballots_stv_temp.getBallot(i), ballots_stv.at(i));
    }
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

