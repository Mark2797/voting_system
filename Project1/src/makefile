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
CXXFLAGS = -Wall -g -pthread -I.

# Directories
SRC_DIR = .
TEST_DIR = ../testing

# Google Test Library Path
GTEST_DIR = -lgtest -lgtest_main -pthread

# Source Files
SRCS = Ballots.cpp Candidate.cpp Election.cpp main.cpp Plurality.cpp STV.cpp
OBJS = Ballots.o Candidate.o Election.o main.o Plurality.o STV.o

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

# Test Files
TEST_SRCS = $(TEST_DIR)/testBallots.cpp $(TEST_DIR)/testCandidate.cpp
TEST_OBJS = $(TEST_DIR)/testBallots.o $(TEST_DIR)/testCandidate.o
TEST_EXECUTABLES = $(TEST_DIR)/testBallots $(TEST_DIR)/testCandidate

$(TEST_DIR)/testBallots.o:
	$(CXX) $(CXXFLAGS) -c $(TEST_DIR)/testBallots.cpp -o $(TEST_DIR)/testBallots.o 

$(TEST_DIR)/testCandidate.o:
	$(CXX) $(CXXFLAGS) -c $(TEST_DIR)/testCandidate.cpp -o $(TEST_DIR)/testCandidate.o

$(TEST_DIR)/testBallots: $(TEST_DIR)/testBallots.o Ballots.o
	$(CXX) $(CXXFLAGS) $(TEST_DIR)/testBallots.o Ballots.o -o $(TEST_DIR)/testBallots $(GTEST_DIR)

$(TEST_DIR)/testCandidate: $(TEST_DIR)/testCandidate.o Candidate.o
	$(CXX) $(CXXFLAGS) $(TEST_DIR)/testCandidate.o Candidate.o -o $(TEST_DIR)/testCandidate $(GTEST_DIR)

# Target executable
TARGET = voting_system

# Default target
all: $(TARGET)

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
	rm -f $(TARGET) $(OBJS) $(TEST_OBJS) $(TEST_EXECUTABLES)

# Rule to run the executable
run: $(TARGET)
	./$(TARGET)
