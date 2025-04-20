#include "gtest/gtest.h"

#include "Driver.h"

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

// extern void open_file(std::ifstream& file);
// extern Ballots read_file(std::ifstream& file, bool shuffle);
// extern int shuffleOffFlag(int argc, char **argv, bool &shuffle);
// extern void prompt_user_seatNum(int& seatNum);
// extern void prompt_user_alg(std::string& alg);

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
        vector<string> algo;
        vector<string> file_names;
        vector<string> seatNums;
        vector<string> auditFile;
        int old_stdout = dup(STDOUT_FILENO);
        int old_stdin = dup(STDIN_FILENO);
        int shuffleValid = 1000;
        int argc = 1;
        char **argv = new char *[argc];
        vector<string> name = {"voting_system"};
    void SetUp() override {
        setArguments(argc, argv, name);
        file_names = {"../testing/STV1.csv\n", "../testing/Plurality1.csv\n", "../testing/STV2.csv\n",
        "../testing/Plurality2.csv\n", "../testing/STV3.csv\n", "../testing/Plurality3.csv\n",
        "../testing/STV4.csv\n", "../testing/Plurality4.csv\n", "../testing/STV5.csv\n",
        "../testing/Plurality5.csv\n", "../testing/pluralityTie.csv\n", "../testing/stvTie.csv\n",
        "../testing/seatLowPlurality.csv\n", "../testing/seatLowSTV.csv\n"};
        seatNums = {"21\n", "5\n", "1\n", "3\n", "10\n"};
        auditFile = {"../testing/audits/seatValid.txt\n", "../testing/audits/noShuffle.txt\n", "../testing/audits/withShuffle.txt\n", 
        "../testing/audits/minSTV.txt\n", "../testing/audits/regSTV.txt\n", "../testing/audits/timeTest.txt\n", 
        "../testing/audits/stvTie.txt\n", "../testing/audits/lowVoteValid.txt\n"};
        algo = {"1\n", "2\n"};
    }
    void TearDown() override {
        for (int i = 0; i < argc; i++) {
            delete[] argv[i];
        }
        delete[] argv;
    }
};

TEST_F(SysTest, seatValidityDefecitSTV) {
    vector<string> user_input;
    user_input.push_back(file_names.at(13)); // TODO FIX STRING VECTOR VARIABLES
    user_input.push_back(seatNums.at(0));
    user_input.push_back(algo.at(1));
    user_input.push_back(auditFile.at(7));
    
    userInput(user_input);
    testing::internal::CaptureStdout();
    
    Driver driver = Driver();
    driver.run(argc, argv);
    restore_stdin_fd(old_stdin);
    string ret = testing::internal::GetCapturedStdout();
    vector<string> results;
    looper(ret, results);
    string correct = "Election type: STV\nNumber of seats: 21\nNumber of ballots: 1\nNumber of candidates: 20\nWinners:\n"
                     "Leon Kennedy\nLosers:\nChuck Lancaster\nMark Suckerberg\nAndrew Hero\nMicheal Ashton\nJoe Cool\nJimmy Donaldson\nJohn Kennedy\n"
                     "Patrick Star\nRobot Iam\nAlice Wonder\nBarack Obama\nJames Lancaster\nHaley Welsh\nJames Blake\nJohnny Bravo\nAdam Sandler\nHolly Summers\n"
                     "Polly Cracker\nAlbert Wesk";
    vector<string> expected_results;
    looper(correct, expected_results);
    vector<string> sub((results.begin() + 9), results.end());

    EXPECT_EQ(sub, expected_results);
};

TEST_F(SysTest, seatValidityDefecitPV) { // Test Case ID#: 38
    vector<string> user_input;
    user_input.push_back(file_names.at(12));
    user_input.push_back(seatNums.at(0));
    user_input.push_back(algo.at(0));

    userInput(user_input);
    testing::internal::CaptureStdout();

    Driver driver = Driver();
    driver.run(argc, argv);
    restore_stdin_fd(old_stdin);
    string ret = testing::internal::GetCapturedStdout();
    vector<string> results;
    looper(ret, results);
    results.erase(results.begin(), results.begin() + 7);

    vector<string> subWinners(results.begin() + 5, results.begin() + 25);
    vector<string> subLosers(results.begin() + 26, results.begin() + 26);
    vector<string> subPercentages(results.begin() + 27, results.begin() + 47);

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
};

