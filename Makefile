.PHONY: build
.DEFAULT_GOAL := run

SRC=src/*.cpp
INCLUDE=include
PROGRAM=build.exe
BUILD=build

build:
	@g++ -o $(BUILD)/$(PROGRAM) $(SRC) -I$(INCLUDE) -std=c++26

run: build
	@./$(BUILD)/$(PROGRAM)
	@echo
	@xxd program.bin