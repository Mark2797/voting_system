#include "gtest/gtest.h"

#include "Driver.h"
#include "STV.h"
#include "Plurality.h"

// First input: Read file
// Second input: Number of seats
// Third input: Algo type
// Read screen output with winner/loser list

#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include <bits/stdc++.h>
#include <sstream>
#include <string>

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

void setArguments(int argc, char **argv, std::vector<std::string> &args) {
    for (int i = 0; i < argc; i++) {
        argv[i] = new char[args.at(i).length() + 1];
        strcpy(argv[i], args[i].c_str());
    }
}

void restore_stdin_fd(int old_stdin) {
    // restore the original FILENO
    dup2(old_stdin, STDIN_FILENO);
}

bool comp(int a, int b) {
    return a > b;
}

void looper(string loop_thru, vector<string> &result) {
    istringstream stream(loop_thru);
    string line;
    while(getline(stream, line)) {
        result.push_back(line);
    }
}

class SysTest : public ::testing::Test {
    protected:
        vector<string> file_names;
        vector<string> auditFile;
        int old_stdout = dup(STDOUT_FILENO);
        int old_stdin = dup(STDIN_FILENO);
        int shuffleValid = 1000;
        int argc_no_shuffle = 2;
        int argc_shuffle = 1;
        char **argv_no_shuffle = new char *[argc_no_shuffle];
        char **argv_shuffle = new char*[argc_shuffle];
        vector<string> name_no_shuffle = {"voting_system", "shuffle-off"};
        vector<string> name_shuffle = {"voting_system"};
    void SetUp() override {
        setArguments(argc_no_shuffle, argv_no_shuffle, name_no_shuffle);
        setArguments(argc_shuffle, argv_shuffle, name_shuffle);
        file_names = {"../testing/STV1.csv\n", "../testing/Plurality1.csv\n", "../testing/STV2.csv\n",
        "../testing/Plurality2.csv\n", "../testing/STV3.csv\n", "../testing/Plurality3.csv\n",
        "../testing/STV4.csv\n", "../testing/Plurality4.csv\n", "../testing/STV5.csv\n",
        "../testing/Plurality5.csv\n", "../testing/pluralityTie.csv\n", "../testing/stvTie.csv\n",
        "../testing/seatLowPlurality.csv\n", "../testing/seatLowSTV.csv\n", 
        "../testing/pluralitySurplus.csv\n",  "../testing/stvSurplus.csv\n"};
        auditFile = {"../testing/audits/seatValid.txt\n", "../testing/audits/noShuffle.txt\n", "../testing/audits/withShuffle.txt\n", 
        "../testing/audits/minSTV.txt\n", "../testing/audits/regSTV.txt\n", "../testing/audits/timeTest.txt\n", 
        "../testing/audits/stvTie.txt\n", "../testing/audits/lowVoteValid.txt\n"};
    }
    void TearDown() override {
        for (int i = 0; i < argc_no_shuffle; i++) {
            delete[] argv_no_shuffle[i];
        }
        for (int i = 0; i < argc_shuffle; i++) {
            delete[] argv_shuffle[i];
        }
        delete[] argv_shuffle;
        delete[] argv_no_shuffle;
    }
};

TEST_F(SysTest, seatValidityDefecitSTV) { // Test Case ID#: 27
    vector<string> user_input;
    user_input.push_back(file_names.at(13)); 
    user_input.push_back(auditFile.at(7));
    
    userInput(user_input);
    testing::internal::CaptureStdout();
    
    Driver driver = Driver();
    Election* election;
    Ballots* ballots;
    driver.run(argc_no_shuffle, argv_no_shuffle, election, ballots);
    string ret = testing::internal::GetCapturedStdout();
    restore_stdin_fd(old_stdin);
    vector<string> results;
    looper(ret, results);
    string correct = "Election type: STV\nNumber of seats: 21\nNumber of ballots: 1\nNumber of candidates: 20\nWinners:\n"
                     "Leon Kennedy\nLosers:\nChuck Lancaster\nMark Suckerberg\nAndrew Hero\nMicheal Ashton\nJoe Cool\nJimmy Donaldson\nJohn Kennedy\n"
                     "Patrick Star\nRobot Iam\nAlice Wonder\nBarack Obama\nJames Lancaster\nHaley Welsh\nJames Blake\nJohnny Bravo\nAdam Sandler\nHolly Summers\n"
                     "Polly Cracker\nAlbert Wesk";
    vector<string> expected_results;
    looper(correct, expected_results);
    vector<string> sub((results.begin() + 5), results.end());

    EXPECT_EQ(sub, expected_results);
    bool access(auditFile.at(7).c_str());
    EXPECT_TRUE(access);
    delete election;
    delete ballots;
};