TEST_F(SysTest, seatValidityDefecitPVSmall) { // Test Case ID#: 37
    vector<string> user_input;
    user_input.push_back(file_names.at(9));
    user_input.push_back(seatNums.at(0));
    user_input.push_back(algo.at(0));

    userInput(user_input);
    testing::internal::CaptureStdout();

    Driver driver = Driver();
    driver.run(argc, argv);
    restore_stdin_fd(old_stdin);
    string ret = testing::internal::GetCapturedStdout();
    vector<string> results;
    looper(ret, results);
    results.erase(results.begin(), results.begin() + 7);

    vector<string> subWinners(results.begin() + 5, results.begin() + 25);
    vector<string> subLosers(results.begin() + 26, results.begin() + 26);
    vector<string> subPercentages(results.begin() + 27, results.begin() + 47);

    sort(subWinners.begin(), subWinners.end());
    sort(subPercentages.begin(), subPercentages.end());

    EXPECT_EQ(results.at(0), "Election type: Plurality");
    EXPECT_EQ(results.at(1), "Number of seats: 21");
    EXPECT_EQ(results.at(2), "Number of ballots: 100000");
    EXPECT_EQ(results.at(3), "Number of candidates: 20");
    EXPECT_EQ(results.at(4), "Winners:");
    EXPECT_EQ(results.at(25), "Losers:");
    EXPECT_EQ(results.at(26), "Percentage of votes:");
    EXPECT_EQ(subWinners.size(), 20);
    EXPECT_EQ(subLosers.size(), 0);
    EXPECT_EQ(subPercentages.size(), 20);
};

// TEST_F(SysTest, seatValiditySurplus) {
//     int seatNum;
//     userInput(file_names.at(8));
//     ifstream file;
//     open_file(file);
//     EXPECT_TRUE(file.is_open());

//     Ballots ballots = read_file(file, false);
//     file.close();

//     userInput(file_names.at(9));
//     ifstream file2;
//     open_file(file2);
//     EXPECT_TRUE(file2.is_open());
//     restore_stdin_fd(old_stdin);
//     Ballots ballotp = read_file(file2, false);
//     file2.close();

//     userInput(seatNums.at(0));
//     prompt_user_seatNum(seatNum);
//     EXPECT_EQ(seatNum, 21);
//     restore_stdin_fd(old_stdin);

//     STVelection = new STV(&ballots, seatNum);
//     userInput(auditFile.at(0));
//     STVelection->runElection(); 
//     restore_stdin_fd(old_stdin);

//     int winners_size = STVelection->getWinners().size();
//     int losers_size = STVelection->getLosers().size();
//     int candidates_size = STVelection->getCandidates().size();
//     EXPECT_EQ(winners_size, candidates_size);
//     EXPECT_EQ(losers_size, 0);

//     pluralityElection = new Plurality(&ballotp, seatNum);

//     pluralityElection->runElection();
//     winners_size = pluralityElection->getWinners().size();
//     losers_size = pluralityElection->getLosers().size();
//     candidates_size = pluralityElection->getCandidates().size();
//     EXPECT_EQ(winners_size, candidates_size);
//     EXPECT_EQ(losers_size, 0);
// };

// TEST_F(SysTest, ballotShuffleValidity) {
//     userInput(file_names.at(4));
//     ifstream file;
//     open_file(file);
//     EXPECT_TRUE(file.is_open());
//     restore_stdin_fd(old_stdin);
//     Ballots ballots = read_file(file, false);
//     file.close();

//     for (int a = 0; a < shuffleValid; a++) {
//         userInput(file_names.at(4));
//         ifstream file2;
//         open_file(file2);
//         EXPECT_TRUE(file2.is_open());
//         restore_stdin_fd(old_stdin);
//         Ballots shuffled = read_file(file2, true);
//         file2.close();

//         int count = ballots.getBallotCount();
//         int not_equal = 0;
//         for (int i = 0 ; i < count; i++) {
//             vector<int> ballot = ballots.getBallot(i);
//             vector<int> sballot = shuffled.getBallot(i);
//             if (ballot != sballot) not_equal++;
//         }
//         cout << not_equal << "% shuffled" << endl;
//         EXPECT_GE(not_equal, 90); // Covers that the ballots are at minimum 90% shuffled
//     }
// };

