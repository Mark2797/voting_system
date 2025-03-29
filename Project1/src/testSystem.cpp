#include "gtest/gtest.h"

#include "STV.h"
#include "Plurality.h"
#include "Ballots.h"
#include "Election.h"
#include "Candidate.h"

#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <bits/stdc++.h>

extern void open_file(std::ifstream& file);
extern Ballots read_file(std::ifstream& file, bool shuffle);
extern int shuffleOffFlag(int argc, char **argv, bool &shuffle);
extern void prompt_user_seatNum(int& seatNum);
extern void prompt_user_alg(std::string& alg);

using namespace std;

void userInput(std::vector<std::string> input) {
    // Use pipe as user input
    int pipe_fds[2];
    pipe(pipe_fds);

    for (const auto& str : input) {
        write(pipe_fds[1], str.c_str(), str.size());
    }
    close(pipe_fds[1]);
    dup2(pipe_fds[0], STDIN_FILENO);
    close(pipe_fds[0]);
}

void restore_stdin_fd(int old_stdin) {
    // restore the original FILENO
    dup2(old_stdin, STDIN_FILENO);
}

bool comp(string a, string b) {
    return a > b;
}

class SysTest : public ::testing::Test {
    protected:
        vector<vector<string>> file_names;
        vector<vector<string>> seatNums;
        vector<vector<string>> auditFile;
        int old_stdout = dup(STDOUT_FILENO);
        int old_stdin = dup(STDIN_FILENO);
        int shuffleValid = 1000;
        STV* STVelection;
        STV* STVelectionS;
        Plurality* pluralityElection;
        Plurality* pluralityElectionS;
        
    void SetUp() override {
        file_names = {{"../testing/test_file1.csv\n"}, {"../testing/test_file5.csv\n"},
        {"../testing/test_file7.csv\n"}, {"../testing/test_file8.csv\n"}};
        seatNums = {{"100\n"}, {"5\n"}};
        auditFile = {{"whatever.txt\n"}, {"noShuffle.txt\n"}, {"withShuffle.txt\n"}};
    }
    void TearDown() override {

    }
};

TEST_F(SysTest, seatValidity) {
    int seatNum;
    userInput(file_names.at(0));
    ifstream file;
    open_file(file);
    EXPECT_TRUE(file.is_open());
    restore_stdin_fd(old_stdin);
    Ballots ballots = read_file(file, false);
    file.close();
    
    userInput(seatNums.at(0));
    prompt_user_seatNum(seatNum);
    EXPECT_EQ(seatNum, 100);
    restore_stdin_fd(old_stdin);

    STVelection = new STV(&ballots, seatNum);
    userInput(auditFile.at(0));
    STVelection->runElection(); 
    restore_stdin_fd(old_stdin);

    int winners_size = STVelection->getWinners().size();
    EXPECT_EQ(winners_size, 1);
    pluralityElection = new Plurality(&ballots, seatNum);
    pluralityElection->runElection();
    winners_size = pluralityElection->getWinners().size();
    EXPECT_EQ(winners_size, 1);
};

TEST_F(SysTest, ballotShuffleValidity) {
    userInput(file_names.at(1));
    ifstream file;
    open_file(file);
    EXPECT_TRUE(file.is_open());
    restore_stdin_fd(old_stdin);
    Ballots ballots = read_file(file, false);
    file.close();

    for (int a = 0; a < shuffleValid; a++) {
        userInput(file_names.at(1));
        ifstream file2;
        open_file(file2);
        EXPECT_TRUE(file2.is_open());
        restore_stdin_fd(old_stdin);
        Ballots shuffled = read_file(file2, true);
        file2.close();

        int count = ballots.getBallotCount();
        int not_equal = 0;
        for (int i = 0 ; i < count; i++) {
            vector<int> ballot = ballots.getBallot(i);
            vector<int> sballot = shuffled.getBallot(i);
            if (ballot != sballot) not_equal++;
        }
        cout << not_equal << "% shuffled" << endl;
        EXPECT_GE(not_equal, 90); // Covers that the ballots are at minimum 90% shuffled
    }
};

