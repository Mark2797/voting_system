// testUI.cpp
// Unit testing for UI.cpp
// Author: Mark Tsai

#include "gtest/gtest.h"

#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <cstring>

#include "UI.h"

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

class UITest : public ::testing::Test {
    protected:
        UI ui;
        std::vector<std::string> bad_seatNum;
        std::vector<std::string> good_seatNum;
        std::vector<std::string> bad_alg;
        std::vector<std::string> good_alg_one;
        std::vector<std::string> good_alg_two;
        int old_stdout;
        int old_stdin;
        int null_fd;

    void SetUp() override {
        ui = UI();
        bad_seatNum = {"10000000000\n", "-10000000000\n", "abc\n", "0\n", "-123123\n", "5\n"};
        good_seatNum = {"10\n"};
        bad_alg = {"10000000000\n", "-10000000000\n", "abc\n", "0\n", "-123123\n", "5\n", "1\n"};
        good_alg_one = {"1\n"};
        good_alg_two = {"2\n"};
        old_stdout = dup(STDOUT_FILENO);
        old_stdin = dup(STDIN_FILENO);
        null_fd = open("/dev/null", O_WRONLY);
        dup2(null_fd, STDOUT_FILENO);
    }

    void TearDown() override {
        dup2(old_stdout, STDOUT_FILENO);
        close(null_fd);
    }
};

TEST_F(UITest, SeatNumTest) {
    int seatNum;
    
    userInput(bad_seatNum);
    ui.prompt_user_seatNum(seatNum);
    restore_stdin_fd(old_stdin);
    EXPECT_EQ(seatNum, 5);

    userInput(good_seatNum);
    ui.prompt_user_seatNum(seatNum);
    restore_stdin_fd(old_stdin);
    EXPECT_EQ(seatNum, 10);
}

TEST_F(UITest, AlgTest) {
    std::string alg;

    userInput(bad_alg);
    ui.prompt_user_alg(alg);
    restore_stdin_fd(old_stdin);
    EXPECT_EQ(alg, "Plurality");

    userInput(good_alg_one);
    ui.prompt_user_alg(alg);
    restore_stdin_fd(old_stdin);
    EXPECT_EQ(alg, "Plurality");

    userInput(good_alg_two);
    ui.prompt_user_alg(alg);
    restore_stdin_fd(old_stdin);
    EXPECT_EQ(alg, "STV");
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

