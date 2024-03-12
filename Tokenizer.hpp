#pragma once

#include <string>
#include <vector>
#include <optional>

enum class TokenType { 
    ABRIR,
    CERRAR,
    NUMBER,
    IDENTIFIER,
    ASSIGN,
    COMPARE,
};

struct Token {
    TokenType _type;
    std::optional<std::string> value = std::nullopt;
};


class Tokenizer
{
public:
    inline explicit Tokenizer(std::string input)
        : _input(input)
    {
    }

    inline std::vector<Token> tokenize() {

        std::vector<Token> tokens = {};
        std::string buf;


        while (peek().has_value())
        {
            
            if (std::isalpha(peek().value()) || peek().value() == '_') {
                buf.push_back(consume().value());

                while (
                    peek().has_value() && 
                    (std::isalnum(peek().value()) || peek().value() == '_')
                ) 
                {
                    buf.push_back(consume().value());
                }

                if (buf == "ABRIR") {
                    tokens.push_back({TokenType::ABRIR});

                    buf.clear();
                    continue;

                } else if (buf == "CERRAR") {
                    if (std::isspace(peek(-7).value())) {
                        tokens.push_back({TokenType::CERRAR});
                    }
                    else {
                        std::cout << "Missing space before token CERRAR" << std::endl;
                        exit(1);
                    }

                    buf.clear();
                    continue;

                } else {
                    tokens.push_back({TokenType::IDENTIFIER, buf});

                    buf.clear();
                    continue;
                
                }
            }

            else if (std::isdigit(peek().value())) {
                buf.push_back(consume().value());

                while (peek().has_value() && std::isdigit(peek().value())) 
                {
                    buf.push_back(consume().value());
                }

                tokens.push_back({TokenType::NUMBER, buf});

                buf.clear();
                continue;
            }

            else if (peek().value() == '=') {
                buf.push_back(consume().value());
                if (peek().has_value() && peek().value() == '=') {
                    buf.push_back(consume().value());

                    tokens.push_back({TokenType::COMPARE, buf});
                }
                else {
                    tokens.push_back({TokenType::ASSIGN});
                }
                buf.clear();
                continue;   
            }
            else if (peek().value() == '<' || peek().value() == '>'){
                buf.push_back(consume().value());

                if (peek().has_value() && peek().value() == '=') {
                    buf.push_back(consume().value());
                }
                tokens.push_back({TokenType::COMPARE, buf});
                buf.clear();
                continue;
            }
            else if (peek().value() == '!') {
                buf.push_back(consume().value());

                if (peek().has_value() && peek().value() == '=') {
                    buf.push_back(consume().value());
                    tokens.push_back({TokenType::COMPARE, buf});
                }
                else {
                    std::cout << "Error, invalid character at position " << _index << ": " << buf << std::endl;
                    exit(1);
                }
                buf.clear();
                continue;
            }

            else if (std::isspace(peek().value())) {
                consume();
                continue;
            }

            else {
                buf.push_back(consume().value());
                std::cout << "Error, invalid character at position " << _index << ": " << buf << std::endl;

                exit(1);
            }
        }
        _index = 0;
        return tokens;
    }

    bool validate(std::vector<Token> tokens = {}) {
        if (tokens.empty()) {
            tokens = tokenize();
        }
        // tokens must have exactly 7 elements
        if (tokens.size() != 7) {
            std::cout << "Error, expected 7 tokens, got " << tokens.size() << std::endl;
            return false;
        }
        // the first token must be ABRIR
        if (tokens[0]._type != TokenType::ABRIR) {
            std::cout << "Error, expected ABRIR at position 0" << std::endl;
            return false;
        }
        // the last token must be CERRAR
        if (tokens[tokens.size() - 1]._type != TokenType::CERRAR) {
            std::cout << "Error, expected CERRAR at position " << tokens.size() - 1 << std::endl;
            return false;
        }

        // second token must be an IDENTIFIER
        if (tokens[1]._type != TokenType::IDENTIFIER) {
            std::cout << "Error, expected IDENTIFIER as second token" << std::endl;
            return false;
        }

        // third token must be an ASSIGN
        if (tokens[2]._type != TokenType::ASSIGN) {
            std::cout << "Error, expected ASSIGN as third token" << std::endl;
            return false;
        }
        // fourth  token must be a NUMBER
        if (tokens[3]._type != TokenType::NUMBER) {
            std::cout << "Error, expected NUMBER as fourth token" << std::endl;
            return false;
        }
        if (tokens[5]._type != TokenType::NUMBER) {
            std::cout << "Error, expected NUMBER as sixth token" << std::endl;
            return false;
        }
        // fifth token must be a COMPARE
        if (tokens[4]._type != TokenType::COMPARE) {
            std::cout << "Error, expected COMPARE as fifth token" << std::endl;
            return false;
        }
        return true;
    }

    void tokens_to_file(std::ofstream& OutputFile, std::vector<Token> tokens={}) {

        if (tokens.empty()) {
            tokens = tokenize();
        }

        OutputFile << "Tokens: " << std::endl;
    
        for (const auto& token : tokens) {
            switch (token._type)
            {
            case TokenType::ABRIR:
                OutputFile << "ABRIR" << std::endl;
                break;
            case TokenType::CERRAR:
                OutputFile << "CERRAR" << std::endl;
                break;
            case TokenType::IDENTIFIER:
                OutputFile << "IDENTIFIER: " << token.value.value() << std::endl;
                break;

            case TokenType::NUMBER:
                OutputFile << "NUMBER: " << token.value.value() << std::endl;
                break;
            case TokenType::ASSIGN:
                OutputFile << "ASSIGN" << std::endl;
                break;
            case TokenType::COMPARE:
                OutputFile << "COMPARE: " << token.value.value() << std::endl;
                break;
            default:
                break;
            }
        }
    }

private:
    std::string _input;
    int _index = 0;

    std::optional<char> peek(int ahead = 0) const {
        if (_index + ahead >= _input.size()) {
            return std::nullopt;
        }
        return _input.at(_index + ahead);
    }

    std::optional<char> consume() {
        if (_index >= _input.size()) {
            return std::nullopt;
        }
        return _input.at(_index++);
    }
};