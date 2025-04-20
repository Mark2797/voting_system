// testDriver.cpp
// Unit testing for Driver.cpp
// Author: Mark Tsai

#include "gtest/gtest.h"

#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <cstring>

#include "Driver.h"

void setArguments(int argc, char **argv, std::vector<std::string> &args) {
    for (int i = 0; i < argc; i++) {
        argv[i] = new char[args.at(i).length() + 1];
        std::strcpy(argv[i], args[i].c_str());
    }
}

void userInput(std::vector<std::string> input) {
    // Use pipe as user input
    int pipe_fds[2];
    pipe(pipe_fds);
    for (const auto& str : input) {
        write(pipe_fds[1], str.c_str(), str.length());
    }
    close(pipe_fds[1]);
    dup2(pipe_fds[0], STDIN_FILENO);
    close(pipe_fds[0]);
}

void restore_stdin_fd(int old_stdin) {
    // restore the original FILENO
    dup2(old_stdin, STDIN_FILENO);
}

class DriverTest : public ::testing::Test {
    protected:
        Driver driver;
        int old_stdin;
        int good_argc;
        int good_shuffle_argc;
        int bad_argc;
        int many_argc;
        char **good_argv;
        char **good_shuffle_argv;
        char **bad_argv;
        char **many_argv;
        std::vector<std::string> run_input;

    void SetUp() override {
        driver = Driver();
        old_stdin = dup(STDIN_FILENO);
        good_argc = 1;
        good_shuffle_argc = 2;
        bad_argc = 2;
        many_argc = 3;
        good_argv = new char *[good_argc];
        good_shuffle_argv = new char *[good_shuffle_argc];
        bad_argv = new char *[bad_argc];
        many_argv = new char *[many_argc];
        std::vector<std::string> good_args = {"program"};
        setArguments(good_argc, good_argv, good_args);
        std::vector<std::string> good_shuffle_args = {"program", "shuffle-off"};
        setArguments(good_shuffle_argc, good_shuffle_argv, good_shuffle_args);
        std::vector<std::string> bad_args = {"program", "shuffle"};
        setArguments(bad_argc, bad_argv, bad_args);
        std::vector<std::string> many_args = {"program", "shuffle-off", "extra"};
        setArguments(many_argc, many_argv, many_args);
        run_input = {"../testing/pluralityWithHeaderTestFileHandler.csv"};
    }

    void TearDown() override {
        for (int i = 0; i < good_argc; i++) {
            delete[] good_argv[i];
        }
        for (int i = 0; i < good_shuffle_argc; i++) {
            delete[] good_shuffle_argv[i];
        }
        for (int i = 0; i < bad_argc; i++) {
            delete[] bad_argv[i];
        }
        for (int i = 0; i < many_argc; i++) {
            delete[] many_argv[i];
        }
        delete[] good_argv;
        delete[] good_shuffle_argv;
        delete[] bad_argv;
        delete[] many_argv;
    }
};

TEST_F(DriverTest, RunTest) {
    userInput(run_input);
    testing::internal::CaptureStdout();
    Election *election;
    driver.run(good_shuffle_argc, good_shuffle_argv, election);
    delete election;
    std::string result = testing::internal::GetCapturedStdout();
    restore_stdin_fd(old_stdin);
    std::string expected_result = "Shuffle is off\nPlease enter the csv file name that contains the candidates and ballots\nPlease also include the .csv extension:\nElection type: Plurality\nNumber of seats: 2\nNumber of ballots: 11\nNumber of candidates: 6\nWinners:\nBill Jones\nAlice Mix\nLosers:\nSally Ride\nAhmed Mohamed\nSiyang Xiong\nPreeti Banerjee\nPercentage of votes:\nBill Jones (45.45%)\nAlice Mix (18.18%)\nSally Ride (9.09%)\nAhmed Mohamed (9.09%)\nSiyang Xiong (9.09%)\nPreeti Banerjee (9.09%)\n";
    EXPECT_EQ(result, expected_result);
}

TEST_F(DriverTest, ShuffleFlagTest) {
    int result_return;

    bool good_result;
    result_return = driver.shuffleOffFlag(good_argc, good_argv, good_result);
    EXPECT_EQ(good_result, true);
    EXPECT_EQ(result_return, 0);

    bool good_shuffle_result;
    result_return = driver.shuffleOffFlag(good_shuffle_argc, good_shuffle_argv, good_shuffle_result);
    EXPECT_EQ(good_shuffle_result, false);
    EXPECT_EQ(result_return, 0);

    bool bad_result;
    result_return = driver.shuffleOffFlag(bad_argc, bad_argv, bad_result);
    EXPECT_EQ(bad_result, true);
    EXPECT_EQ(result_return, 1);

    bool many_result;
    result_return = driver.shuffleOffFlag(many_argc, many_argv, many_result);
    EXPECT_EQ(many_result, true);
    EXPECT_EQ(result_return, 1);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