// TEST_F(SysTest, fairElectionValiditySTV) {
//     int seatNum;
//     userInput(file_names.at(4));
//     ifstream file;
//     open_file(file);
//     EXPECT_TRUE(file.is_open());
//     restore_stdin_fd(old_stdin);
//     Ballots ballots = read_file(file, false);
//     file.close();

//     userInput(file_names.at(4));
//     ifstream file2;
//     open_file(file2);
//     EXPECT_TRUE(file2.is_open());
//     restore_stdin_fd(old_stdin);
//     Ballots shuffled = read_file(file2, true);
//     file2.close();

//     userInput(seatNums.at(2));
//     prompt_user_seatNum(seatNum);
//     EXPECT_EQ(seatNum, 1);
//     restore_stdin_fd(old_stdin);

//     STVelection = new STV(&ballots, seatNum);
//     userInput(auditFile.at(1));
//     STVelection->runElection(); 
//     restore_stdin_fd(old_stdin);

//     STVelectionS = new STV(&shuffled, seatNum);
//     userInput(auditFile.at(2));
//     STVelectionS->runElection();
//     restore_stdin_fd(old_stdin);

//     vector<Candidate> regWinners = STVelection->getWinners();
//     vector<Candidate> shfWinners = STVelectionS->getWinners();

//     vector<int> regWin(regWinners.size());
//     vector<int> shfWin(shfWinners.size()); // Names are unreliable - we need to confirm ballot size.

//     for (long unsigned int i = 0; i < regWinners.size(); i++) {
//         regWin.at(i) = regWinners.at(i).getAssignedBallots().size();
//         shfWin.at(i) = shfWinners.at(i).getAssignedBallots().size();
//     }

//     sort(regWin.begin(), regWin.end(), comp); // Consider winners and losers come in different times.
//     sort(shfWin.begin(), shfWin.end(), comp);
//     EXPECT_EQ(regWin, shfWin);
// };

TEST_F(SysTest, fairElectionValidityPlurality) { // Test Case ID#: 29
    vector<string> user_input;
    user_input.push_back(file_names.at(5));
    user_input.push_back(seatNums.at(3));
    user_input.push_back(algo.at(0));

    userInput(user_input);
    testing::internal::CaptureStdout();

    Driver driver = Driver();
    driver.run(argc, argv);
    restore_stdin_fd(old_stdin);
    string ret = testing::internal::GetCapturedStdout();
    vector<string> results;
    looper(ret, results);
    results.erase(results.begin(), results.begin() + 7);

    vector<string> subWinners(results.begin() + 5, results.begin() + 8);
    vector<string> subLosers(results.begin() + 9, results.begin() + 11);
    vector<string> subPercentages(results.begin() + 12, results.begin() + 17);

    sort(subWinners.begin(), subWinners.end());
    sort(subLosers.begin(), subLosers.end());
    sort(subPercentages.begin(), subPercentages.end());

    EXPECT_EQ(results.at(0), "Election type: Plurality");
    EXPECT_EQ(results.at(1), "Number of seats: 3");
    EXPECT_EQ(results.at(2), "Number of ballots: 100");
    EXPECT_EQ(results.at(3), "Number of candidates: 5");
    EXPECT_EQ(results.at(4), "Winners:");
    EXPECT_EQ(results.at(8), "Losers:");
    EXPECT_EQ(results.at(11), "Percentage of votes:");
    EXPECT_EQ(subWinners.size(), 3);
    EXPECT_EQ(subLosers.size(), 2);
    EXPECT_EQ(subPercentages.size(), 5);

    for (int i = 0; i < argc; i++) {
        delete[] argv[i];
    }
    delete[] argv;

    name.push_back("shuffle-off");
    argc = 2;
    argv = new char *[argc];
    setArguments(argc, argv, name);

    userInput(user_input);
    testing::internal::CaptureStdout();

    Driver driverS = Driver();
    driverS.run(argc, argv);
    restore_stdin_fd(old_stdin);
    string retS = testing::internal::GetCapturedStdout();
    vector<string> resultsS;
    looper(retS, resultsS);
    resultsS.erase(resultsS.begin(), resultsS.begin() + 8);

    vector<string> subWinnersS(resultsS.begin() + 5, resultsS.begin() + 8);
    vector<string> subLosersS(resultsS.begin() + 9, resultsS.begin() + 11);
    vector<string> subPercentagesS(resultsS.begin() + 12, resultsS.begin() + 17);

    sort(subWinnersS.begin(), subWinnersS.end());
    sort(subLosersS.begin(), subLosersS.end());
    sort(subPercentagesS.begin(), subPercentagesS.end());

    EXPECT_EQ(resultsS.at(0), "Election type: Plurality");
    EXPECT_EQ(resultsS.at(1), "Number of seats: 3");
    EXPECT_EQ(resultsS.at(2), "Number of ballots: 100");
    EXPECT_EQ(resultsS.at(3), "Number of candidates: 5");
    EXPECT_EQ(resultsS.at(4), "Winners:");
    EXPECT_EQ(resultsS.at(8), "Losers:");
    EXPECT_EQ(resultsS.at(11), "Percentage of votes:");
    EXPECT_EQ(subWinnersS.size(), 3);
    EXPECT_EQ(subLosersS.size(), 2);
    EXPECT_EQ(subPercentagesS.size(), 5);
    for (int i = 0; i < static_cast<int>(subWinners.size()); i++) {
        EXPECT_EQ(subWinners.at(i), subWinnersS.at(i));
    }
    for (int i = 0; i < static_cast<int>(subLosers.size()); i++) {
        EXPECT_EQ(subLosers.at(i), subLosersS.at(i));
    }
    for (int i = 0; i < static_cast<int>(subPercentages.size()); i++) {
        EXPECT_EQ(subPercentages.at(i), subPercentagesS.at(i));
    }
};

