#include "compiler.hpp"
#include "magic_enum.hpp"
#include "parser.hpp"
#include <fstream>
#include <iostream>

OpCode *opCodes = new OpCode[256]{};

static uint8_t getOpCode(Instruction instr) {
    for (uint8_t i = 0; i < 256; i++) {
        if (opCodes[i].addressMode == instr.mode &&
            opCodes[i].instr == instr.opCode) {
            return i;
        }
    }

    return 0U;
}

static std::vector<uint8_t> getBytes(Instruction instr) {
    std::vector<uint8_t> data = {};

    uint8_t op = getOpCode(instr);
    data.push_back(op);

    switch (instr.mode) {
        case AddressMode::IndirectX:
        case AddressMode::IndirectY:
        case AddressMode::Relative:
        case AddressMode::Zeropage:
        case AddressMode::ZeropageX:
        case AddressMode::ZeropageY:
        case AddressMode::Immediate:
            data.push_back((uint8_t)(instr.imm16 & 0x00FF));
            break;

        case AddressMode::Indirect:
        case AddressMode::Absolute:
        case AddressMode::AbsoluteX:
        case AddressMode::AbsoluteY:
            data.push_back((uint8_t)(instr.imm16 & 0xFF00));
            data.push_back((uint8_t)(instr.imm16 & 0x00FF));
            break;
    }

    return data;
}

void compile(std::string filename, std::vector<Instruction> instructions) {
    std::ofstream binfile(filename);
    int fileSize = 0;

    initOpCodes();

    std::vector<uint8_t> data = {};

    if (!binfile.is_open())
        return;

    for (int i = 0; i < instructions.size(); i++) {
        auto bytes = getBytes(instructions[i]);

        data.reserve(data.size() + bytes.size());
        data.insert(data.end(), bytes.begin(), bytes.end());

        fileSize += bytes.size();
    }

    binfile.write((char *)data.data(), fileSize * sizeof(uint8_t));
}

