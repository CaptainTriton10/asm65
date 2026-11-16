#pragma once
#include "magic_enum.hpp"
#include <iostream>
#include <string>
#include <variant>
#include <vector>

const std::string emptyChars = "\n\r\t ";
const std::string textChars =
    "_-abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::string numChars = "0123456789abcdefABCDEF";

enum class TokenType {
    INSTR,
    VALUE,
    LABEL,
    OP_HASH,
    OP_DOLLAR,
    OP_COMMA,
    OP_LPAREN,
    OP_RPAREN
};

constexpr std::array<std::string, 57> instructions = {
    "ADC", "AND", "ASL", "BCC", "BCS", "BEQ", "BIT", "BMI", "BNE", "BPL",
    "BRK", "BVC", "BVS", "CLC", "CLD", "CLI", "CLV", "CMP", "CPX", "CPY",
    "DEC", "DEX", "DEY", "EOR", "INC", "INX", "INY", "JMP", "JSR", "LDA",
    "LDX", "LDY", "LSR", "NOP", "ORA", "PHA", "PHP", "PLA", "PLP", "ROL",
    "ROR", "RTI", "RTS", "SBC", "SEC", "SED", "SEI", "STA", "STX", "STY",
    "TAX", "TAY", "TSX", "TXA", "TXS", "TYA", "TST"};

struct Token {
    TokenType type;
    std::variant<uint16_t, std::string> value;
    bool is16Bit = false;
};

inline std::ostream &operator<<(std::ostream &os, const Token &t) {
    if (const uint16_t *value = std::get_if<uint16_t>(&t.value)) {
        os << magic_enum::enum_name(t.type) << "(" << std::dec << (int)*value
           << ")";
    } else {
        os << magic_enum::enum_name(t.type) << "("
           << std::get<std::string>(t.value) << ")";
    }

    return os;
}

class Lexer {
  private:
    const std::string line;
    int nextIndex;
    char currentChar;

    void advance();

    std::string ingestText();
    std::string ingestNumber();

  public:
    Lexer(std::string _line) : line(_line) {
        nextIndex = 0;
        advance();
    }

    std::vector<Token> tokenise();
};