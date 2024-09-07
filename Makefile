CXX = g++
CXXFLAGS = -Wall -std=c++11

SRCS = PasswordDriver.cpp PasswordManager.cpp EncryptionAlgorithm.cpp
HEADERS = PasswordManager.h

TARGET = PasswordManager.exe

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	del /f $(TARGET)
	del /f *.o

run: $(TARGET)
	$(TARGET)