// TEST_F(SysTest, minElectionValidity) {
//     int seatNum;
//     userInput(file_names.at(0));
//     ifstream file;
//     open_file(file);
//     EXPECT_TRUE(file.is_open());
//     restore_stdin_fd(old_stdin);
//     Ballots ballots = read_file(file, false);
//     file.close();

//     userInput(file_names.at(1));
//     ifstream file2;
//     open_file(file2);
//     EXPECT_TRUE(file2.is_open());
//     restore_stdin_fd(old_stdin);
//     Ballots ballotp = read_file(file2, false);
//     file2.close();

//     userInput(seatNums.at(2));
//     prompt_user_seatNum(seatNum);
//     EXPECT_EQ(seatNum, 1);
//     restore_stdin_fd(old_stdin);

//     STVelection = new STV(&ballots, seatNum);
//     userInput(auditFile.at(3));
//     STVelection->runElection(); 
//     restore_stdin_fd(old_stdin);

//     vector<Candidate> winners = STVelection->getWinners();
//     vector<Candidate> losers = STVelection->getLosers();
//     EXPECT_EQ(winners.size(), 1);
//     EXPECT_EQ(losers.size(), 0);
//     Candidate winner = winners.at(0);
//     EXPECT_EQ(winner.getBallotNum(), STVelection->getDroopQuota()); // Droop Quote, not total ballots!

//     pluralityElection = new Plurality(&ballotp, seatNum);
//     pluralityElection->runElection();

//     winners = pluralityElection->getWinners();
//     losers = pluralityElection->getLosers();
//     EXPECT_EQ(winners.size(), 1);
//     EXPECT_EQ(losers.size(), 0);
//     winner = winners.at(0);
//     EXPECT_EQ(winner.getBallotNum(), pluralityElection->getBallots()->getBallotCount());
// };

TEST_F(SysTest, minElectionValidityPV) { // Test Case ID#: 30
    vector<string> user_input;
    user_input.push_back(file_names.at(1));
    user_input.push_back(seatNums.at(2));
    user_input.push_back(algo.at(0));

    userInput(user_input);
    testing::internal::CaptureStdout();

    Driver driver = Driver();
    driver.run(argc, argv);
    restore_stdin_fd(old_stdin);
    string ret = testing::internal::GetCapturedStdout();
    vector<string> results;
    looper(ret, results);
    results.erase(results.begin(), results.begin() + 7);

    vector<string> subWinners(results.begin() + 5, results.begin() + 6);
    vector<string> subLosers(results.begin() + 6, results.begin() + 6);
    vector<string> subPercentages(results.begin() + 7, results.begin() + 8);

    sort(subWinners.begin(), subWinners.end());
    sort(subLosers.begin(), subLosers.end());
    sort(subPercentages.begin(), subPercentages.end());

    EXPECT_EQ(results.at(0), "Election type: Plurality");
    EXPECT_EQ(results.at(1), "Number of seats: 1");
    EXPECT_EQ(results.at(2), "Number of ballots: 1");
    EXPECT_EQ(results.at(3), "Number of candidates: 1");
    EXPECT_EQ(results.at(4), "Winners:");
    EXPECT_EQ(results.at(5), "Chuck Lancaster");
    EXPECT_EQ(results.at(6), "Losers:");
    EXPECT_EQ(results.at(7), "Percentage of votes:");
    EXPECT_EQ(results.at(8), "Chuck Lancaster (100.00%)");
    EXPECT_EQ(subWinners.size(), 1);
    EXPECT_EQ(subLosers.size(), 0);
    EXPECT_EQ(subPercentages.size(), 1);
};

