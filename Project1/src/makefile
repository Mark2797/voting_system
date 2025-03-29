# makefile
# Automation for builing, compiling, and testing the project
# Author: Mark Tsai

# Compiler
CXX = g++

# Compiler flags
# -g: gdb
# -pthread: enable multithreading for Google test
# -MMD: generate dependency files, only user header files not system header files
# -MP: make dependency file has a dummy target to prevent make error if a header is not there
# CFLAGS for the C compiler, CXXFLAGS for C++, and CPPFLAGS for both
CXXFLAGS = -Wall -g -pthread -MMD -MP

# Google Test Library Path
GTEST_DIR = -lgtest -lgtest_main -pthread

# Source Files
SRCS = Ballots.cpp Candidate.cpp Election.cpp main.cpp Plurality.cpp STV.cpp
OBJS = Ballots.o Candidate.o Election.o main.o Plurality.o STV.o
OBJS_D = Ballots.d Candidate.d Election.d main.d Plurality.d STV.d

# Test Files
TEST_SRCS = testBallots.cpp testCandidate.cpp testMain.cpp testPlurality.cpp testSTV.cpp testSystem.cpp
TEST_OBJS = testBallots.o testCandidate.o testMain.o main_test.o testPlurality.o testSTV.o testSystem.o
TEST_OBJS_D = testBallots.d testCandidate.d testMain.d main_test.d testPlurality.d testSTV.d testSystem.d
TEST_EXECUTABLES = testBallots testCandidate testMain testPlurality testSTV testSystem
# Target executable
TARGET = voting_system

# Default target
all: $(OBJS) $(TEST_OBJS) $(TARGET) $(TEST_EXECUTABLES)

Ballots.o:
	$(CXX) $(CXXFLAGS) -c Ballots.cpp -o Ballots.o

Candidate.o:
	$(CXX) $(CXXFLAGS) -c Candidate.cpp -o Candidate.o

Election.o:
	$(CXX) $(CXXFLAGS) -c Election.cpp -o Election.o

main.o:
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

Plurality.o:
	$(CXX) $(CXXFLAGS) -c Plurality.cpp -o Plurality.o

STV.o:
	$(CXX) $(CXXFLAGS) -c STV.cpp -o STV.o

testBallots.o:
	$(CXX) $(CXXFLAGS) -c testBallots.cpp -o testBallots.o 

testCandidate.o:
	$(CXX) $(CXXFLAGS) -c testCandidate.cpp -o testCandidate.o

testMain.o:
	$(CXX) $(CXXFLAGS) -c testMain.cpp -o testMain.o

testPlurality.o:
	$(CXX) $(CXXFLAGS) -c testPlurality.cpp -o testPlurality.o

testSTV.o:
	$(CXX) $(CXXFLAGS) -c testSTV.cpp -o testSTV.o

testBallots: testBallots.o Ballots.o
	$(CXX) $(CXXFLAGS) testBallots.o Ballots.o -o testBallots $(GTEST_DIR)

testCandidate: testCandidate.o Candidate.o
	$(CXX) $(CXXFLAGS) testCandidate.o Candidate.o -o testCandidate $(GTEST_DIR)

testPlurality: testPlurality.o Plurality.o Ballots.o Candidate.o Election.o
	$(CXX) $(CXXFLAGS) testPlurality.o Plurality.o Ballots.o Candidate.o Election.o -o testPlurality $(GTEST_DIR)

testSTV: testSTV.o STV.o Ballots.o Candidate.o Election.o
	$(CXX) $(CXXFLAGS) testSTV.o STV.o Ballots.o Candidate.o Election.o -o testSTV $(GTEST_DIR)

testSystem: testSystem.o STV.o Plurality.o Ballots.o Candidate.o Election.o main_test.o
	$(CXX) $(CXXFLAGS) testSystem.o STV.o Plurality.o Ballots.o Candidate.o Election.o main_test.o -o testSystem $(GTEST_DIR)

# enable TESTING flag in main
testMain: testMain.o Ballots.o main_test.o
	$(CXX) $(CXXFLAGS) testMain.o Ballots.o main_test.o -o testMain $(GTEST_DIR) -DTESTING

# main for testing
main_test.o:: main.cpp
	$(CXX) $(CXXFLAGS) -DTESTING -c main.cpp -o main_test.o

# Link object files to the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Run all tests
# $$: shell variable (single $ is for variables)
test: $(TEST_EXECUTABLES)
	for test in $(TEST_EXECUTABLES); do \
		./$$test; \
	done

# Clean compiled files
clean:
	rm -f $(TARGET) $(OBJS) $(TEST_OBJS) $(TEST_EXECUTABLES) $(OBJS_D) $(TEST_OBJS_D)

# Rule to run the executable
run: $(TARGET)
	./$(TARGET)
