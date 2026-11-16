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

int main(int argc, char *argv[]) {
    bool debug = false;
    std::string outputStr = "out.bin";

    if (argc == 1) {
        std::cerr << "Must specify file" << std::endl;
        return 1;
    }

    // for (int i = 0; i < argc; i++) {
    //     std::cout << argv[i] == "-o" << std::endl;
    //     if (argv[i] == "-d")
    //         debug = true;

    //     if (argv[i] == "-o") {
    //         std::cout << argv[i + 1] << std::endl;
    //         outputStr = argv[i + 1];
    //     }
    // }

    auto lines = getFileLines(argv[1]);
    std::vector<Instruction> instructions;

    for (int i = 0; i < lines.size(); i++) {
        if (debug) {
            std::cout << "\n### " << lines[i] << " ###\n";
        }

        Lexer lexer(lines[i]);
        std::vector<Token> tokens = lexer.tokenise();

        if (debug) {
            for (int j = 0; j < tokens.size(); j++) {
                std::cout << tokens[j] << " ";
            }
            std::cout << std::endl;
        }

        auto instr = parse(tokens);
        instructions.push_back(instr);

        if (debug) {
            std::cout << instr << std::endl;
        }
    }

    compile(outputStr, instructions);

    return 0;
}