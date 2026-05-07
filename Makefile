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
CPPFLAGS ?=
CPPFLAGS += -Iinclude
CXXFLAGS ?= -Wall -Wextra -std=c++17
LDFLAGS ?=
LDLIBS ?=
USE_POSTGRES ?= 0

APP_NAME := password-manager
BUILD_DIR := bin
TARGET := $(BUILD_DIR)/$(APP_NAME)
IMAGE_NAME ?= password-manager:local

SRCS := \
	src/app/PasswordDriver.cpp \
	src/core/PasswordManager.cpp \
	src/crypto/HashingLayer.cpp \
	src/database/FilePasswordRepository.cpp \
	src/database/PostgresPasswordRepository.cpp

HEADERS := \
	include/core/PasswordManager.h \
	include/crypto/HashingLayer.h \
	include/database/PasswordRepository.h \
	include/database/FilePasswordRepository.h \
	include/database/PostgresPasswordRepository.h

ifeq ($(USE_POSTGRES),1)
CPPFLAGS += -DUSE_POSTGRES $(shell pg_config --includedir 2>/dev/null | sed 's/^/-I/')
LDFLAGS += $(shell pg_config --libdir 2>/dev/null | sed 's/^/-L/')
LDLIBS += -lpq
endif

.PHONY: all build run clean docker-build compose-build compose-run compose-up compose-down compose-logs

all: build

build: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
ifeq ($(CXX),)
	$(error No C++ compiler found. Install g++, clang++, or run make build CXX=/path/to/compiler)
endif
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS) $(LDLIBS)

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