TEST_F(SysTest, pluralityRegular) { // Test Case ID#: 31
    vector<string> user_input;
    user_input.push_back(file_names.at(5));
    user_input.push_back(seatNums.at(3));
    user_input.push_back(algo.at(0));

    userInput(user_input);
    testing::internal::CaptureStdout();

    Driver driver = Driver();
    driver.run(argc, argv);
    restore_stdin_fd(old_stdin);
    string ret = testing::internal::GetCapturedStdout();
    vector<string> results;
    looper(ret, results);
    results.erase(results.begin(), results.begin() + 7);

    vector<string> subWinners(results.begin() + 5, results.begin() + 8);
    vector<string> subLosers(results.begin() + 9, results.begin() + 11);
    vector<string> subPercentages(results.begin() + 12, results.end());

    sort(subWinners.begin(), subWinners.end());
    sort(subLosers.begin(), subLosers.end());
    sort(subPercentages.begin(), subPercentages.end());

    EXPECT_EQ(results.at(0), "Election type: Plurality");
    EXPECT_EQ(results.at(1), "Number of seats: 3");
    EXPECT_EQ(results.at(2), "Number of ballots: 100");
    EXPECT_EQ(results.at(3), "Number of candidates: 5");
    EXPECT_EQ(results.at(4), "Winners:");
    EXPECT_EQ(results.at(8), "Losers:");
    EXPECT_EQ(results.at(11), "Percentage of votes:");
    EXPECT_EQ(subWinners.size(), 3);
    EXPECT_EQ(subLosers.size(), 2);
    EXPECT_EQ(subPercentages.size(), 5);
    EXPECT_EQ(subWinners.at(0), "Andrew Hero");
    EXPECT_EQ(subWinners.at(1), "Chuck Lancaster");
    EXPECT_EQ(subWinners.at(2), "Joe Cool");
    EXPECT_EQ(subLosers.at(0), "Mark Suckerberg");
    EXPECT_EQ(subLosers.at(1), "Micheal Ashton");
    EXPECT_EQ(subPercentages.at(0), "Andrew Hero (23.00%)");
    EXPECT_EQ(subPercentages.at(1), "Chuck Lancaster (20.00%)");
    EXPECT_EQ(subPercentages.at(2), "Joe Cool (26.00%)");
    EXPECT_EQ(subPercentages.at(3), "Mark Suckerberg (19.00%)");
    EXPECT_EQ(subPercentages.at(4), "Micheal Ashton (12.00%)");

    // int seatNum;
    // userInput(file_names.at(5));
    // ifstream file;
    // open_file(file);
    // EXPECT_TRUE(file.is_open());
    // restore_stdin_fd(old_stdin);
    // Ballots ballots = read_file(file, false);
    // file.close();

    // userInput(seatNums.at(3));
    // prompt_user_seatNum(seatNum);
    // EXPECT_EQ(seatNum, 3);
    // restore_stdin_fd(old_stdin);

    // pluralityElection = new Plurality(&ballots, seatNum);
    // pluralityElection->runElection();

    // vector<Candidate> winners = pluralityElection->getWinners();
    // vector<Candidate> losers = pluralityElection->getLosers();
    // cout << pluralityElection->getCandidates().size() << endl;
    // int loser_size = pluralityElection->getCandidates().size() - seatNum;
    // EXPECT_EQ(winners.size(), seatNum);
    // EXPECT_EQ(losers.size(), loser_size);
    // for (long unsigned int i = 0; i < winners.size(); i++) {
    //     for (long unsigned int j = 0; j < losers.size(); j++) {
    //         // Test all winners with all losers - should have at least the same, if not greater ballots
    //         EXPECT_GE(winners.at(i).getBallotNum(), losers.at(j).getBallotNum()); 
    //     }
    // }
};

