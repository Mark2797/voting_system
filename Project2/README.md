### Compiling the System
Within the src folder of the program, run 'make' to compile the program and tests.
(RECOMMENDED: USE make -j FOR THREADING TO COMPILE FASTER.)

Running "make run" within your command line interface will run the program after compilation.

Running "make test" within your command line interface will run the program's unit and system tests.

### Running The System:
./voting_system - running the voting_system software with shuffling
./voting_system shuffle-off - running the voting_system software without shuffling

Afterwards, you will be presented with an input asking for your .csv file name.
Make sure to type the entire file name, along with the path set to the file.

Example: If your file was stored in a seperate file called "testing", type:
../testing/<FILE_NAME>.csv

This program is ONLY intended to run with .csv files.

Next, the program will ask if you want to input multiple files.
If you wish to input another .csv file, type 1, else type 2.
If you type 1, the program will loop back to the prompt beforehand to ask for another file.
Otherwise, the election will.

### Audit File Output:
For the STV algorithm ONLY, the program will prompt you to ask for an audit file.
This audit file serves to showcase the steps taken within the STV algorithm to disperse the votes, along with each round
that the algorithm runs for to show the winners and losers list being updated.

The audit file will be inputed as plaintext, and will not need any extensions added, however the 
output file will be a .txt file with your input in the name within the same folder as the program.

### Interpreting Results:
For our election, we run 3 main types:
STV - Single Transferable Voting
PV - Plurality Voting
MV - Municipal Voting

The specific of each election will have different results, however there are similarities with these results that you get.
You will get:
    - The type of election ran (STV, PV, MV).
    - The total number of seats.
    - The total number of ballots.
    - The winners of the election.
    - The losers of the election.

For each individual election, different results will come up after these common results are shown.

    - STV will not have any information other than the winners and losers.
    - PV will have the percentage of votes recieved by the associated candidate.
    - MV will have the number of votes received be the associated candidate.

### Exiting the Program:
In the event that the program does NOT run the election (i.e. nothing happens on multiple file input
due to invalid files), input ctrl+C to exit the program manually.

Otherwise, the election will automatically exit the program as soon as the results are displayed 
(and for STV, once the audit file is outputted).