#include "compiler.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> getFileLines(std::string filename) {
    std::ifstream file(filename);
    std::vector<std::string> lines;

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
    }

    return lines;
}

int main() {
    auto lines = getFileLines("test.asm");
    std::vector<Instruction> instructions;

    for (int i = 0; i < lines.size(); i++) {
        Lexer lexer(lines[i]);
        std::vector<Token> tokens = lexer.tokenise();

        for (int j = 0; j < tokens.size(); j++) {
            std::cout << tokens[j] << " ";
        }
        std::cout << std::endl;

        auto instr = parse(tokens);
        instructions.push_back(instr);

        std::cout << instr << std::endl;
    }

    compile("program.bin", instructions);

    return 0;
}