TEST_F(SysTest, pluralityTie) { // Test Case ID#: 32
    vector<string> user_input;
    user_input.push_back(file_names.at(10));
    user_input.push_back(seatNums.at(3));
    user_input.push_back(algo.at(0));

    userInput(user_input);
    testing::internal::CaptureStdout();

    Driver driver = Driver();
    driver.run(argc, argv);
    restore_stdin_fd(old_stdin);
    string ret = testing::internal::GetCapturedStdout();
    vector<string> results;
    looper(ret, results);
    results.erase(results.begin(), results.begin() + 7);

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

    // int seatNum;
    // userInput(file_names.at(10));
    // ifstream file;
    // open_file(file);
    // EXPECT_TRUE(file.is_open());
    // restore_stdin_fd(old_stdin);
    // Ballots ballots = read_file(file, false);
    // file.close();

    // userInput(seatNums.at(3));
    // prompt_user_seatNum(seatNum);
    // EXPECT_EQ(seatNum, 3);
    // restore_stdin_fd(old_stdin);

    // pluralityElection = new Plurality(&ballots, seatNum);
    // pluralityElection->runElection();

    // vector<Candidate> winners = pluralityElection->getWinners();
    // vector<Candidate> losers = pluralityElection->getLosers();
    // int loser_size = pluralityElection->getCandidates().size() - seatNum;
    // EXPECT_EQ(winners.size(), seatNum);
    // EXPECT_EQ(losers.size(), loser_size);
    // for (long unsigned int i = 0; i < winners.size(); i++) {
    //     for (long unsigned int j = 0; j < losers.size(); j++) {
    //         // Test all winners with all losers - should have the same ballots exactly.
    //         EXPECT_EQ(winners.at(i).getBallotNum(), losers.at(j).getBallotNum()); 
    //     }
    // }
};

// TEST_F(SysTest, stvRegular) {
//     int seatNum;
//     userInput(file_names.at(4));
//     ifstream file;
//     open_file(file);
//     EXPECT_TRUE(file.is_open());
//     restore_stdin_fd(old_stdin);
//     Ballots ballots = read_file(file, false);
//     file.close();

//     userInput(seatNums.at(3));
//     prompt_user_seatNum(seatNum);
//     EXPECT_EQ(seatNum, 3);
//     restore_stdin_fd(old_stdin);

//     STVelection = new STV(&ballots, seatNum);
//     userInput(auditFile.at(4));
//     STVelection->runElection(); 
//     restore_stdin_fd(old_stdin);

//     vector<Candidate> winners = STVelection->getWinners();
//     vector<Candidate> losers = STVelection->getLosers();
//     int loser_size = STVelection->getCandidates().size() - seatNum;
//     EXPECT_EQ(winners.size(), seatNum);
//     EXPECT_EQ(losers.size(), loser_size);
//     for (long unsigned int i = 0; i < winners.size(); i++) {
//         // All winners should have met the Droop Quota.
//         EXPECT_EQ(winners.at(i).getBallotNum(), STVelection->getDroopQuota());
//         for (long unsigned int j = 0; j < losers.size(); j++) {
//             // Test all winners with all losers - should have at least the same, if not greater ballots
//             EXPECT_GE(winners.at(i).getBallotNum(), losers.at(j).getBallotNum()); 
//         }
//     }
//     bool access(auditFile.at(3).at(0).c_str());
//     EXPECT_TRUE(access);
// };

// TEST_F(SysTest, timeSTV) {
//     auto t0 = chrono::high_resolution_clock::now();
//     int seatNum; 
//     userInput(file_names.at(8));
//     ifstream file;
//     open_file(file);
//     EXPECT_TRUE(file.is_open());
//     restore_stdin_fd(old_stdin);
//     Ballots ballots = read_file(file, false);
//     file.close();

//     userInput(seatNums.at(4));
//     prompt_user_seatNum(seatNum);
//     EXPECT_EQ(seatNum, 10);
//     restore_stdin_fd(old_stdin);

//     STVelection = new STV(&ballots, seatNum);
//     userInput(auditFile.at(5));
//     STVelection->runElection(); 
//     restore_stdin_fd(old_stdin);

