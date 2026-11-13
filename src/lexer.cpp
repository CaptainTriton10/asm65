#include "lexer.hpp"
#include "utils.hpp"
#include <vector>

void Lexer::advance() {
    if (nextIndex < line.size()) {
        currentChar = line[nextIndex];
        // std::cout << std::hex << (int)currentChar << " ";
        // std::cout << currentChar << " ";
        nextIndex++;
    } else {
        currentChar = '\0';
    }
}

std::vector<Token> Lexer::tokenise() {
    std::vector<Token> tokens;

    while (currentChar != '\0') {
        if (emptyChars.contains(currentChar)) {
            advance();
            continue;
        } else if (textChars.contains(currentChar)) {
            auto text = ingestText();

            if (arrContains(instructions, text)) {
                tokens.push_back({TokenType::INSTR, text});
            } else {
                tokens.push_back({TokenType::LABEL, text});
            }

            continue;
        } else if (numChars.contains(currentChar)) {
            auto value = ingestNumber();
            tokens.push_back({TokenType::VALUE, value});

            continue;
        }

        switch (currentChar) {
            case '#':
                tokens.push_back({TokenType::OP_HASH, "#"});
                break;
            case ',':
                tokens.push_back({TokenType::OP_COMMA, ","});
                break;
            case '$':
                tokens.push_back({TokenType::OP_DOLLAR, "$"});
                break;
            case '(':
                tokens.push_back({TokenType::OP_LPAREN, "("});
                break;
            case ')':
                tokens.push_back({TokenType::OP_RPAREN, ")"});
                break;
            default:
                std::cerr << "Lexer: unknown token " << currentChar
                          << std::endl;
        }

        advance();
    }

    return tokens;
}

std::string Lexer::ingestText() {
    std::string text = "";

    while (currentChar != '\0') {
        if (textChars.contains(currentChar) || numChars.contains(currentChar)) {
            text += currentChar;
            advance();
        } else {
            break;
        }
    }

    return text;
}

uint8_t Lexer::ingestNumber() {
    std::string text = "";

    while (currentChar != '\0') {
        if (numChars.contains(currentChar)) {
            text += currentChar;
            advance();
        } else {
            break;
        }
    }

    return (uint8_t)std::stoi(text);
}