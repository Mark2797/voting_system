// testDriver.cpp
// Unit testing for testDriver
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

class MainTest : public ::testing::Test {
    protected:
        Driver driver;
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
        driver = Driver();
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