TEST_F(SysTest, seatValidityDefecitPV) { // Test Case ID#: 28
    vector<string> user_input;
    user_input.push_back(file_names.at(12));

    userInput(user_input);
    testing::internal::CaptureStdout();

    Driver driver = Driver();
    Election* election;
    Ballots* ballots;
    driver.run(argc_no_shuffle, argv_no_shuffle, election, ballots);
    restore_stdin_fd(old_stdin);
    string ret = testing::internal::GetCapturedStdout();
    vector<string> results;
    looper(ret, results);
    results.erase(results.begin(), results.begin() + 3);

    vector<string> subWinners(results.begin() + 5, results.begin() + 25);
    vector<string> subLosers(results.begin() + 26, results.begin() + 26);
    vector<string> subPercentages(results.begin() + 27, results.end());

    sort(subWinners.begin(), subWinners.end());
    sort(subPercentages.begin(), subPercentages.end());

    EXPECT_EQ(results.at(0), "Election type: Plurality");
    EXPECT_EQ(results.at(1), "Number of seats: 21");
    EXPECT_EQ(results.at(2), "Number of ballots: 1");
    EXPECT_EQ(results.at(3), "Number of candidates: 20");
    EXPECT_EQ(results.at(4), "Winners:");
    EXPECT_EQ(results.at(25), "Losers:");
    EXPECT_EQ(results.at(26), "Percentage of votes:");
    for (int i = 0; i < 20; i++) {
        if (subWinners.at(i) == "Leon Kennedy") {
            EXPECT_EQ(subWinners.at(i) + " (100.00%)", subPercentages.at(i));
        } else {
            EXPECT_EQ(subWinners.at(i) + " (0.00%)", subPercentages.at(i));
        }
    }
    delete election;
    delete ballots;
};

TEST_F(SysTest, seatValiditySurplusPV) { // Test Case ID#: 29
    vector<string> user_input;
    user_input.push_back(file_names.at(14));

    userInput(user_input);
    testing::internal::CaptureStdout();

    Driver driver = Driver();
    Election* election;
    Ballots* ballots;
    driver.run(argc_no_shuffle, argv_no_shuffle, election, ballots);
    restore_stdin_fd(old_stdin);
    string ret = testing::internal::GetCapturedStdout();

    Plurality* PVelection = static_cast<Plurality*>(election);

    int winners_size = PVelection->getWinners().size();
    int losers_size = PVelection->getLosers().size();
    int candidates_size = PVelection->getCandidates().size();

    EXPECT_EQ(winners_size, candidates_size);
    EXPECT_EQ(losers_size, 0);
    delete election;
    delete ballots;
};

TEST_F(SysTest, seatValiditySurplusSTV) { // Test Case ID#: 30
    vector<string> user_input;
    user_input.push_back(file_names.at(15));
    user_input.push_back(auditFile.at(0));

    userInput(user_input);
    testing::internal::CaptureStdout();

    Driver driver = Driver();
    Election* election;
    Ballots* ballots;
    driver.run(argc_no_shuffle, argv_no_shuffle, election, ballots);
    restore_stdin_fd(old_stdin);
    string ret = testing::internal::GetCapturedStdout();

    int winners_size = static_cast<STV*>(election)->getWinners().size();
    int losers_size = static_cast<STV*>(election)->getLosers().size();
    int candidates_size = static_cast<STV*>(election)->getCandidates().size();

    EXPECT_EQ(winners_size, candidates_size);
    EXPECT_EQ(losers_size, 0);

    bool access(auditFile.at(0).c_str());
    EXPECT_TRUE(access);

    delete election;
    delete ballots;
};

