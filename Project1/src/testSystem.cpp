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
#include <chrono>
#include <iostream>
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

bool comp(int a, int b) {
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
        file_names = {{"../testing/STV1.csv\n"}, {"../testing/Plurality1.csv\n"}, {"../testing/STV2.csv\n"},
        {"../testing/Plurality2.csv\n"}, {"../testing/STV3.csv\n"}, {"../testing/Plurality3.csv\n"},
        {"../testing/STV4.csv\n"}, {"../testing/Plurality4.csv\n"}, {"../testing/STV5.csv\n"},
        {"../testing/Plurality5.csv\n"}, {"../testing/pluralityTie.csv\n"}, {"../testing/stvTie.csv\n"},
        {"../testing/seatLowPlurality.csv\n"}, {"../testing/seatLowSTV.csv\n"}};
        seatNums = {{"21\n"}, {"5\n"}, {"1\n"}, {"3\n"}, {"10\n"}};
        auditFile = {{"../testing/audits/seatValid.txt\n"}, {"../testing/audits/noShuffle.txt\n"}, {"../testing/audits/withShuffle.txt\n"}, 
        {"../testing/audits/minSTV.txt\n"}, {"../testing/audits/regSTV.txt\n"}, {"../testing/audits/timeTest.txt\n"}, 
        {"../testing/audits/stvTie.txt\n"}, {"../testing/audits/lowVoteValid.txt\n"}};
    }
    void TearDown() override {
        
    }
};

TEST_F(SysTest, seatValidityDefecit) {
    int seatNum;
    userInput(file_names.at(13));
    ifstream file;
    open_file(file);
    EXPECT_TRUE(file.is_open());
    restore_stdin_fd(old_stdin);
    Ballots ballots = read_file(file, false);
    file.close();

    userInput(file_names.at(12));
    ifstream file2;
    open_file(file2);
    EXPECT_TRUE(file2.is_open());
    restore_stdin_fd(old_stdin);
    Ballots ballotp = read_file(file2, false);
    file2.close();

    userInput(seatNums.at(0));
    prompt_user_seatNum(seatNum);
    EXPECT_EQ(seatNum, 21);
    restore_stdin_fd(old_stdin);

    STVelection = new STV(&ballots, seatNum);
    userInput(auditFile.at(7));
    STVelection->runElection(); 
    restore_stdin_fd(old_stdin);

    int winners_size = STVelection->getWinners().size();
    int losers_size = STVelection->getLosers().size();
    int candidates_size = STVelection->getCandidates().size() - 1;
    EXPECT_EQ(winners_size, 1);
    EXPECT_EQ(losers_size, candidates_size);

    pluralityElection = new Plurality(&ballotp, seatNum);
    
    pluralityElection->runElection();
    winners_size = pluralityElection->getWinners().size();
    losers_size = pluralityElection->getLosers().size();
    candidates_size = pluralityElection->getCandidates().size() - 1;
    EXPECT_EQ(winners_size, 1);
    EXPECT_EQ(losers_size, candidates_size);
};

TEST_F(SysTest, seatValiditySurplus) {
    int seatNum;
    userInput(file_names.at(8));
    ifstream file;
    open_file(file);
    EXPECT_TRUE(file.is_open());
    restore_stdin_fd(old_stdin);
    Ballots ballots = read_file(file, false);
    file.close();

    userInput(file_names.at(9));
    ifstream file2;
    open_file(file2);
    EXPECT_TRUE(file2.is_open());
    restore_stdin_fd(old_stdin);
    Ballots ballotp = read_file(file2, false);
    file2.close();

    userInput(seatNums.at(0));
    prompt_user_seatNum(seatNum);
    EXPECT_EQ(seatNum, 21);
    restore_stdin_fd(old_stdin);

    STVelection = new STV(&ballots, seatNum);
    userInput(auditFile.at(0));
    STVelection->runElection(); 
    restore_stdin_fd(old_stdin);

    int winners_size = STVelection->getWinners().size();
    int losers_size = STVelection->getLosers().size();
    int candidates_size = STVelection->getCandidates().size();
    EXPECT_EQ(winners_size, candidates_size);
    EXPECT_EQ(losers_size, 0);

    pluralityElection = new Plurality(&ballotp, seatNum);

    pluralityElection->runElection();
    winners_size = pluralityElection->getWinners().size();
    losers_size = pluralityElection->getLosers().size();
    candidates_size = pluralityElection->getCandidates().size();
    EXPECT_EQ(winners_size, candidates_size);
    EXPECT_EQ(losers_size, 0);
};

