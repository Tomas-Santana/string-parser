#pragma once

#include <string>
#include <vector>
#include <optional>

enum class TokenType { 
    ABRIR,
    CERRAR,
    INT_LIT,
    IDENTIFIER,
    ASSIGN,
    COMPARE,
    UNDEF
};

enum class CoolBool {
    TRUE,
    FALSE,
    IND = -1,
};

struct Token {
    TokenType _type;
    int pos;
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
                    tokens.push_back({TokenType::ABRIR, _index});

                    buf.clear();
                    continue;

                } else if (buf == "CERRAR") {
                    if (std::isspace(peek(-7).value())) {
                        tokens.push_back({TokenType::CERRAR, _index});
                    }
                    else {
                        std::cerr << "Missing space before token CERRAR" << std::endl;
                        exit(1);
                    }

                    buf.clear();
                    continue;

                } else {
                    tokens.push_back({TokenType::IDENTIFIER, _index,buf});

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

                tokens.push_back({TokenType::INT_LIT, _index,buf});

                buf.clear();
                continue;
            }

            else if (peek().value() == '=') {
                buf.push_back(consume().value());
                if (peek().has_value() && peek().value() == '=') {
                    buf.push_back(consume().value());

                    tokens.push_back({TokenType::COMPARE,_index, buf});
                }
                else {
                    tokens.push_back({TokenType::ASSIGN, _index});
                }
                buf.clear();
                continue;   
            }
            else if (peek().value() == '<' || peek().value() == '>'){
                buf.push_back(consume().value());

                if (peek().has_value() && peek().value() == '=') {
                    buf.push_back(consume().value());
                }
                tokens.push_back({TokenType::COMPARE, _index, buf});
                buf.clear();
                continue;
            }
            else if (peek().value() == '!') {
                buf.push_back(consume().value());

                if (peek().has_value() && peek().value() == '=') {
                    buf.push_back(consume().value());
                    tokens.push_back({TokenType::COMPARE, _index, buf});
                }
                else {
                    std::cerr << "Error, invalid character at position " << _index << ": " << buf << std::endl;
                    tokens.push_back({TokenType::UNDEF, _index, buf});
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
                std::cerr << "Error, invalid character at position " << _index << ": " << buf << std::endl;

                tokens.push_back({TokenType::UNDEF, _index, buf});
            }
        }
        _index = 0;
        return tokens;
    }

    int evaluate(std::vector<Token> tokens = {}) {
        if (tokens.empty()) {
            tokens = tokenize();
        }
        if (!validate(tokens)) return -1;
            

        int first = std::stoi(tokens[3].value.value());
        int second = std::stoi(tokens[5].value.value());

        std::string operand = tokens[4].value.value();
        if(operand == "==") return first == second;
        else if (operand == ">") return first > second;
        else if (operand == "<") return first < second;
        else if (operand == "<=") return first <= second;
        else if (operand == ">=") return first  >= second;

        return -1;
    }

    bool validate(std::vector<Token> tokens = {}) {
        bool validity = true;
        if (tokens.empty()) {
            tokens = tokenize();
        }
        if (tokens[0]._type != TokenType::ABRIR) {
            std::cout << "Error, expected ABRIR at position 0, got " << token_type_to_string(tokens[0]) << std::endl;
            std::cout << _input << std::endl;
            // print a ^ under the error
            for (int i = 0; i < tokens[0].pos -1; i++) {
                std::cout << " ";
            }
            std::cout<< "^" << std::endl;
            
            return false;
        }
        // the last token must be CERRAR
        if (tokens[tokens.size() - 1]._type != TokenType::CERRAR) {
            std::cout << "Error, expected CERRAR as last token, got " << token_type_to_string(tokens[tokens.size() - 1]) << std::endl;
            std::cout << _input << std::endl;
            // print a ^ under the error
            for (int i = 0; i < tokens[tokens.size() - 1].pos -1; i++) {
                std::cout << " ";
            }
            std::cout<< "^" << std::endl;
            return false;
        }

        // second token must be an IDENTIFIER
        if (tokens[1]._type != TokenType::IDENTIFIER) {
            std::cout << "Error, expected IDENTIFIER at position " << tokens[1].pos << " got " << token_type_to_string(tokens[1]) << std::endl;
            std::cout << _input << std::endl;
            // print a ^ under the error
            for (int i = 0; i < tokens[1].pos -1; i++) {
                std::cout << " ";
            }
            std::cout<< "^" << std::endl;
            return false;
        }

        // third token must be an ASSIGN
        if (tokens[2]._type != TokenType::ASSIGN) {
            std::cout << "Error, expected ASSIGN at position " << tokens[2].pos << " got " << token_type_to_string(tokens[2]) << std::endl;
            std::cout << _input << std::endl;
            // print a ^ under the error
            for (int i = 0; i < tokens[2].pos -1; i++) {
                std::cout << " ";
            }
            std::cout<< "^" << std::endl;
            return false;
        }
        // fourth  token must be a NUMBER
        if (tokens[3]._type != TokenType::INT_LIT) {
            std::cout << "Error, expected NUMBER at position " << tokens[3].pos << " got " << token_type_to_string(tokens[3]) << std::endl;
            std::cout << _input << std::endl;
            // print a ^ under the error
            for (int i = 0; i < tokens[3].pos -1; i++) {
                std::cout << " ";
            }
            std::cout<< "^" << std::endl;
            
            return false;
        }
        if (tokens[5]._type != TokenType::INT_LIT) {
            std::cout << "Error, expected NUMBER at position " << tokens[5].pos << " got " << token_type_to_string(tokens[5]) << std::endl;
            std::cout << _input << std::endl;
            // print a ^ under the error
            for (int i = 0; i < tokens[5].pos -1; i++) {
                std::cout << " ";
            }
            std::cout<< "^" << std::endl;
            return false;
        }
        // fifth token must be a COMPARE
        if (tokens[4]._type != TokenType::COMPARE) {
            std::cout << "Error, expected COMPARE at position " << tokens[4].pos << " got " << token_type_to_string(tokens[4]) << std::endl;
            std::cout << _input << std::endl;
            // print a ^ under the error
            for (int i = 0; i < tokens[4].pos -1; i++) {
                std::cout << " ";
            }
            std::cout<< "^" << std::endl;
            return false;
        }
        if (tokens.size() != 7) {
            std::cout << "Error, expected 7 tokens, got " << tokens.size() << std::endl;
            return false;
        }
        return true;
    }

    std::string token_type_to_string(Token token) {
        switch (token._type)
        {
        case TokenType::ABRIR:
            return "ABRIR";
            break;
        case TokenType::ASSIGN:
            return "ASSIGN";
            break;
        case TokenType::CERRAR:
            return "CERRAR";
            break;
        case TokenType::COMPARE:
            return "COMPARE";
            break;
        case TokenType::IDENTIFIER:
            return "IDENTIFIER: " + token.value.value();
            break;
        case TokenType::INT_LIT:
            return "INT_LIT: " + token.value.value();
            break;
        case TokenType::UNDEF:
            return "UNDEF: " + token.value.value();
        default:
            break;
        }
        return "";
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
            case TokenType::UNDEF:
                OutputFile << "UNDEFINED CHARACTER: " << token.value.value() << std::endl;
            case TokenType::INT_LIT:
                OutputFile << "INT_LIT: " << token.value.value() << std::endl;
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