TEST_F(SysTest, ballotShuffleValidity) { // Test Case ID#: 31
    vector<string> user_input;
    user_input.push_back(file_names.at(5));

    userInput(user_input);
    testing::internal::CaptureStdout();

    Driver driver = Driver();
    Election* election;
    Election* electionShuffle;
    Ballots* ballots;
    Ballots* ballotsShuffle;
    driver.run(argc_no_shuffle, argv_no_shuffle, election, ballots);
    restore_stdin_fd(old_stdin);
    string ret = testing::internal::GetCapturedStdout();

    Plurality* pluralityElection = dynamic_cast<Plurality*>(election);
    Ballots* ballot_1 = pluralityElection->getBallots();
    int count = ballot_1->getBallotCount();
    int not_equal = 0;

    for (int a = 0; a < shuffleValid; a++) {
        userInput(user_input);
        testing::internal::CaptureStdout();

        Driver driver2 = Driver();
        driver2.run(argc_shuffle, argv_shuffle, electionShuffle, ballotsShuffle);
        restore_stdin_fd(old_stdin);
        string ret2 = testing::internal::GetCapturedStdout();

        Plurality* pluralityElection2 = dynamic_cast<Plurality*>(electionShuffle);
        Ballots* ballot_2 = pluralityElection2->getBallots();
        
        
        for (int i = 0; i < count; i++) {
            vector<int> ballot = ballot_1->getBallot(i);
            vector<int> sballot = ballot_2->getBallot(i);
            if (ballot != sballot) not_equal++;
        }
        delete electionShuffle;
        delete ballotsShuffle;
    }
    int avg_shuffle = not_equal / 1000;
    cout << avg_shuffle << "% average for shuffling" << endl;
    EXPECT_GE(avg_shuffle, 75); // 75% shuffle average overall.
    delete election;
    delete ballots;
};

TEST_F(SysTest, fairElectionValiditySTV) { // Test Case ID#: 32
    vector<string> user_input;
    user_input.push_back(file_names.at(6));
    user_input.push_back(auditFile.at(1));

    userInput(user_input);
    testing::internal::CaptureStdout();

    Driver driver = Driver();
    Election* election;
    Ballots* ballots;
    driver.run(argc_no_shuffle, argv_no_shuffle, election, ballots);
    restore_stdin_fd(old_stdin);
    string ret = testing::internal::GetCapturedStdout();

    STV* STVelection = static_cast<STV*>(election);

    vector<Candidate> winners = STVelection->getWinners();

    vector<string> user_inputS;
    user_inputS.push_back(file_names.at(6));
    user_inputS.push_back(auditFile.at(2));
    userInput(user_inputS);
    testing::internal::CaptureStdout();

    Election* electionShuffle;
    Ballots* ballotsShuffle;
    driver.run(argc_shuffle, argv_shuffle, electionShuffle, ballotsShuffle);
    restore_stdin_fd(old_stdin);
    string ret2 = testing::internal::GetCapturedStdout();

    STV* STVelectionS = static_cast<STV*>(electionShuffle);

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

    bool access(auditFile.at(1).c_str());
    EXPECT_TRUE(access);

    delete election;
    delete ballots;
    delete electionShuffle;
    delete ballotsShuffle;
};

