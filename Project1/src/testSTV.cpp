// testSTV.cpp
// Unit testing for STV class
// Author: Michael Dunn

#include "gtest/gtest.h"

#include "STV.h"
#include <iostream>
// Use optional to delay initialization
// Google test requires a default constructor
#include <optional>

// Elections must be processed in under 5 minutes
#include <chrono>

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

class STVTest : public ::testing::Test {
    protected:
        std::vector<std::string> candidates;

        std::optional<STV> stv_basic_win;
        std::optional<STV> stv_winner_reassign_win;
        std::optional<STV> stv_loser_reassign_win;

        std::optional<Ballots> ballots_basic_win;
        std::optional<Ballots> ballots_winner_reassign_win;
        std::optional<Ballots> ballots_loser_reassign_win;

        std::vector<std::vector<int>> basic_win;
        std::vector<std::vector<int>> winner_reassign_win;
        std::vector<std::vector<int>> loser_reassign_win;

        int old_stdin;
        
    void SetUp() override {
        candidates = {
            "Bill Jones",
            "Alice Mix",
            "Sally Ride",
            "Ahmed Mohamed",
            "Siyang Xiong",
            "Preeti Banerjee"
        };

        basic_win = {
            {1,0,2,0,3,0},
            {3,2,1,4,6,5},
            {1,2,0,0,3,4},
            {4,5,1,2,0,3},
            {1,3,2,4,5,6},
            {2,3,1,5,4,3}
        };
        
        winner_reassign_win = {
            {1,0,2,0,3,0},
            {3,2,1,4,6,5},
            {1,2,0,0,3,4},
            {4,5,1,2,0,3},
            {1,3,2,4,5,6},
            {3,4,2,5,6,1},
            {1,3,2,5,4,3}
        };
        
        loser_reassign_win = {
            {1,0,2,0,3,0},
            {3,2,1,4,6,5},
            {1,2,0,0,3,4},
            {4,5,1,2,0,3},
            {6,3,2,4,1,5},
            {1,0,0,0,0,0}
        };

        old_stdin = dup(STDIN_FILENO);
    }
};

TEST_F(STVTest, BasicWinTest) {
    // basic win with 2 seats elected
    ballots_basic_win.emplace(candidates, basic_win, false);
    stv_basic_win.emplace(&(ballots_basic_win.value()), 2);

    std::vector<std::string> input_file_name = {"BasicWinTest.txt\n"};
    userInput(input_file_name);
    stv_basic_win->runElection();
    restore_stdin_fd(old_stdin);

    EXPECT_EQ(stv_basic_win->getBallots(), &(ballots_basic_win.value()));
    EXPECT_EQ(stv_basic_win->getSeats(), 2);
    for (long unsigned int i = 0; i < stv_basic_win->getCandidates().size(); i++) {
        EXPECT_EQ(stv_basic_win->getCandidates().at(i).getName(), candidates.at(i));
    }
    
    EXPECT_EQ(stv_basic_win->getWinners().size(), 2);
    std::vector<std::string> winners = {"Bill Jones", "Sally Ride"};
    for (long unsigned int i = 0; i < stv_basic_win->getWinners().size(); i++) {
        EXPECT_EQ(stv_basic_win->getWinners().at(i).getName(), winners.at(i));
    }

    EXPECT_EQ(stv_basic_win->getLosers().size(), 4);
    std::vector<std::string> losers = {"Alice Mix", "Ahmed Mohamed", "Siyang Xiong", "Preeti Banerjee"};
    for (long unsigned int i = 0; i < stv_basic_win->getLosers().size(); i++) {
        EXPECT_EQ(stv_basic_win->getLosers().at(i).getName(), losers.at(i));
    }
}