TEST_F(SysTest, fairElectionValiditySTV) {
    int seatNum;
    userInput(file_names.at(2));
    ifstream file;
    open_file(file);
    EXPECT_TRUE(file.is_open());
    restore_stdin_fd(old_stdin);
    Ballots ballots = read_file(file, false);
    file.close();

    userInput(file_names.at(2));
    ifstream file2;
    open_file(file2);
    EXPECT_TRUE(file2.is_open());
    restore_stdin_fd(old_stdin);
    Ballots shuffled = read_file(file2, true);
    file2.close();

    userInput(seatNums.at(1));
    prompt_user_seatNum(seatNum);
    EXPECT_EQ(seatNum, 5);
    restore_stdin_fd(old_stdin);

    STVelection = new STV(&ballots, seatNum);
    userInput(auditFile.at(1));
    STVelection->runElection(); 
    restore_stdin_fd(old_stdin);

    STVelectionS = new STV(&shuffled, seatNum);
    userInput(auditFile.at(2));
    STVelectionS->runElection();
    restore_stdin_fd(old_stdin);

    vector<Candidate> regWinners = STVelection->getWinners();
    vector<Candidate> shfWinners = STVelectionS->getWinners();

    vector<string> regWin(regWinners.size());
    vector<string> shfWin(shfWinners.size());

    for (int i = 0; i < regWinners.size(); i++) {
        regWin.at(i) = regWinners.at(i).getName();
        shfWin.at(i) = shfWinners.at(i).getName();
    }

    sort(regWin.begin(), regWin.end(), comp); // Consider winners and losers come in different times.
    sort(shfWin.begin(), shfWin.end(), comp);
    EXPECT_EQ(regWin, shfWin);
};

TEST_F(SysTest, fairElectionValidityPlurlaity) {
    int seatNum;
    userInput(file_names.at(3));
    ifstream file;
    open_file(file);
    EXPECT_TRUE(file.is_open());
    restore_stdin_fd(old_stdin);
    Ballots ballots = read_file(file, false);
    file.close();

    userInput(file_names.at(3));
    ifstream file2;
    open_file(file2);
    EXPECT_TRUE(file2.is_open());
    restore_stdin_fd(old_stdin);
    Ballots shuffled = read_file(file2, true);
    file2.close();

    userInput(seatNums.at(1));
    prompt_user_seatNum(seatNum);
    EXPECT_EQ(seatNum, 5);
    restore_stdin_fd(old_stdin);

    pluralityElection = new Plurality(&ballots, seatNum);
    userInput(auditFile.at(1));
    pluralityElection->runElection(); 
    restore_stdin_fd(old_stdin);

    pluralityElectionS = new Plurality(&shuffled, seatNum);
    userInput(auditFile.at(2));
    pluralityElectionS->runElection();
    restore_stdin_fd(old_stdin);

    vector<Candidate> regWinners = pluralityElection->getWinners();
    vector<Candidate> shfWinners = pluralityElectionS->getWinners();

    vector<string> regWin(regWinners.size());
    vector<string> shfWin(shfWinners.size());

    for (int i = 0; i < regWinners.size(); i++) {
        regWin.at(i) = regWinners.at(i).getName();
        shfWin.at(i) = shfWinners.at(i).getName();
    }

    sort(regWin.begin(), regWin.end(), comp); // Consider winners and losers come in different times.
    sort(shfWin.begin(), shfWin.end(), comp);
    EXPECT_EQ(regWin, shfWin);
};
// TEST_F(SysTest, minElectionValidity) {};
// TEST_F(SysTest, pluralityRegular) {};
// TEST_F(SysTest, pluralityTwoTie) {};
// TEST_F(SysTest, pluralityMoreTie) {};
// TEST_F(SysTest, pluralityEveryoneTie) {};
// TEST_F(SysTest, pluralitySeatExceed) {};
// TEST_F(SysTest, stvRegular) {};
// TEST_F(SysTest, stvTie) {};
// TEST_F(SysTest, stvSeatExceed) {};
// TEST_F(SysTest, stvNonWinnerWinner) {};

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