TEST_F(SysTest, fairElectionValidityPlurality) { // Test Case ID#: 33
    vector<string> user_input;
    user_input.push_back(file_names.at(7));

    userInput(user_input);
    testing::internal::CaptureStdout();

    Driver driver = Driver();
    Election* election;
    Ballots* ballots;
    driver.run(argc_no_shuffle, argv_no_shuffle, election, ballots);
    restore_stdin_fd(old_stdin);
    string ret = testing::internal::GetCapturedStdout();

    vector<string> results;
    looper(ret, results);
    results.erase(results.begin(), results.begin() + 3);

    vector<string> subWinners(results.begin() + 5, results.begin() + 10);
    vector<string> subLosers(results.begin() + 11, results.begin() + 16);
    vector<string> subPercentages(results.begin() + 17, results.begin() + 27);

    sort(subWinners.begin(), subWinners.end());
    sort(subLosers.begin(), subLosers.end());
    sort(subPercentages.begin(), subPercentages.end());

    EXPECT_EQ(results.at(0), "Election type: Plurality");
    EXPECT_EQ(results.at(1), "Number of seats: 5");
    EXPECT_EQ(results.at(2), "Number of ballots: 1000");
    EXPECT_EQ(results.at(3), "Number of candidates: 10");
    EXPECT_EQ(results.at(4), "Winners:");
    EXPECT_EQ(results.at(10), "Losers:");
    EXPECT_EQ(results.at(16), "Percentage of votes:");
    EXPECT_EQ(subWinners.size(), 5);
    EXPECT_EQ(subLosers.size(), 5);
    EXPECT_EQ(subPercentages.size(), 10);


    userInput(user_input);
    testing::internal::CaptureStdout();

    Election* electionShuffle;
    Ballots* ballotsShuffle;
    driver.run(argc_shuffle, argv_shuffle, electionShuffle, ballotsShuffle);
    restore_stdin_fd(old_stdin);
    string retS = testing::internal::GetCapturedStdout();
    vector<string> resultsS;
    looper(retS, resultsS);
    resultsS.erase(resultsS.begin(), resultsS.begin() + 2);

    vector<string> subWinnersS(resultsS.begin() + 5, resultsS.begin() + 10);
    vector<string> subLosersS(resultsS.begin() + 11, resultsS.begin() + 16);
    vector<string> subPercentagesS(resultsS.begin() + 17, resultsS.begin() + 27);

    sort(subWinnersS.begin(), subWinnersS.end());
    sort(subLosersS.begin(), subLosersS.end());
    sort(subPercentagesS.begin(), subPercentagesS.end());

    EXPECT_EQ(resultsS.at(0), "Election type: Plurality");
    EXPECT_EQ(resultsS.at(1), "Number of seats: 5");
    EXPECT_EQ(resultsS.at(2), "Number of ballots: 1000");
    EXPECT_EQ(resultsS.at(3), "Number of candidates: 10");
    EXPECT_EQ(resultsS.at(4), "Winners:");
    EXPECT_EQ(resultsS.at(10), "Losers:");
    EXPECT_EQ(resultsS.at(16), "Percentage of votes:");
    EXPECT_EQ(subWinnersS.size(), 5);
    EXPECT_EQ(subLosersS.size(), 5);
    EXPECT_EQ(subPercentagesS.size(), 10);
    for (int i = 0; i < static_cast<int>(subWinners.size()); i++) {
        EXPECT_EQ(subWinners.at(i), subWinnersS.at(i));
    }
    for (int i = 0; i < static_cast<int>(subLosers.size()); i++) {
        EXPECT_EQ(subLosers.at(i), subLosersS.at(i));
    }
    for (int i = 0; i < static_cast<int>(subPercentages.size()); i++) {
        EXPECT_EQ(subPercentages.at(i), subPercentagesS.at(i));
    }
    delete election;
    delete ballots;
    delete electionShuffle;
    delete ballotsShuffle;
};


TEST_F (SysTest, minElectionValiditySTV) { // Test Case ID#: 34
    vector<string> user_input;
    user_input.push_back(file_names.at(0));
    user_input.push_back(auditFile.at(3));

    userInput(user_input);
    testing::internal::CaptureStdout();

    Driver driver = Driver();
    Election* election;
    Ballots* ballots;
    driver.run(argc_no_shuffle, argv_no_shuffle, election, ballots);
    restore_stdin_fd(old_stdin);
    string ret = testing::internal::GetCapturedStdout();

    int winners_size = static_cast<STV*>(election)->getWinners().size();
    int losers_size = static_cast<STV*>(election)->getLosers().size();
    int candidates_size = static_cast<STV*>(election)->getCandidates().size();

    EXPECT_EQ(winners_size, candidates_size);
    EXPECT_EQ(losers_size, 0);

    bool access(auditFile.at(3).c_str());
    EXPECT_TRUE(access);

    delete election;
    delete ballots;
};

TEST_F(SysTest, minElectionValidityPV) { // Test Case ID#: 35
    vector<string> user_input;
    user_input.push_back(file_names.at(1));

    userInput(user_input);
    testing::internal::CaptureStdout();

    Driver driver = Driver();
    Election* election;
    Ballots* ballots;
    driver.run(argc_no_shuffle, argv_no_shuffle, election, ballots);
    restore_stdin_fd(old_stdin);
    string ret = testing::internal::GetCapturedStdout();

    int winners_size = static_cast<Plurality*>(election)->getWinners().size();
    int losers_size = static_cast<Plurality*>(election)->getLosers().size();
    int candidates_size = static_cast<Plurality*>(election)->getCandidates().size();

    EXPECT_EQ(winners_size, candidates_size);
    EXPECT_EQ(losers_size, 0);
    delete election;
    delete ballots;
};

