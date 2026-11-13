#pragma once
#include "parser.hpp"
#include <string>

struct OpCode {
    AddressMode addressMode;
    std::string instr;
};

void compile(std::string filename, std::vector<Instruction> instructions);

OpCode *initOpCodes();