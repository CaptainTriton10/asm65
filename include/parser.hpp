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

struct Pattern {
    std::vector<TokenType> expectedTokens;
    bool isImm16;
};

const Pattern ptn_accumulator{
    {
        TokenType::INSTR,
        TokenType::LABEL,
    },
    false,
};

const Pattern ptn_absolute{
    {
        TokenType::INSTR,
        TokenType::OP_DOLLAR,
        TokenType::VALUE,
    },
    true,
};

const Pattern ptn_absoluteX{
    {
        TokenType::INSTR,
        TokenType::OP_DOLLAR,
        TokenType::VALUE,
        TokenType::OP_COMMA,
        TokenType::LABEL,
    },
    true,
};

const Pattern ptn_absoluteY{
    {
        TokenType::INSTR,
        TokenType::OP_DOLLAR,
        TokenType::VALUE,
        TokenType::OP_COMMA,
        TokenType::LABEL,
    },
    true,
};

const Pattern ptn_immediate{
    {
        TokenType::INSTR,
        TokenType::OP_HASH,
        TokenType::OP_DOLLAR,
        TokenType::VALUE,
    },
    false,
};

const Pattern ptn_implied{
    {
        TokenType::INSTR,
    },
    false,
};

const Pattern ptn_indirect{
    {
        TokenType::INSTR,
        TokenType::OP_LPAREN,
        TokenType::OP_DOLLAR,
        TokenType::VALUE,
        TokenType::OP_RPAREN,
    },
    true,
};

const Pattern ptn_indirectX{
    {
        TokenType::INSTR,
        TokenType::OP_LPAREN,
        TokenType::OP_DOLLAR,
        TokenType::VALUE,
        TokenType::OP_COMMA,
        TokenType::LABEL,
        TokenType::OP_RPAREN,
    },
    false,
};

const Pattern ptn_indirectY{
    {
        TokenType::INSTR,
        TokenType::OP_LPAREN,
        TokenType::OP_DOLLAR,
        TokenType::VALUE,
        TokenType::OP_RPAREN,
        TokenType::OP_COMMA,
        TokenType::LABEL,
    },
    false,
};

const Pattern ptn_relative{
    {
        TokenType::INSTR,
        TokenType::OP_LPAREN,
        TokenType::OP_DOLLAR,
        TokenType::VALUE,
        TokenType::OP_RPAREN,
    },
    false,
};

const Pattern ptn_zeropage{
    {
        TokenType::INSTR,
        TokenType::OP_DOLLAR,
        TokenType::VALUE,
    },
    false,
};

const Pattern ptn_zeropageX{
    {
        TokenType::INSTR,
        TokenType::OP_DOLLAR,
        TokenType::VALUE,
        TokenType::OP_COMMA,
        TokenType::LABEL,
    },
    false,
};

const Pattern ptn_zeropageY{
    {
        TokenType::INSTR,
        TokenType::OP_DOLLAR,
        TokenType::VALUE,
        TokenType::OP_COMMA,
        TokenType::LABEL,
    },
    false,
};

inline std::ostream &operator<<(std::ostream &os, const Instruction &i) {
    os << magic_enum::enum_name(i.mode) << "(" << i.opCode << ", " << i.imm16
       << ")";

    return os;
}

Instruction parse(std::vector<Token> &line);