//     vector<Candidate> winners = STVelection->getWinners();
//     vector<Candidate> losers = STVelection->getLosers();
//     int loser_size = STVelection->getCandidates().size() - seatNum;
//     EXPECT_EQ(winners.size(), seatNum);
//     EXPECT_EQ(losers.size(), loser_size);
//     for (long unsigned int i = 0; i < winners.size(); i++) {
//         // All winners should have met the Droop Quota.
//         EXPECT_EQ(winners.at(i).getBallotNum(), STVelection->getDroopQuota());
//         for (long unsigned int j = 0; j < losers.size(); j++) {
//             // Test all winners with all losers - should have at least the same, if not greater ballots
//             EXPECT_GE(winners.at(i).getBallotNum(), losers.at(j).getBallotNum()); 
//         }
//     }
//     bool access(auditFile.at(4).at(0).c_str());
//     EXPECT_TRUE(access);
//     auto t1 = chrono::high_resolution_clock::now();
//     chrono::duration<double> diff = t1 - t0;
//     cout << fixed << setprecision(2) << diff.count() << " seconds to run." << endl;
//     EXPECT_LE(diff.count(), 300.0);
// };

TEST_F(SysTest, timePlurality) { // Test Case ID#: 36
    auto t0 = chrono::high_resolution_clock::now();

    vector<string> user_input;
    user_input.push_back(file_names.at(9));
    user_input.push_back(seatNums.at(4));
    user_input.push_back(algo.at(0));

    userInput(user_input);
    testing::internal::CaptureStdout();

    Driver driver = Driver();
    driver.run(argc, argv);
    restore_stdin_fd(old_stdin);
    string ret = testing::internal::GetCapturedStdout();
    vector<string> results;
    looper(ret, results);
    results.erase(results.begin(), results.begin() + 7);

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

    // int seatNum; 
    // userInput(file_names.at(9));
    // ifstream file;
    // open_file(file);
    // EXPECT_TRUE(file.is_open());
    // restore_stdin_fd(old_stdin);
    // Ballots ballots = read_file(file, false);
    // file.close();

    // userInput(seatNums.at(4));
    // prompt_user_seatNum(seatNum);
    // EXPECT_EQ(seatNum, 10);
    // restore_stdin_fd(old_stdin);

    // pluralityElection = new Plurality(&ballots, seatNum);
    // pluralityElection->runElection();

    // vector<Candidate> winners = pluralityElection->getWinners();
    // vector<Candidate> losers = pluralityElection->getLosers();
    // int loser_size = pluralityElection->getCandidates().size() - seatNum;
    // EXPECT_EQ(winners.size(), seatNum);
    // EXPECT_EQ(losers.size(), loser_size);
    // for (long unsigned int i = 0; i < winners.size(); i++) {
    //     for (long unsigned int j = 0; j < losers.size(); j++) {
    //         // Test all winners with all losers - should have at least the same, if not greater ballots
    //         EXPECT_GE(winners.at(i).getBallotNum(), losers.at(j).getBallotNum()); 
    //     }
    // }
};

// TEST_F(SysTest, stvTie) {
//     int seatNum;
//     userInput(file_names.at(11));
//     ifstream file;
//     open_file(file);
//     EXPECT_TRUE(file.is_open());
//     restore_stdin_fd(old_stdin);
//     Ballots ballots = read_file(file, false);
//     file.close();

//     userInput(seatNums.at(3));
//     prompt_user_seatNum(seatNum);
//     EXPECT_EQ(seatNum, 3);
//     restore_stdin_fd(old_stdin);

//     STVelection = new STV(&ballots, seatNum);
//     userInput(auditFile.at(6));
//     STVelection->runElection(); 
//     restore_stdin_fd(old_stdin);

//     vector<Candidate> winners = STVelection->getWinners();
//     vector<Candidate> losers = STVelection->getLosers();
//     int loser_size = STVelection->getCandidates().size() - seatNum;
//     EXPECT_EQ(winners.size(), seatNum);
//     EXPECT_EQ(losers.size(), loser_size);
//     for (long unsigned int i = 0; i < winners.size(); i++) {
//         // All winners should have met the Droop Quota.
//         EXPECT_EQ(winners.at(i).getBallotNum(), STVelection->getDroopQuota());
//         for (long unsigned int j = 0; j < losers.size(); j++) {
//             // Test all winners with all losers - should be about the same - winners in losers bracket.
//             EXPECT_GE(winners.at(i).getBallotNum(), losers.at(j).getBallotNum()); 
//         }
//     }
// };

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
