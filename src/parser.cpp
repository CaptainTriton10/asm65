#include "parser.hpp"

Instruction parse(std::vector<Token> line) {
    Instruction instr;

    if (line[0].type != TokenType::INSTR) {
        throw std::exception();
    }

    if (line.size() == 1) {
        instr.mode == AddressMode::Implied;
        return instr;
    }

    instr.opCode = std::get<std::string>(line[0].value);

    switch (line[1].type) {
        case TokenType::OP_HASH:
            instr.mode = AddressMode::Immediate;
            instr.imm16 = std::get<uint16_t>(line[2].value);
            break;

        case TokenType::OP_DOLLAR:
            instr.mode = AddressMode::Zeropage;
            instr.imm16 = std::get<uint16_t>(line[2].value);
            break;

        case TokenType::LABEL:
            if (std::get<std::string>(line[1].value) == "A") {
                instr.mode = AddressMode::Accumulator;
                break;
            }

        default:
            throw "Not implememnted.";
    }

    return instr;
}

static Instruction opDollar(std::vector<Token> &line) {
    
}