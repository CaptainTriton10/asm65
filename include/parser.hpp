#pragma once
#include "lexer.hpp"
#include "magic_enum.hpp"
#include <cstdint>
#include <string>
#include <vector>

enum class AddressMode {
    Accumulator,
    Absolute,
    AbsoluteX,
    AbsoluteY,
    Immediate,
    Implied,
    Indirect,
    IndirectX,
    IndirectY,
    Relative,
    Zeropage,
    ZeropageX,
    ZeropageY
};

struct Instruction {
    std::string opCode;
    AddressMode mode;
    uint16_t imm16;
};

inline std::ostream &operator<<(std::ostream &os, const Instruction &i) {
    os << magic_enum::enum_name(i.mode) << "(" << i.opCode << ", " << i.imm16
       << ")";

    return os;
}

Instruction parse(std::vector<Token> line) noexcept;