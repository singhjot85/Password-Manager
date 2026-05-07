COMPILER_CANDIDATES := g++ g++-15 g++-14 g++-13 g++-12 g++-11 clang++ c++
DETECTED_CXX := $(firstword $(foreach compiler,$(COMPILER_CANDIDATES),$(shell command -v $(compiler) 2>/dev/null)))

ifeq ($(origin CXX),command line)
SELECTED_CXX := $(CXX)
else ifneq ($(shell command -v $(CXX) 2>/dev/null),)
SELECTED_CXX := $(CXX)
else
SELECTED_CXX := $(DETECTED_CXX)
endif

CXX := $(SELECTED_CXX)
CXXFLAGS ?= -Wall -Wextra -std=c++15

APP_NAME := password-manager
BUILD_DIR := bin
TARGET := $(BUILD_DIR)/$(APP_NAME)
SRCS := PasswordDriver.cpp PasswordManager.cpp EncryptionAlgorithm.cpp
HEADERS := PasswordManager.h
IMAGE_NAME ?= password-manager:local

.PHONY: all build run clean docker-build compose-build compose-run compose-up compose-down compose-logs

all: build

build: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
ifeq ($(CXX),)
	$(error No C++ compiler found. Install g++, clang++, or run make build CXX=/path/to/compiler)
endif
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

run: build
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

docker-build:
	docker build -t $(IMAGE_NAME) .

compose-build:
	docker compose build

compose-run:
	docker compose run --rm app

compose-up:
	docker compose up -d db

compose-down:
	docker compose down

compose-logs:
	docker compose logs -f
