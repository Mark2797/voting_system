// testMain.cpp
// Unit testing for main.cpp
// Author: Mark Tsai

#include "gtest/gtest.h"

#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <cstring>

#include "Ballots.h"

extern void open_file(std::ifstream& file);
extern Ballots read_file(std::ifstream& file, bool shuffle);
extern int shuffleOffFlag(int argc, char **argv, bool &shuffle);
extern void prompt_user_seatNum(int& seatNum, int candidateNum);
extern void prompt_user_alg(std::string& alg);

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

void setArguments(int argc, char **argv, std::vector<std::string> &args) {
    for (int i = 0; i < argc; i++) {
        argv[i] = new char[args.at(i).length() + 1];
        std::strcpy(argv[i], args[i].c_str());
    }
}

class MainTest : public ::testing::Test {
    protected:
        std::vector<std::string> candidates;
        std::vector<std::vector<int>> ballots_plurality;
        std::vector<std::vector<int>> ballots_stv;
        std::vector<std::string> plurality_file_name;
        std::vector<std::string> stv_file_name;
        std::vector<std::string> bad_file_name;
        std::vector<std::string> bad_seatNum;
        std::vector<std::string> good_seatNum;
        std::vector<std::string> bad_alg;
        std::vector<std::string> good_alg_one;
        std::vector<std::string> good_alg_two;
        int old_stdout;
        int old_stdin;
        int null_fd;
        int good_argc;
        int good_shuffle_argc;
        int bad_argc;
        int many_argc;
        char **good_argv;
        char **good_shuffle_argv;
        char **bad_argv;
        char **many_argv;

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
        plurality_file_name = {"../testing/pluralityTestMain.csv\n"};
        stv_file_name = {"../testing/stvTestMain.csv\n"};
        bad_file_name = {"12343\n", "../testing/asdasd/\n", ".csv\n", "../testing/stvTestMain.csv\n"};
        bad_seatNum = {"abc\n", "0\n", "-123123\n", "10\n", "5\n"};
        good_seatNum = {"5\n"};
        bad_alg = {"abc\n", "0\n", "-123123\n", "5\n", "1\n"};
        good_alg_one = {"1\n"};
        good_alg_two = {"2\n"};
        old_stdout = dup(STDOUT_FILENO);
        old_stdin = dup(STDIN_FILENO);
        null_fd = open("/dev/null", O_WRONLY);
        dup2(null_fd, STDOUT_FILENO);
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
        dup2(old_stdout, STDOUT_FILENO);
        close(null_fd);
    }
};

TEST_F(MainTest, ShuffleFlagTest) {
    int result_return;

    // Test taking shuffle-off flag
    bool good_result;
    result_return = shuffleOffFlag(good_argc, good_argv, good_result);
    EXPECT_EQ(good_result, true);
    EXPECT_EQ(result_return, 0);

    bool good_shuffle_result;
    result_return = shuffleOffFlag(good_shuffle_argc, good_shuffle_argv, good_shuffle_result);
    EXPECT_EQ(good_shuffle_result, false);
    EXPECT_EQ(result_return, 0);

    bool bad_result;
    result_return = shuffleOffFlag(bad_argc, bad_argv, bad_result);
    EXPECT_EQ(bad_result, true);
    EXPECT_EQ(result_return, 1);

    bool many_result;
    result_return = shuffleOffFlag(many_argc, many_argv, many_result);
    EXPECT_EQ(many_result, true);
    EXPECT_EQ(result_return, 1);
}

TEST_F(MainTest, OpenFileTest) {
    // Test input with bad inputs and last correct input
    userInput(bad_file_name);
    std::ifstream file_bad;
    open_file(file_bad);
    EXPECT_TRUE(file_bad.is_open());
    file_bad.close();
    restore_stdin_fd(old_stdin);

    // Test with corret input
    userInput(plurality_file_name);
    std::ifstream file_plurality;
    open_file(file_plurality);
    EXPECT_TRUE(file_plurality.is_open());
    file_plurality.close();
    restore_stdin_fd(old_stdin);

    // Test with corret input
    userInput(stv_file_name);
    std::ifstream file_stv;
    open_file(file_stv);
    EXPECT_TRUE(file_stv.is_open());
    file_stv.close();
    restore_stdin_fd(old_stdin);
}

TEST_F(MainTest, ReadFileTest) {
    // Read the plurality csv file
    userInput(plurality_file_name);
    std::ifstream file_plurality;
    open_file(file_plurality);
    Ballots ballots_plurality_temp = read_file(file_plurality, false);
    file_plurality.close();
    restore_stdin_fd(old_stdin);
    EXPECT_EQ(ballots_plurality_temp.getCandidates(), candidates);
    for (int i = 0; i < static_cast<int>(ballots_plurality.size()); i++) {
        EXPECT_EQ(ballots_plurality_temp.getBallot(i), ballots_plurality.at(i));
    }

    // Read the stv csv file
    userInput(stv_file_name);
    std::ifstream file_stv;
    open_file(file_stv);
    Ballots ballots_stv_temp = read_file(file_stv, false);
    file_stv.close();
    restore_stdin_fd(old_stdin);
    EXPECT_EQ(ballots_stv_temp.getCandidates(), candidates);
    for (int i = 0; i < static_cast<int>(ballots_stv.size()); i++) {
        EXPECT_EQ(ballots_stv_temp.getBallot(i), ballots_stv.at(i));
    }
}

TEST_F(MainTest, SeatNumTest) {
    int seatNum;
    
    userInput(bad_seatNum);
    prompt_user_seatNum(seatNum, 6);
    restore_stdin_fd(old_stdin);
    EXPECT_EQ(seatNum, 5);

    userInput(good_seatNum);
    prompt_user_seatNum(seatNum, 6);
    restore_stdin_fd(old_stdin);
    EXPECT_EQ(seatNum, 5);
}

TEST_F(MainTest, AlgTest) {
    std::string alg;

    userInput(bad_alg);
    prompt_user_alg(alg);
    restore_stdin_fd(old_stdin);
    EXPECT_EQ(alg, "Plurality");

    userInput(good_alg_one);
    prompt_user_alg(alg);
    restore_stdin_fd(old_stdin);
    EXPECT_EQ(alg, "Plurality");

    userInput(good_alg_two);
    prompt_user_alg(alg);
    restore_stdin_fd(old_stdin);
    EXPECT_EQ(alg, "STV");
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