TEST_F(SysTest, pluralityRegular) { // Test Case ID#: 36
    // Rework to use PV4
    vector<string> user_input;
    user_input.push_back(file_names.at(7));

    userInput(user_input);
    testing::internal::CaptureStdout();

    Driver driver = Driver();
    Election* election;
    Ballots* ballots;
    driver.run(argc_no_shuffle, argv_no_shuffle, election, ballots);
    restore_stdin_fd(old_stdin);
    string ret = testing::internal::GetCapturedStdout();

    Plurality* PVelection = static_cast<Plurality*>(election);
    vector<Candidate> winners = PVelection->getWinners();
    vector<Candidate> losers = PVelection->getLosers();
    int seatNum = PVelection->getSeats();
    int loser_size = PVelection->getCandidates().size() - seatNum;

    EXPECT_EQ(seatNum, PVelection->getSeats());
    EXPECT_EQ(winners.size(), seatNum);
    EXPECT_EQ(losers.size(), loser_size);
    for (long unsigned int i = 0; i < winners.size(); i++) {
        for (long unsigned int j = 0; j < losers.size(); j++) {
            // Test all winners with all losers - should have at least the same, if not greater ballots
            EXPECT_GE(winners.at(i).getBallotNum(), losers.at(j).getBallotNum()); 
        }
    }
    delete election;
    delete ballots;
};

TEST_F(SysTest, pluralityTie) { // Test Case ID#: 37
    vector<string> user_input;
    user_input.push_back(file_names.at(10));

    userInput(user_input);
    testing::internal::CaptureStdout();

    Driver driver = Driver();
    Election* election;
    Ballots* ballots;
    driver.run(argc_no_shuffle, argv_no_shuffle, election, ballots);
    restore_stdin_fd(old_stdin);
    string ret = testing::internal::GetCapturedStdout();
    vector<string> results;
    looper(ret, results);
    results.erase(results.begin(), results.begin() + 3);

    vector<string> subWinners(results.begin() + 5, results.begin() + 8);
    vector<string> subLosers(results.begin() + 9, results.begin() + 11);
    vector<string> subPercentages(results.begin() + 12, results.begin() + 17);

    sort(subWinners.begin(), subWinners.end());
    sort(subLosers.begin(), subLosers.end());
    sort(subPercentages.begin(), subPercentages.end());

    EXPECT_EQ(results.at(0), "Election type: Plurality");
    EXPECT_EQ(results.at(1), "Number of seats: 3");
    EXPECT_EQ(results.at(2), "Number of ballots: 200");
    EXPECT_EQ(results.at(3), "Number of candidates: 5");
    EXPECT_EQ(results.at(4), "Winners:");
    EXPECT_EQ(results.at(8), "Losers:");
    EXPECT_EQ(results.at(11), "Percentage of votes:");
    EXPECT_EQ(subWinners.size(), 3);
    EXPECT_EQ(subLosers.size(), 2);
    EXPECT_EQ(subPercentages.size(), 5);
    for (int i = 0; i < static_cast<int>(subPercentages.size()); i++) {
        string percent = subPercentages.at(i).substr(subPercentages.at(i).length() - 8, 8);
        EXPECT_EQ(percent, "(20.00%)");
    }
    delete election;
    delete ballots;
};