TEST_F(STVTest, WinnerReassignWin) {
    // winner hits droop (with 2 seats) and has their extra votes redistributed
    ballots_winner_reassign_win.emplace(candidates, winner_reassign_win, false);
    stv_winner_reassign_win.emplace(&(ballots_winner_reassign_win.value()), 2);

    std::vector<std::string> input_file_name = {"WinnerReassignWin.txt\n"};
    userInput(input_file_name);
    stv_winner_reassign_win->runElection();
    restore_stdin_fd(old_stdin);

    EXPECT_EQ(stv_winner_reassign_win->getBallots(), &(ballots_winner_reassign_win.value()));
    EXPECT_EQ(stv_winner_reassign_win->getSeats(), 2);
    for (long unsigned int i = 0; i < stv_winner_reassign_win->getCandidates().size(); i++) {
        EXPECT_EQ(stv_winner_reassign_win->getCandidates().at(i).getName(), candidates.at(i));
    }
    
    EXPECT_EQ(stv_winner_reassign_win->getWinners().size(), 2);
    std::vector<std::string> winners = {"Bill Jones", "Sally Ride"};
    for (long unsigned int i = 0; i < stv_winner_reassign_win->getWinners().size(); i++) {
        EXPECT_EQ(stv_winner_reassign_win->getWinners().at(i).getName(), winners.at(i));
    }

    EXPECT_EQ(stv_winner_reassign_win->getLosers().size(), 4);
    std::vector<std::string> losers = {"Alice Mix", "Ahmed Mohamed", "Siyang Xiong", "Preeti Banerjee"};
    for (long unsigned int i = 0; i < stv_winner_reassign_win->getLosers().size(); i++) {
        EXPECT_EQ(stv_winner_reassign_win->getLosers().at(i).getName(), losers.at(i));
    }
}

TEST_F(STVTest, LoserReassignWin) {
    // [Droop = 2] a loser is selected and their votes are redistributed to find a winner
    ballots_loser_reassign_win.emplace(candidates, loser_reassign_win, false);
    stv_loser_reassign_win.emplace(&(ballots_loser_reassign_win.value()), 2);

    std::vector<std::string> input_file_name = {"LoserReassignWin.txt\n"};
    userInput(input_file_name);
    stv_loser_reassign_win->runElection();
    restore_stdin_fd(old_stdin);

    EXPECT_EQ(stv_loser_reassign_win->getBallots(), &(ballots_loser_reassign_win.value()));
    EXPECT_EQ(stv_loser_reassign_win->getSeats(), 2);
    for (long unsigned int i = 0; i < stv_loser_reassign_win->getCandidates().size(); i++) {
        EXPECT_EQ(stv_loser_reassign_win->getCandidates().at(i).getName(), candidates.at(i));
    }
    
    EXPECT_EQ(stv_loser_reassign_win->getWinners().size(), 2);
    std::vector<std::string> winners = {"Bill Jones", "Sally Ride"};
    for (long unsigned int i = 0; i < stv_loser_reassign_win->getWinners().size(); i++) {
        EXPECT_EQ(stv_loser_reassign_win->getWinners().at(i).getName(), winners.at(i));
    }

    EXPECT_EQ(stv_loser_reassign_win->getLosers().size(), 4);
    std::vector<std::string> losers = {"Alice Mix", "Ahmed Mohamed", "Preeti Banerjee", "Siyang Xiong"};
    for (long unsigned int i = 0; i < stv_loser_reassign_win->getLosers().size(); i++) {
        EXPECT_EQ(stv_loser_reassign_win->getLosers().at(i).getName(), losers.at(i));
    }
}

TEST_F(STVTest, DisplayElectionDetailsTest) {
    testing::internal::CaptureStdout(); // catch previous prompts
    std::optional<STV> stv;
    std::optional<Ballots> ballots;
    ballots.emplace(candidates, basic_win, false);
    stv.emplace(&(ballots.value()), 2);

    std::vector<std::string> input_file_name = {"DisplayElectionDetailsTest.txt\n"};
    userInput(input_file_name);
    stv->runElection();
    restore_stdin_fd(old_stdin);

    std::string correct = "Please enter a filename for the audit file:\nPlease also include the .txt extension:\nElection type: STV\nNumber of seats: 2\nNumber of ballots: 6\nNumber of candidates: 6\nWinners:\nBill Jones\nSally Ride\nLosers:\nAlice Mix\nAhmed Mohamed\nSiyang Xiong\nPreeti Banerjee\n";
    EXPECT_EQ(testing::internal::GetCapturedStdout(), correct);
}

TEST_F(STVTest, EmptySeats) {
    std::optional<STV> stv;
    std::optional<Ballots> ballots;
    ballots.emplace(candidates, basic_win, false);
    stv.emplace(&(ballots.value()), 7);

    std::vector<std::string> input_file_name = {"EmptySeats.txt\n"};
    userInput(input_file_name);
    stv->runElection();
    restore_stdin_fd(old_stdin);

    EXPECT_EQ(stv->getBallots(), &(ballots.value()));
    EXPECT_EQ(stv->getSeats(), 7);
    for (long unsigned int i = 0; i < stv->getCandidates().size(); i++) {
        EXPECT_EQ(stv->getCandidates().at(i).getName(), candidates.at(i));
    }
    
    EXPECT_EQ(stv->getWinners().size(), 6);
    std::vector<std::string> winners = {"Bill Jones", "Sally Ride", "Alice Mix", "Ahmed Mohamed", "Siyang Xiong", "Preeti Banerjee"};
    for (long unsigned int i = 0; i < stv->getWinners().size(); i++) {
        EXPECT_EQ(stv->getWinners().at(i).getName(), winners.at(i));
    }

    EXPECT_EQ(stv->getLosers().size(), 0);
}

