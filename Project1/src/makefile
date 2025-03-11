# makefile
# Automation for builing, compiling, and testing the project
# Author: Mark Tsai

# Compiler
CXX = g++

# Compiler flags
# -g: gdb
# -pthread: enable multithreading for Google test
# -I: Include path
# -I.: Add the correct directory to the include search path
# CFLAGS for the C compiler, CXXFLAGS for C++, and CPPFLAGS for both
CXXFLAGS = -Wall -g -pthread -I. -MMD -MP

# Directories
SRC_DIR = .
TEST_DIR = ../testing

# Google Test Library Path
GTEST_DIR = -lgtest -lgtest_main -pthread

# Source Files
SRCS = Ballots.cpp Candidate.cpp Election.cpp main.cpp Plurality.cpp STV.cpp
OBJS = Ballots.o Candidate.o Election.o main.o Plurality.o STV.o
OBJS_D = Ballots.d Candidate.d Election.d main.d Plurality.d STV.d

# Test Files
TEST_SRCS = $(TEST_DIR)/testBallots.cpp $(TEST_DIR)/testCandidate.cpp $(TEST_DIR)/testMain.cpp
TEST_OBJS = $(TEST_DIR)/testBallots.o $(TEST_DIR)/testCandidate.o $(TEST_DIR)/testMain.o
TEST_OBJS_D = $(TEST_DIR)/testBallots.d $(TEST_DIR)/testCandidate.d $(TEST_DIR)/testMain.d
TEST_EXECUTABLES = $(TEST_DIR)/testBallots $(TEST_DIR)/testCandidate $(TEST_DIR)/testMain

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

$(TEST_DIR)/testBallots.o:
	$(CXX) $(CXXFLAGS) -c $(TEST_DIR)/testBallots.cpp -o $(TEST_DIR)/testBallots.o 

$(TEST_DIR)/testCandidate.o:
	$(CXX) $(CXXFLAGS) -c $(TEST_DIR)/testCandidate.cpp -o $(TEST_DIR)/testCandidate.o

$(TEST_DIR)/testMain.o:
	$(CXX) $(CXXFLAGS) -c $(TEST_DIR)/testMain.cpp -o $(TEST_DIR)/testMain.o

$(TEST_DIR)/testBallots: $(TEST_DIR)/testBallots.o Ballots.o
	$(CXX) $(CXXFLAGS) $(TEST_DIR)/testBallots.o Ballots.o -o $(TEST_DIR)/testBallots $(GTEST_DIR)

$(TEST_DIR)/testCandidate: $(TEST_DIR)/testCandidate.o Candidate.o
	$(CXX) $(CXXFLAGS) $(TEST_DIR)/testCandidate.o Candidate.o -o $(TEST_DIR)/testCandidate $(GTEST_DIR)

# enable TESTING flag in main
$(TEST_DIR)/testMain: $(TEST_DIR)/testMain.o Ballots.o main_test.o
	$(CXX) $(CXXFLAGS) $(TEST_DIR)/testMain.o Ballots.o main_test.o -o $(TEST_DIR)/testMain $(GTEST_DIR) -DTESTING

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
