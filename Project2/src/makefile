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
SRCS = Ballots.cpp Candidate.cpp Election.cpp main.cpp Plurality.cpp STV.cpp Municipal.cpp Driver.cpp FileHandler.cpp
OBJS = Ballots.o Candidate.o Election.o main.o Plurality.o STV.o Municipal.o Driver.o FileHandler.o
OBJS_D = Ballots.d Candidate.d Election.d main.d Plurality.d STV.d Municipal.d Driver.d FileHandler.d

# Test Files
TEST_SRCS = testBallots.cpp testCandidate.cpp testPlurality.cpp testSTV.cpp testMunicipal.cpp testFileHandler.cpp testDriver.cpp testSystem.cpp
TEST_OBJS = testBallots.o testCandidate.o testPlurality.o testSTV.o testMunicipal.o testFileHandler.o testDriver.o testSystem.o
TEST_OBJS_D = testBallots.d testCandidate.d testPlurality.d testSTV.d testMunicipal.d testFileHandler.d testDriver.d testSystem.d
TEST_EXECUTABLES = testBallots testCandidate testPlurality testSTV testMunicipal testFileHandler testDriver testSystem
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

Municipal.o:
	$(CXX) $(CXXFLAGS) -c Municipal.cpp -o Municipal.o

Driver.o:
	$(CXX) $(CXXFLAGS) -c Driver.cpp -o Driver.o

FileHandler.o:
	$(CXX) $(CXXFLAGS) -c FileHandler.cpp -o FileHandler.o

testBallots: testBallots.o Ballots.o
	$(CXX) $(CXXFLAGS) testBallots.o Ballots.o -o testBallots $(GTEST_DIR)

testCandidate: testCandidate.o Candidate.o
	$(CXX) $(CXXFLAGS) testCandidate.o Candidate.o -o testCandidate $(GTEST_DIR)

testPlurality: testPlurality.o Plurality.o Ballots.o Candidate.o Election.o
	$(CXX) $(CXXFLAGS) testPlurality.o Plurality.o Ballots.o Candidate.o Election.o -o testPlurality $(GTEST_DIR)

testSTV: testSTV.o STV.o Ballots.o Candidate.o Election.o
	$(CXX) $(CXXFLAGS) testSTV.o STV.o Ballots.o Candidate.o Election.o -o testSTV $(GTEST_DIR)

testMunicipal: testMunicipal.o Municipal.o Ballots.o Candidate.o Election.o
	$(CXX) $(CXXFLAGS) testMunicipal.o Municipal.o Ballots.o Candidate.o Election.o -o testMunicipal $(GTEST_DIR)

testFileHandler: testFileHandler.o FileHandler.o
	$(CXX) $(CXXFLAGS) testFileHandler.o FileHandler.o -o testFileHandler $(GTEST_DIR)

testDriver: testDriver.o Ballots.o Candidate.o Election.o Plurality.o STV.o Municipal.o Driver.o FileHandler.o
	$(CXX) $(CXXFLAGS) testDriver.o Ballots.o Candidate.o Election.o Plurality.o STV.o Municipal.o Driver.o FileHandler.o -o testDriver $(GTEST_DIR)

testSystem: testSystem.o Ballots.o Candidate.o Election.o Plurality.o STV.o Municipal.o Driver.o FileHandler.o 
	$(CXX) $(CXXFLAGS) testSystem.o Ballots.o Candidate.o Election.o Plurality.o Municipal.o STV.o Driver.o FileHandler.o -o testSystem $(GTEST_DIR)

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
	rm -f $(TARGET) $(OBJS) $(TEST_OBJS) $(TEST_EXECUTABLES) $(OBJS_D) $(TEST_OBJS_D) *.txt ../testing/*.txt

# Rule to run the executable
run: $(TARGET)
	./$(TARGET)