TEST_F(STVTest, ElectionTimeLimit) {
    auto start = std::chrono::high_resolution_clock::now();
    std::optional<STV> stv_two;
    std::optional<Ballots> ballots_two;
    std::vector<std::string> candidates_two;
    std::vector<std::vector<int>> ballots_stv_two;
    candidates_two = {
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine",
        "ten",
    };
    ballots_stv_two = {};
    for (int i = 0; i < 100000; i = i + 10) {
        ballots_stv_two.push_back({1, 2, 3, 4, 5, 6, 7, 8, 9, 0});
        ballots_stv_two.push_back({1, 2, 3, 4, 5, 6, 7, 8, 9, 0});
        ballots_stv_two.push_back({1, 2, 3, 4, 5, 6, 7, 8, 9, 0});
        ballots_stv_two.push_back({1, 2, 3, 4, 5, 6, 7, 8, 9, 0});
        ballots_stv_two.push_back({1, 2, 3, 4, 5, 6, 7, 8, 9, 0});
        ballots_stv_two.push_back({1, 2, 3, 4, 5, 6, 7, 8, 9, 0});
        ballots_stv_two.push_back({1, 2, 3, 4, 5, 6, 7, 8, 9, 0});
        ballots_stv_two.push_back({1, 2, 3, 4, 5, 6, 7, 8, 9, 0});
        ballots_stv_two.push_back({1, 2, 3, 4, 5, 6, 7, 8, 9, 0});
        ballots_stv_two.push_back({1, 2, 3, 4, 5, 6, 7, 8, 9, 0});
    }
    ballots_two.emplace(candidates_two, ballots_stv_two, false);
    stv_two.emplace(&(ballots_two.value()), 5);

    std::vector<std::string> input_file_name = {"ElectionTimeLimit.txt\n"};
    userInput(input_file_name);
    stv_two->runElection();
    restore_stdin_fd(old_stdin);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    EXPECT_LT(duration.count() / 1000.0, 5.000);
    EXPECT_EQ(stv_two->getBallots(), &(ballots_two.value()));
    EXPECT_EQ(stv_two->getSeats(), 5);
    for (long unsigned int i = 0; i < stv_two->getCandidates().size(); i++) {
        EXPECT_EQ(stv_two->getCandidates().at(i).getName(), candidates_two.at(i));
    }
    EXPECT_EQ(stv_two->getWinners().size(), 5);
    EXPECT_EQ(stv_two->getLosers().size(), 5);
}

TEST_F(STVTest, TieWin) {
    // one seat election
    ballots_basic_win.emplace(candidates, basic_win, false);
    stv_basic_win.emplace(&(ballots_basic_win.value()), 1);

    std::vector<std::string> input_file_name = {"TieWin.txt\n"};
    userInput(input_file_name);
    stv_basic_win->runElection();
    restore_stdin_fd(old_stdin);

    EXPECT_EQ(stv_basic_win->getBallots(), &(ballots_basic_win.value()));
    EXPECT_EQ(stv_basic_win->getSeats(), 1);
    for (long unsigned int i = 0; i < stv_basic_win->getCandidates().size(); i++) {
        EXPECT_EQ(stv_basic_win->getCandidates().at(i).getName(), candidates.at(i));
    }
    
    // regardless of who is the winner, there is only ONE winner and they have the correct number of ballots
    EXPECT_EQ(stv_basic_win->getWinners().size(), 1);
    int winnerBallots = stv_basic_win->getWinners().at(0).getAssignedBallots().size();
    EXPECT_EQ(winnerBallots, stv_basic_win->getDroopQuota());

    // because the winner is either Bill Jones or Sally Ride, they will be the last 'loser' on the list, so make sure the rest of the list is correct
    EXPECT_EQ(stv_basic_win->getLosers().size(), 5);
    std::vector<std::string> losers = {"Alice Mix", "Ahmed Mohamed", "Siyang Xiong", "Preeti Banerjee"};
    for (long unsigned int i = 0; i < stv_basic_win->getLosers().size() - 1; i++) {
        EXPECT_EQ(stv_basic_win->getLosers().at(i).getName(), losers.at(i));
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}