#include "parser.hpp"

static bool matchPattern(std::vector<Token> &line, Pattern pattern) {
    if (line.size() != pattern.expectedTokens.size())
        return false;

    for (int i = 0; i < line.size(); i++) {
        if (line[i].type == TokenType::VALUE &&
            pattern.isImm16 != line[i].is16Bit) {
            return false;
        }

        if (line[i].type != pattern.expectedTokens[i])
            return false;
    }

    return true;
}

Instruction parse(std::vector<Token> &line) {
    Instruction instr;

    if (line.size() == 0)
        throw "Empty line tokens";

    instr.opCode = std::get<std::string>(line[0].value);

    if (matchPattern(line, ptn_accumulator)) {
        if (std::get<std::string>(line[1].value) == "A") {
            instr.mode = AddressMode::Accumulator;
            instr.imm16 = (uint16_t)NULL;

            return instr;
        }
    }
    if (matchPattern(line, ptn_absolute)) {
        instr.mode = AddressMode::Absolute;
        instr.imm16 = std::get<uint16_t>(line[2].value);

        return instr;
    }
    if (matchPattern(line, ptn_absoluteX)) {
        if (std::get<std::string>(line[4].value) == "X") {
            instr.mode = AddressMode::AbsoluteX;
            instr.imm16 = std::get<uint16_t>(line[2].value);

            return instr;
        }
    }
    if (matchPattern(line, ptn_absoluteY)) {
        if (std::get<std::string>(line[4].value) == "Y") {
            instr.mode = AddressMode::AbsoluteY;
            instr.imm16 = std::get<uint16_t>(line[2].value);

            return instr;
        }
    }
    if (matchPattern(line, ptn_immediate)) {
        instr.mode = AddressMode::Immediate;
        instr.imm16 = std::get<uint16_t>(line[3].value);

        return instr;
    }
    if (matchPattern(line, ptn_implied)) {
        instr.mode = AddressMode::Implied;
        instr.imm16 = (uint16_t)NULL;

        return instr;
    }
    if (matchPattern(line, ptn_indirect)) {
        instr.mode = AddressMode::Indirect;
        instr.imm16 = std::get<uint16_t>(line[3].value);

        return instr;
    }
    if (matchPattern(line, ptn_indirectX)) {
        instr.mode = AddressMode::IndirectX;
        instr.imm16 = std::get<uint16_t>(line[3].value);

        return instr;
    }
    if (matchPattern(line, ptn_indirectY)) {
        instr.mode = AddressMode::IndirectY;
        instr.imm16 = std::get<uint16_t>(line[3].value);

        return instr;
    }
    if (matchPattern(line, ptn_relative)) { // TODO: deal with signed values
        instr.mode = AddressMode::Relative;
        instr.imm16 = std::get<uint16_t>(line[3].value);

        return instr;
    }
    if (matchPattern(line, ptn_zeropage)) {
        instr.mode = AddressMode::Zeropage;
        instr.imm16 = std::get<uint16_t>(line[2].value);

        return instr;
    }
    if (matchPattern(line, ptn_zeropageX)) {
        if (std::get<std::string>(line[4].value) == "X") {
            instr.mode = AddressMode::ZeropageX;
            instr.imm16 = std::get<uint16_t>(line[2].value);

            return instr;
        }
    }
    if (matchPattern(line, ptn_zeropageY)) {
        if (std::get<std::string>(line[4].value) == "Y") {
            instr.mode = AddressMode::ZeropageY;
            instr.imm16 = std::get<uint16_t>(line[2].value);

            return instr;
        }
    } else {
        throw "Unknown instruction pattern.";
    }

    return instr;
}

// Instruction parse(std::vector<Token> line) {
//     Instruction instr;

//     if (line[0].type != TokenType::INSTR) {
//         throw std::exception();
//     }

//     if (line.size() == 1) {
//         instr.mode == AddressMode::Implied;
//         return instr;
//     }

//     instr.opCode = std::get<std::string>(line[0].value);

//     switch (line[1].type) {
//         case TokenType::OP_HASH:
//             instr.mode = AddressMode::Immediate;
//             instr.imm16 = std::get<uint16_t>(line[2].value);
//             break;

//         case TokenType::OP_DOLLAR:
//             return opDollar(line);

//         case TokenType::LABEL:
//             if (std::get<std::string>(line[1].value) == "A") {
//                 instr.mode = AddressMode::Accumulator;
//                 break;
//             }

//         default:
//             throw "Not implememnted.";
//     }

//     return instr;
// }

// static Instruction opDollar(std::vector<Token> &line) {
//     Instruction instr;

//     instr.imm16 = std::get<uint16_t>(line[2].value);
//     instr.opCode = std::get<std::string>(line[0].value);

//     bool is16Bit = instr.imm16 > UINT8_MAX;
//     int length = line.size();

//     if (length == 2) {
//         instr.mode = AddressMode::Zeropage;
//     } else if (is16Bit && length == 3) {
//         instr.mode = AddressMode::Absolute;
//     } else if (is16Bit && std::get<std::string>(line[4].value) == "X") {
//     }
// }