OpCode *initOpCodes() {
    // ADC - Add with Carry
    opCodes[0x69] = {AddressMode::Immediate, "ADC"};
    opCodes[0x65] = {AddressMode::Zeropage, "ADC"};
    opCodes[0x75] = {AddressMode::ZeropageX, "ADC"};
    opCodes[0x6D] = {AddressMode::Absolute, "ADC"};
    opCodes[0x7D] = {AddressMode::AbsoluteX, "ADC"};
    opCodes[0x79] = {AddressMode::AbsoluteY, "ADC"};
    opCodes[0x61] = {AddressMode::IndirectX, "ADC"};
    opCodes[0x71] = {AddressMode::IndirectY, "ADC"};

    // AND - Logical AND
    opCodes[0x29] = {AddressMode::Immediate, "AND"};
    opCodes[0x25] = {AddressMode::Zeropage, "AND"};
    opCodes[0x35] = {AddressMode::ZeropageX, "AND"};
    opCodes[0x2D] = {AddressMode::Absolute, "AND"};
    opCodes[0x3D] = {AddressMode::AbsoluteX, "AND"};
    opCodes[0x39] = {AddressMode::AbsoluteY, "AND"};
    opCodes[0x21] = {AddressMode::IndirectX, "AND"};
    opCodes[0x31] = {AddressMode::IndirectY, "AND"};

    // ASL - Arithmetic Shift Left
    opCodes[0x0A] = {AddressMode::Accumulator, "ASL"};
    opCodes[0x06] = {AddressMode::Zeropage, "ASL"};
    opCodes[0x16] = {AddressMode::ZeropageX, "ASL"};
    opCodes[0x0E] = {AddressMode::Absolute, "ASL"};
    opCodes[0x1E] = {AddressMode::AbsoluteX, "ASL"};

    // BCC - Branch if Carry Clear
    opCodes[0x90] = {AddressMode::Relative, "BCC"};

    // BCS - Branch if Carry Set
    opCodes[0xB0] = {AddressMode::Relative, "BCS"};

    // BEQ - Branch if Equal
    opCodes[0xF0] = {AddressMode::Relative, "BEQ"};

    // BIT - Bit Test
    opCodes[0x24] = {AddressMode::Zeropage, "BIT"};
    opCodes[0x2C] = {AddressMode::Absolute, "BIT"};

    // BMI - Branch if Minus
    opCodes[0x30] = {AddressMode::Relative, "BMI"};

    // BNE - Branch if Not Equal
    opCodes[0xD0] = {AddressMode::Relative, "BNE"};

    // BPL - Branch if Plus
    opCodes[0x10] = {AddressMode::Relative, "BPL"};

    // BRK - Break
    opCodes[0x00] = {AddressMode::Implied, "BRK"};

    // BVC - Branch if Overflow Clear
    opCodes[0x50] = {AddressMode::Relative, "BVC"};

    // BVS - Branch if Overflow Set
    opCodes[0x70] = {AddressMode::Relative, "BVS"};

    // CLC - Clear Carry
    opCodes[0x18] = {AddressMode::Implied, "CLC"};

    // CLD - Clear Decimal
    opCodes[0xD8] = {AddressMode::Implied, "CLD"};

    // CLI - Clear Interrupt Disable
    opCodes[0x58] = {AddressMode::Implied, "CLI"};

    // CLV - Clear Overflow
    opCodes[0xB8] = {AddressMode::Implied, "CLV"};

    // CMP - Compare Accumulator
    opCodes[0xC9] = {AddressMode::Immediate, "CMP"};
    opCodes[0xC5] = {AddressMode::Zeropage, "CMP"};
    opCodes[0xD5] = {AddressMode::ZeropageX, "CMP"};
    opCodes[0xCD] = {AddressMode::Absolute, "CMP"};
    opCodes[0xDD] = {AddressMode::AbsoluteX, "CMP"};
    opCodes[0xD9] = {AddressMode::AbsoluteY, "CMP"};
    opCodes[0xC1] = {AddressMode::IndirectX, "CMP"};
    opCodes[0xD1] = {AddressMode::IndirectY, "CMP"};

    // CPX - Compare X Register
    opCodes[0xE0] = {AddressMode::Immediate, "CPX"};
    opCodes[0xE4] = {AddressMode::Zeropage, "CPX"};
    opCodes[0xEC] = {AddressMode::Absolute, "CPX"};

    // CPY - Compare Y Register
    opCodes[0xC0] = {AddressMode::Immediate, "CPY"};
    opCodes[0xC4] = {AddressMode::Zeropage, "CPY"};
    opCodes[0xCC] = {AddressMode::Absolute, "CPY"};

    // DEC - Decrement Memory
    opCodes[0xC6] = {AddressMode::Zeropage, "DEC"};
    opCodes[0xD6] = {AddressMode::ZeropageX, "DEC"};
    opCodes[0xCE] = {AddressMode::Absolute, "DEC"};
    opCodes[0xDE] = {AddressMode::AbsoluteX, "DEC"};

    // DEX - Decrement X
    opCodes[0xCA] = {AddressMode::Implied, "DEX"};

    // DEY - Decrement Y
    opCodes[0x88] = {AddressMode::Implied, "DEY"};

    // EOR - Exclusive OR
    opCodes[0x49] = {AddressMode::Immediate, "EOR"};
    opCodes[0x45] = {AddressMode::Zeropage, "EOR"};
    opCodes[0x55] = {AddressMode::ZeropageX, "EOR"};
    opCodes[0x4D] = {AddressMode::Absolute, "EOR"};
    opCodes[0x5D] = {AddressMode::AbsoluteX, "EOR"};
    opCodes[0x59] = {AddressMode::AbsoluteY, "EOR"};
    opCodes[0x41] = {AddressMode::IndirectX, "EOR"};
    opCodes[0x51] = {AddressMode::IndirectY, "EOR"};

    // INC - Increment Memory
    opCodes[0xE6] = {AddressMode::Zeropage, "INC"};
    opCodes[0xF6] = {AddressMode::ZeropageX, "INC"};
    opCodes[0xEE] = {AddressMode::Absolute, "INC"};
    opCodes[0xFE] = {AddressMode::AbsoluteX, "INC"};

    // INX - Increment X
    opCodes[0xE8] = {AddressMode::Implied, "INX"};

    // INY - Increment Y
    opCodes[0xC8] = {AddressMode::Implied, "INY"};

    // JMP - Jump
    opCodes[0x4C] = {AddressMode::Absolute, "JMP"};
    opCodes[0x6C] = {AddressMode::Indirect, "JMP"};

    // JSR - Jump to Subroutine
    opCodes[0x20] = {AddressMode::Absolute, "JSR"};

    // LDA - Load Accumulator
    opCodes[0xA9] = {AddressMode::Immediate, "LDA"};
    opCodes[0xA5] = {AddressMode::Zeropage, "LDA"};
    opCodes[0xB5] = {AddressMode::ZeropageX, "LDA"};
    opCodes[0xAD] = {AddressMode::Absolute, "LDA"};
    opCodes[0xBD] = {AddressMode::AbsoluteX, "LDA"};
    opCodes[0xB9] = {AddressMode::AbsoluteY, "LDA"};
    opCodes[0xA1] = {AddressMode::IndirectX, "LDA"};
    opCodes[0xB1] = {AddressMode::IndirectY, "LDA"};

    // LDX - Load X Register
    opCodes[0xA2] = {AddressMode::Immediate, "LDX"};
    opCodes[0xA6] = {AddressMode::Zeropage, "LDX"};
    opCodes[0xB6] = {AddressMode::ZeropageY, "LDX"};
    opCodes[0xAE] = {AddressMode::Absolute, "LDX"};
    opCodes[0xBE] = {AddressMode::AbsoluteY, "LDX"};

    // LDY - Load Y Register
    opCodes[0xA0] = {AddressMode::Immediate, "LDY"};
    opCodes[0xA4] = {AddressMode::Zeropage, "LDY"};
    opCodes[0xB4] = {AddressMode::ZeropageX, "LDY"};
    opCodes[0xAC] = {AddressMode::Absolute, "LDY"};
    opCodes[0xBC] = {AddressMode::AbsoluteX, "LDY"};

    // LSR - Logical Shift Right
    opCodes[0x4A] = {AddressMode::Implied, "LSR"};
    opCodes[0x46] = {AddressMode::Zeropage, "LSR"};
    opCodes[0x56] = {AddressMode::ZeropageX, "LSR"};
    opCodes[0x4E] = {AddressMode::Absolute, "LSR"};
    opCodes[0x5E] = {AddressMode::AbsoluteX, "LSR"};

    // NOP - No Operation
    opCodes[0xEA] = {AddressMode::Implied, "NOP"};

    // ORA - Logical OR
    opCodes[0x09] = {AddressMode::Immediate, "ORA"};
    opCodes[0x05] = {AddressMode::Zeropage, "ORA"};
    opCodes[0x15] = {AddressMode::ZeropageX, "ORA"};
    opCodes[0x0D] = {AddressMode::Absolute, "ORA"};
    opCodes[0x1D] = {AddressMode::AbsoluteX, "ORA"};
    opCodes[0x19] = {AddressMode::AbsoluteY, "ORA"};
    opCodes[0x01] = {AddressMode::IndirectX, "ORA"};
    opCodes[0x11] = {AddressMode::IndirectY, "ORA"};

    // PHA - Push Accumulator
    opCodes[0x48] = {AddressMode::Implied, "PHA"};

    // PHP - Push Processor Status
    opCodes[0x08] = {AddressMode::Implied, "PHP"};

    // PLA - Pull Accumulator
    opCodes[0x68] = {AddressMode::Implied, "PLA"};

    // PLP - Pull Processor Status
    opCodes[0x28] = {AddressMode::Implied, "PLP"};

    // ROL - Rotate Left
    opCodes[0x2A] = {AddressMode::Implied, "ROL"};
    opCodes[0x26] = {AddressMode::Zeropage, "ROL"};
    opCodes[0x36] = {AddressMode::ZeropageX, "ROL"};
    opCodes[0x2E] = {AddressMode::Absolute, "ROL"};
    opCodes[0x3E] = {AddressMode::AbsoluteX, "ROL"};

    // ROR - Rotate Right
    opCodes[0x6A] = {AddressMode::Implied, "ROR"};
    opCodes[0x66] = {AddressMode::Zeropage, "ROR"};
    opCodes[0x76] = {AddressMode::ZeropageX, "ROR"};
    opCodes[0x6E] = {AddressMode::Absolute, "ROR"};
    opCodes[0x7E] = {AddressMode::AbsoluteX, "ROR"};

    // RTI - Return from Interrupt
    opCodes[0x40] = {AddressMode::Implied, "RTI"};

    // RTS - Return from Subroutine
    opCodes[0x60] = {AddressMode::Implied, "RTS"};

    // SBC - Subtract with Carry
    opCodes[0xE9] = {AddressMode::Immediate, "SBC"};
    opCodes[0xE5] = {AddressMode::Zeropage, "SBC"};
    opCodes[0xF5] = {AddressMode::ZeropageX, "SBC"};
    opCodes[0xED] = {AddressMode::Absolute, "SBC"};
    opCodes[0xFD] = {AddressMode::AbsoluteX, "SBC"};
    opCodes[0xF9] = {AddressMode::AbsoluteY, "SBC"};
    opCodes[0xE1] = {AddressMode::IndirectX, "SBC"};
    opCodes[0xF1] = {AddressMode::IndirectY, "SBC"};

    // SEC - Set Carry
    opCodes[0x38] = {AddressMode::Implied, "SEC"};

    // SED - Set Decimal
    opCodes[0xF8] = {AddressMode::Implied, "SED"};

    // SEI - Set Interrupt Disable
    opCodes[0x78] = {AddressMode::Implied, "SEI"};

    // STA - Store Accumulator
    opCodes[0x85] = {AddressMode::Zeropage, "STA"};
    opCodes[0x95] = {AddressMode::ZeropageX, "STA"};
    opCodes[0x8D] = {AddressMode::Absolute, "STA"};
    opCodes[0x9D] = {AddressMode::AbsoluteX, "STA"};
    opCodes[0x99] = {AddressMode::AbsoluteY, "STA"};
    opCodes[0x81] = {AddressMode::IndirectX, "STA"};
    opCodes[0x91] = {AddressMode::IndirectY, "STA"};

    // STX - Store X Register
    opCodes[0x86] = {AddressMode::Zeropage, "STX"};
    opCodes[0x96] = {AddressMode::ZeropageY, "STX"};
    opCodes[0x8E] = {AddressMode::Absolute, "STX"};

    // STY - Store Y Register
    opCodes[0x84] = {AddressMode::Zeropage, "STY"};
    opCodes[0x94] = {AddressMode::ZeropageX, "STY"};
    opCodes[0x8C] = {AddressMode::Absolute, "STY"};

    // TAX - Transfer Accumulator to X
    opCodes[0xAA] = {AddressMode::Implied, "TAX"};

    // TAY - Transfer Accumulator to Y
    opCodes[0xA8] = {AddressMode::Implied, "TAY"};

    // TSX - Transfer Stack Pointer to X
    opCodes[0xBA] = {AddressMode::Implied, "TSX"};

    // TXA - Transfer X to Accumulator
    opCodes[0x8A] = {AddressMode::Implied, "TXA"};

    // TXS - Transfer X to Stack Pointer
    opCodes[0x9A] = {AddressMode::Implied, "TXS"};

    // TYA - Transfer Y to Accumulator
    opCodes[0x98] = {AddressMode::Implied, "TYA"};

    // opCodes[0xFF] = {AddressMode::Immediate, "TST"};
    return opCodes;
}