TEST_F(SysTest, ballotShuffleValidity) {
    userInput(file_names.at(4));
    ifstream file;
    open_file(file);
    EXPECT_TRUE(file.is_open());
    restore_stdin_fd(old_stdin);
    Ballots ballots = read_file(file, false);
    file.close();

    for (int a = 0; a < shuffleValid; a++) {
        userInput(file_names.at(4));
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
    userInput(file_names.at(4));
    ifstream file;
    open_file(file);
    EXPECT_TRUE(file.is_open());
    restore_stdin_fd(old_stdin);
    Ballots ballots = read_file(file, false);
    file.close();

    userInput(file_names.at(4));
    ifstream file2;
    open_file(file2);
    EXPECT_TRUE(file2.is_open());
    restore_stdin_fd(old_stdin);
    Ballots shuffled = read_file(file2, true);
    file2.close();

    userInput(seatNums.at(2));
    prompt_user_seatNum(seatNum);
    EXPECT_EQ(seatNum, 1);
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

    vector<int> regWin(regWinners.size());
    vector<int> shfWin(shfWinners.size()); // Names are unreliable - we need to confirm ballot size.

    for (long unsigned int i = 0; i < regWinners.size(); i++) {
        regWin.at(i) = regWinners.at(i).getAssignedBallots().size();
        shfWin.at(i) = shfWinners.at(i).getAssignedBallots().size();
    }

    sort(regWin.begin(), regWin.end(), comp); // Consider winners and losers come in different times.
    sort(shfWin.begin(), shfWin.end(), comp);
    EXPECT_EQ(regWin, shfWin);
};

TEST_F(SysTest, fairElectionValidityPlurlaity) {
    int seatNum;
    userInput(file_names.at(5));
    ifstream file;
    open_file(file);
    EXPECT_TRUE(file.is_open());
    restore_stdin_fd(old_stdin);
    Ballots ballots = read_file(file, false);
    file.close();

    userInput(file_names.at(5));
    ifstream file2;
    open_file(file2);
    EXPECT_TRUE(file2.is_open());
    restore_stdin_fd(old_stdin);
    Ballots shuffled = read_file(file2, true);
    file2.close();

    userInput(seatNums.at(3));
    prompt_user_seatNum(seatNum);
    EXPECT_EQ(seatNum, 3);
    restore_stdin_fd(old_stdin);
    
    pluralityElection = new Plurality(&ballots, seatNum);
    pluralityElection->runElection(); 
    restore_stdin_fd(old_stdin);

    pluralityElectionS = new Plurality(&shuffled, seatNum);
    pluralityElectionS->runElection();
    restore_stdin_fd(old_stdin);


    vector<Candidate> regWinners = pluralityElection->getWinners();
    vector<Candidate> shfWinners = pluralityElectionS->getWinners();

    vector<int> regWin(regWinners.size());
    vector<int> shfWin(shfWinners.size()); // Names are unreliable - we need to confirm ballot size.

    for (long unsigned int i = 0; i < regWinners.size(); i++) {
        regWin.at(i) = regWinners.at(i).getAssignedBallots().size();
        shfWin.at(i) = shfWinners.at(i).getAssignedBallots().size();
    }

    sort(regWin.begin(), regWin.end(), comp); // Consider winners and losers come in different times.
    sort(shfWin.begin(), shfWin.end(), comp);
    EXPECT_EQ(regWin, shfWin);
};

TEST_F(SysTest, minElectionValidity) {
    int seatNum;
    userInput(file_names.at(0));
    ifstream file;
    open_file(file);
    EXPECT_TRUE(file.is_open());
    restore_stdin_fd(old_stdin);
    Ballots ballots = read_file(file, false);
    file.close();

    userInput(file_names.at(1));
    ifstream file2;
    open_file(file2);
    EXPECT_TRUE(file2.is_open());
    restore_stdin_fd(old_stdin);
    Ballots ballotp = read_file(file2, false);
    file2.close();

    userInput(seatNums.at(2));
    prompt_user_seatNum(seatNum);
    EXPECT_EQ(seatNum, 1);
    restore_stdin_fd(old_stdin);

    STVelection = new STV(&ballots, seatNum);
    userInput(auditFile.at(3));
    STVelection->runElection(); 
    restore_stdin_fd(old_stdin);

    vector<Candidate> winners = STVelection->getWinners();
    vector<Candidate> losers = STVelection->getLosers();
    EXPECT_EQ(winners.size(), 1);
    EXPECT_EQ(losers.size(), 0);
    Candidate winner = winners.at(0);
    EXPECT_EQ(winner.getBallotNum(), STVelection->getDroopQuota()); // Droop Quote, not total ballots!

    pluralityElection = new Plurality(&ballotp, seatNum);
    pluralityElection->runElection();

    winners = pluralityElection->getWinners();
    losers = pluralityElection->getLosers();
    EXPECT_EQ(winners.size(), 1);
    EXPECT_EQ(losers.size(), 0);
    winner = winners.at(0);
    EXPECT_EQ(winner.getBallotNum(), pluralityElection->getBallots()->getBallotCount());
};

TEST_F(SysTest, pluralityRegular) {
    int seatNum;
    userInput(file_names.at(5));
    ifstream file;
    open_file(file);
    EXPECT_TRUE(file.is_open());
    restore_stdin_fd(old_stdin);
    Ballots ballots = read_file(file, false);
    file.close();

    userInput(seatNums.at(3));
    prompt_user_seatNum(seatNum);
    EXPECT_EQ(seatNum, 3);
    restore_stdin_fd(old_stdin);

    pluralityElection = new Plurality(&ballots, seatNum);
    pluralityElection->runElection();

    vector<Candidate> winners = pluralityElection->getWinners();
    vector<Candidate> losers = pluralityElection->getLosers();
    cout << pluralityElection->getCandidates().size() << endl;
    int loser_size = pluralityElection->getCandidates().size() - seatNum;
    EXPECT_EQ(winners.size(), seatNum);
    EXPECT_EQ(losers.size(), loser_size);
    for (long unsigned int i = 0; i < winners.size(); i++) {
        for (long unsigned int j = 0; j < losers.size(); j++) {
            // Test all winners with all losers - should have at least the same, if not greater ballots
            EXPECT_GE(winners.at(i).getBallotNum(), losers.at(j).getBallotNum()); 
        }
    }
};

TEST_F(SysTest, pluralityTie) {
    int seatNum;
    userInput(file_names.at(10));
    ifstream file;
    open_file(file);
    EXPECT_TRUE(file.is_open());
    restore_stdin_fd(old_stdin);
    Ballots ballots = read_file(file, false);
    file.close();

    userInput(seatNums.at(3));
    prompt_user_seatNum(seatNum);
    EXPECT_EQ(seatNum, 3);
    restore_stdin_fd(old_stdin);

    pluralityElection = new Plurality(&ballots, seatNum);
    pluralityElection->runElection();

    vector<Candidate> winners = pluralityElection->getWinners();
    vector<Candidate> losers = pluralityElection->getLosers();
    int loser_size = pluralityElection->getCandidates().size() - seatNum;
    EXPECT_EQ(winners.size(), seatNum);
    EXPECT_EQ(losers.size(), loser_size);
    for (long unsigned int i = 0; i < winners.size(); i++) {
        for (long unsigned int j = 0; j < losers.size(); j++) {
            // Test all winners with all losers - should have the same ballots exactly.
            EXPECT_EQ(winners.at(i).getBallotNum(), losers.at(j).getBallotNum()); 
        }
    }
};

TEST_F(SysTest, stvRegular) {
    int seatNum;
    userInput(file_names.at(4));
    ifstream file;
    open_file(file);
    EXPECT_TRUE(file.is_open());
    restore_stdin_fd(old_stdin);
    Ballots ballots = read_file(file, false);
    file.close();

    userInput(seatNums.at(3));
    prompt_user_seatNum(seatNum);
    EXPECT_EQ(seatNum, 3);
    restore_stdin_fd(old_stdin);

    STVelection = new STV(&ballots, seatNum);
    userInput(auditFile.at(4));
    STVelection->runElection(); 
    restore_stdin_fd(old_stdin);

    vector<Candidate> winners = STVelection->getWinners();
    vector<Candidate> losers = STVelection->getLosers();
    int loser_size = STVelection->getCandidates().size() - seatNum;
    EXPECT_EQ(winners.size(), seatNum);
    EXPECT_EQ(losers.size(), loser_size);
    for (long unsigned int i = 0; i < winners.size(); i++) {
        // All winners should have met the Droop Quota.
        EXPECT_EQ(winners.at(i).getBallotNum(), STVelection->getDroopQuota());
        for (long unsigned int j = 0; j < losers.size(); j++) {
            // Test all winners with all losers - should have at least the same, if not greater ballots
            EXPECT_GT(winners.at(i).getBallotNum(), losers.at(j).getBallotNum()); 
        }
    }
    bool access(auditFile.at(3).at(0).c_str());
    EXPECT_TRUE(access);
};

TEST_F(SysTest, timeSTV) {
    auto t0 = chrono::high_resolution_clock::now();
    int seatNum; 
    userInput(file_names.at(8));
    ifstream file;
    open_file(file);
    EXPECT_TRUE(file.is_open());
    restore_stdin_fd(old_stdin);
    Ballots ballots = read_file(file, false);
    file.close();

    userInput(seatNums.at(4));
    prompt_user_seatNum(seatNum);
    EXPECT_EQ(seatNum, 10);
    restore_stdin_fd(old_stdin);

    STVelection = new STV(&ballots, seatNum);
    userInput(auditFile.at(5));
    STVelection->runElection(); 
    restore_stdin_fd(old_stdin);

    vector<Candidate> winners = STVelection->getWinners();
    vector<Candidate> losers = STVelection->getLosers();
    int loser_size = STVelection->getCandidates().size() - seatNum;
    EXPECT_EQ(winners.size(), seatNum);
    EXPECT_EQ(losers.size(), loser_size);
    for (long unsigned int i = 0; i < winners.size(); i++) {
        // All winners should have met the Droop Quota.
        EXPECT_EQ(winners.at(i).getBallotNum(), STVelection->getDroopQuota());
        for (long unsigned int j = 0; j < losers.size(); j++) {
            // Test all winners with all losers - should have at least the same, if not greater ballots
            EXPECT_GE(winners.at(i).getBallotNum(), losers.at(j).getBallotNum()); 
        }
    }
    bool access(auditFile.at(4).at(0).c_str());
    EXPECT_TRUE(access);
    auto t1 = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = t1 - t0;
    cout << fixed << setprecision(2) << diff.count() << " seconds to run." << endl;
    EXPECT_LE(diff.count(), 300.0);
};

TEST_F(SysTest, timePlurality) {
    auto t0 = chrono::high_resolution_clock::now();
    int seatNum; 
    userInput(file_names.at(9));
    ifstream file;
    open_file(file);
    EXPECT_TRUE(file.is_open());
    restore_stdin_fd(old_stdin);
    Ballots ballots = read_file(file, false);
    file.close();

    userInput(seatNums.at(4));
    prompt_user_seatNum(seatNum);
    EXPECT_EQ(seatNum, 10);
    restore_stdin_fd(old_stdin);

    pluralityElection = new Plurality(&ballots, seatNum);
    pluralityElection->runElection();

    vector<Candidate> winners = pluralityElection->getWinners();
    vector<Candidate> losers = pluralityElection->getLosers();
    int loser_size = pluralityElection->getCandidates().size() - seatNum;
    EXPECT_EQ(winners.size(), seatNum);
    EXPECT_EQ(losers.size(), loser_size);
    for (long unsigned int i = 0; i < winners.size(); i++) {
        for (long unsigned int j = 0; j < losers.size(); j++) {
            // Test all winners with all losers - should have at least the same, if not greater ballots
            EXPECT_GE(winners.at(i).getBallotNum(), losers.at(j).getBallotNum()); 
        }
    }
    auto t1 = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = t1 - t0;
    cout << fixed << setprecision(2) << diff.count() << " seconds to run." << endl;
    EXPECT_LE(diff.count(), 300.0);
};

TEST_F(SysTest, stvTie) {
    int seatNum;
    userInput(file_names.at(11));
    ifstream file;
    open_file(file);
    EXPECT_TRUE(file.is_open());
    restore_stdin_fd(old_stdin);
    Ballots ballots = read_file(file, false);
    file.close();

    userInput(seatNums.at(3));
    prompt_user_seatNum(seatNum);
    EXPECT_EQ(seatNum, 3);
    restore_stdin_fd(old_stdin);

    STVelection = new STV(&ballots, seatNum);
    userInput(auditFile.at(6));
    STVelection->runElection(); 
    restore_stdin_fd(old_stdin);

    vector<Candidate> winners = STVelection->getWinners();
    vector<Candidate> losers = STVelection->getLosers();
    int loser_size = STVelection->getCandidates().size() - seatNum;
    EXPECT_EQ(winners.size(), seatNum);
    EXPECT_EQ(losers.size(), loser_size);
    for (long unsigned int i = 0; i < winners.size(); i++) {
        // All winners should have met the Droop Quota.
        EXPECT_EQ(winners.at(i).getBallotNum(), STVelection->getDroopQuota());
        for (long unsigned int j = 0; j < losers.size(); j++) {
            // Test all winners with all losers - should be about the same - winners in losers bracket.
            EXPECT_GE(winners.at(i).getBallotNum(), losers.at(j).getBallotNum()); 
        }
    }
};

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
