.PHONY: build
.DEFAULT_GOAL := run

SRC=src/*.cpp
INCLUDE=include
PROGRAM=asm65.exe
BUILD=build

build:
	@g++ -o $(BUILD)/$(PROGRAM) $(SRC) -I$(INCLUDE) -std=c++26

run: build
	@./$(BUILD)/$(PROGRAM) test.asm -o test.bin
	@echo
	@xxd test.bin