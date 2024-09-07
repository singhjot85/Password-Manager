output: PasswordDriver.o PasswordManager.o
	g++ PasswordDriver.o PasswordManager.o -o output

PasswordDriver.o: PasswordDriver.cpp
	g++ -c PasswordDriver.cpp

PasswordManager.o: PasswordManager.o EncryptionAlgorithm.o PasswordManager.h
	g++ -c PasswordManager.cpp EncryptionAlgorithm.cpp 

clean: 
	rm *.o output

This program uses a PasswordManager header file to declare all the required methods, the PasswordManager and EncryptionAlgorithm define methods declared in header file. 
For in-depth detail of the program read PasswordManager.pdf attached.

How to Use:
	To compile the program:
		mingw32-make
	To run the program:
		mingw32-make run
	To clean up all the files:
		mingw32-make clean

Makefile syntax:
	target: dependencies
		action

Commands to manually run the program:
	In PowerShell:
		g++ PasswordDriver.cpp PasswordManager.cpp EncryptionAlgorithm.cpp -o output; ./output.exe
	In CMD:
		g++ PasswordDriver.cpp PasswordManager.cpp EncryptionAlgorithm.cpp -o output && output.exe
	Breakdown,
	Compile:
		g++ PasswordDriver.cpp PasswordManager.cpp EncryptionAlgorithm.cpp -o output
	Run: 
		In PowerShell:
			./output.exe
		In CMD:
			output.exe