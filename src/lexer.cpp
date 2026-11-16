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

    TokenType prevType = TokenType::INSTR;

    while (currentChar != '\0') {
        if (emptyChars.contains(currentChar)) {
            advance();
            continue;
        }

        if (currentChar == ';')
            break;

        if (textChars.contains(currentChar)) {
            if (prevType != TokenType::OP_DOLLAR) {
                auto text = ingestText();

                if (arrContains(instructions, text)) {
                    tokens.push_back({TokenType::INSTR, text});
                    prevType = TokenType::INSTR;
                } else {
                    tokens.push_back({TokenType::LABEL, text});
                    prevType = TokenType::LABEL;
                }

                continue;
            }
        }

        if (numChars.contains(currentChar)) {
            std::string value = ingestNumber();

            if (value.size() >= 4) {
                tokens.push_back({
                    TokenType::VALUE,
                    (uint16_t)std::stoi(value, nullptr, 16),
                    true,
                });

            } else {
                tokens.push_back({
                    TokenType::VALUE,
                    (uint16_t)std::stoi(value, nullptr, 16),
                });
            }

            prevType = TokenType::VALUE;
            continue;
        }

        switch (currentChar) {
            case '#':
                tokens.push_back({TokenType::OP_HASH, "#"});
                prevType = TokenType::OP_HASH;
                break;
            case ',':
                tokens.push_back({TokenType::OP_COMMA, ","});
                prevType = TokenType::OP_COMMA;
                break;
            case '$':
                tokens.push_back({TokenType::OP_DOLLAR, "$"});
                prevType = TokenType::OP_DOLLAR;
                break;
            case '(':
                tokens.push_back({TokenType::OP_LPAREN, "("});
                prevType = TokenType::OP_LPAREN;
                break;
            case ')':
                tokens.push_back({TokenType::OP_RPAREN, ")"});
                prevType = TokenType::OP_RPAREN;
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

std::string Lexer::ingestNumber() {
    std::string text = "";

    while (currentChar != '\0') {
        if (numChars.contains(currentChar)) {
            text += currentChar;
            advance();
        } else {
            break;
        }
    }

    return text;
}