TEST_F(SysTest, stvRegular) { // Test Case ID#: 38
    // Rework to use STV4
    vector<string> user_input;
    user_input.push_back(file_names.at(6));
    user_input.push_back(auditFile.at(4));

    userInput(user_input);
    testing::internal::CaptureStdout();

    Driver driver = Driver();
    Election* election;
    Ballots* ballots;
    driver.run(argc_no_shuffle, argv_no_shuffle, election, ballots);
    restore_stdin_fd(old_stdin);
    string ret = testing::internal::GetCapturedStdout();

    STV* STVelection = static_cast<STV*>(election);

    vector<Candidate> winners = STVelection->getWinners();
    vector<Candidate> losers = STVelection->getLosers();
    int seatNum = STVelection->getSeats();
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
    bool access(auditFile.at(3).c_str());
    EXPECT_TRUE(access);
    delete election;
    delete ballots;
};
TEST_F(SysTest, stvTie) { // Test Case ID#: 39
    vector<string> user_input;
    user_input.push_back(file_names.at(11));
    user_input.push_back(auditFile.at(6));

    userInput(user_input);
    testing::internal::CaptureStdout();

    Driver driver = Driver();
    Election* election;
    Ballots* ballots;
    driver.run(argc_no_shuffle, argv_no_shuffle, election, ballots);
    restore_stdin_fd(old_stdin);
    string ret = testing::internal::GetCapturedStdout();

    STV* STVelection = static_cast<STV*>(election);

    vector<Candidate> winners = STVelection->getWinners();
    vector<Candidate> losers = STVelection->getLosers();
    int seatNum = STVelection->getSeats();
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
    bool access(auditFile.at(6).c_str());
    EXPECT_TRUE(access);
    delete election;
    delete ballots;
};

TEST_F(SysTest, timeSTV) { // Test Case ID#: 40
    auto t0 = chrono::high_resolution_clock::now();

    vector<string> user_input;
    user_input.push_back(file_names.at(8));
    user_input.push_back(auditFile.at(5));

    userInput(user_input);
    testing::internal::CaptureStdout();

    Driver driver = Driver();
    Election* election;
    Ballots* ballots;
    driver.run(argc_no_shuffle, argv_no_shuffle, election, ballots);
    restore_stdin_fd(old_stdin);
    string ret = testing::internal::GetCapturedStdout();

    STV* STVelection = static_cast<STV*>(election);

    vector<Candidate> winners = STVelection->getWinners();
    vector<Candidate> losers = STVelection->getLosers();
    int seatNum = STVelection->getSeats();
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
    bool access(auditFile.at(4).c_str());
    EXPECT_TRUE(access);
    auto t1 = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = t1 - t0;
    cout << fixed << setprecision(2) << diff.count() << " seconds to run." << endl;
    EXPECT_LE(diff.count(), 300.0);
    delete election;
    delete ballots;
};

TEST_F(SysTest, timePlurality) { // Test Case ID#: 41
    // TODO FIX
    auto t0 = chrono::high_resolution_clock::now();

    vector<string> user_input;
    user_input.push_back(file_names.at(9));

    userInput(user_input);
    testing::internal::CaptureStdout();

    Driver driver = Driver();
    Election* election;
    Ballots* ballots;
    driver.run(argc_no_shuffle, argv_no_shuffle, election, ballots);
    restore_stdin_fd(old_stdin);
    string ret = testing::internal::GetCapturedStdout();
    vector<string> results;
    looper(ret, results);
    results.erase(results.begin(), results.begin() + 3);

    vector<string> subWinners(results.begin() + 5, results.begin() + 15);
    vector<string> subLosers(results.begin() + 16, results.begin() + 26);
    vector<string> subPercentages(results.begin() + 27, results.begin() + 47);

    sort(subWinners.begin(), subWinners.end());
    sort(subPercentages.begin(), subPercentages.end());

    EXPECT_EQ(results.at(0), "Election type: Plurality");
    EXPECT_EQ(results.at(1), "Number of seats: 10");
    EXPECT_EQ(results.at(2), "Number of ballots: 100000");
    EXPECT_EQ(results.at(3), "Number of candidates: 20");
    EXPECT_EQ(results.at(4), "Winners:");
    EXPECT_EQ(results.at(15), "Losers:");
    EXPECT_EQ(results.at(26), "Percentage of votes:");
    EXPECT_EQ(subWinners.size(), 10);
    EXPECT_EQ(subLosers.size(), 10);
    EXPECT_EQ(subPercentages.size(), 20);

    auto t1 = chrono::high_resolution_clock::now();

    chrono::duration<double> diff = t1 - t0;
    cout << fixed << setprecision(2) << diff.count() << " seconds to run." << endl;
    EXPECT_LE(diff.count(), 300.0);
    delete election;
    delete ballots;
};

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
