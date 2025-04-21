// testFileHandler.cpp
// Unit testing for FileHandler.cpp
// Author: Mark Tsai

#include "gtest/gtest.h"

#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <cstring>

#include "FileHandler.h"

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

class FileHandlerTest : public ::testing::Test {
    protected:
        FileHandler fh;
        std::vector<std::string> candidates;
        std::vector<std::vector<int>> ballots_plurality;
        std::vector<std::vector<int>> ballots_stv;
        std::vector<std::string> plurality_file_name;
        std::vector<std::string> stv_file_name;
        std::vector<std::string> plurality_file_name_header;
        std::vector<std::string> stv_file_name_header;
        std::vector<std::string> bad_file_name;
        int old_stdin;

    void SetUp() override {
        fh = FileHandler();
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
        plurality_file_name = {"../testing/pluralityTestFileHandler.csv\n"};
        stv_file_name = {"../testing/stvTestFileHandler.csv\n"};
        plurality_file_name_header = {"../testing/pluralityWithHeaderTestFileHandler.csv\n"};
        stv_file_name_header = {"../testing/stvWithHeaderTestFileHandler.csv\n"};
        bad_file_name = {"12343\n", "../testing/asdasd\n", "../testing/asdasd.csv\n", "1231.csv\n", "../testing/stvTestFileHandler.csv\n"};
        old_stdin = dup(STDIN_FILENO);
    }
};

TEST_F(FileHandlerTest, OpenFileTest) {
    // Test input with bad inputs and last correct input
    userInput(bad_file_name);
    std::ifstream file_bad;
    fh.open_file(file_bad);
    EXPECT_TRUE(file_bad.is_open());
    file_bad.close();
    restore_stdin_fd(old_stdin);

    // Test with corret input
    userInput(plurality_file_name);
    std::ifstream file_plurality;
    fh.open_file(file_plurality);
    EXPECT_TRUE(file_plurality.is_open());
    file_plurality.close();
    restore_stdin_fd(old_stdin);

    // Test with corret input
    userInput(stv_file_name);
    std::ifstream file_stv;
    fh.open_file(file_stv);
    EXPECT_TRUE(file_stv.is_open());
    file_stv.close();
    restore_stdin_fd(old_stdin);
}

TEST_F(FileHandlerTest, ReadFileTest) {
    // Read the plurality csv file
    userInput(plurality_file_name);
    std::ifstream file_plurality;
    fh.open_file(file_plurality);
    std::vector<std::string> candidates_plurality;
    std::vector<std::vector<int>> ballots_vector_plurality;
    fh.read_file(file_plurality, candidates_plurality, ballots_vector_plurality);
    file_plurality.close();
    restore_stdin_fd(old_stdin);
    EXPECT_EQ(candidates_plurality, candidates);
    EXPECT_EQ(ballots_vector_plurality, ballots_plurality);

    // Read the stv csv file
    userInput(stv_file_name);
    std::ifstream file_stv;
    fh.open_file(file_stv);
    std::vector<std::string> candidates_stv;
    std::vector<std::vector<int>> ballots_vector_stv;
    fh.read_file(file_stv, candidates_stv, ballots_vector_stv);
    file_stv.close();
    restore_stdin_fd(old_stdin);
    EXPECT_EQ(candidates_stv, candidates);
    EXPECT_EQ(ballots_vector_stv, ballots_stv);
}

TEST_F(FileHandlerTest, ReadFileWithHeaderTest) {
    // Read the plurality csv file with header
    userInput(plurality_file_name_header);
    std::ifstream file_plurality;
    fh.open_file(file_plurality);
    std::vector<std::string> candidates_plurality;
    std::vector<std::vector<int>> ballots_vector_plurality;
    std::string alg_plurality;
    int seatNum_plurality;
    int candidateNum_plurality;
    int ballotNum_plurality;
    fh.read_file(file_plurality, candidates_plurality, ballots_vector_plurality, alg_plurality, seatNum_plurality, candidateNum_plurality, ballotNum_plurality);
    file_plurality.close();
    restore_stdin_fd(old_stdin);
    EXPECT_EQ(candidates_plurality, candidates);
    EXPECT_EQ(ballots_vector_plurality, ballots_plurality);
    EXPECT_EQ(alg_plurality, "PV");
    EXPECT_EQ(seatNum_plurality, 2);
    EXPECT_EQ(candidateNum_plurality, 6);
    EXPECT_EQ(candidateNum_plurality, candidates_plurality.size());
    EXPECT_EQ(ballotNum_plurality, 11);
    EXPECT_EQ(ballotNum_plurality, ballots_vector_plurality.size());

    // Read the stv csv file with header
    userInput(stv_file_name_header);
    std::ifstream file_stv;
    fh.open_file(file_stv);
    std::vector<std::string> candidates_stv;
    std::vector<std::vector<int>> ballots_vector_stv;
    std::string alg_stv;
    int seatNum_stv;
    int candidateNum_stv;
    int ballotNum_stv;
    fh.read_file(file_stv, candidates_stv, ballots_vector_stv, alg_stv, seatNum_stv, candidateNum_stv, ballotNum_stv);
    file_stv.close();
    restore_stdin_fd(old_stdin);
    EXPECT_EQ(candidates_stv, candidates);
    EXPECT_EQ(ballots_vector_stv, ballots_stv);
    EXPECT_EQ(alg_stv, "STV");
    EXPECT_EQ(seatNum_stv, 3);
    EXPECT_EQ(candidateNum_stv, 6);
    EXPECT_EQ(candidateNum_stv, candidates_stv.size());
    EXPECT_EQ(ballotNum_stv, 7);
    EXPECT_EQ(ballotNum_stv, ballots_vector_stv.size());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

