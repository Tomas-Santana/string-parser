#pragma once
#include <string>
#include <vector>
#include <optional>
#include <variant>
#include <iostream>


#include "./Tokenizer.hpp"

struct NodeCompare;
struct NodeExpr;
struct NodeAssign;
struct NodeOutput;


struct NodeCompare;
struct NodeAssign;

struct NodeExpr {
    // can be int_lit or NodeCompare
    std::variant<Token, NodeCompare, NodeAssign> expr; 
};

struct NodeAssign {
    Token identifier;
    NodeExpr expr;
};

struct NodeCompare {
    NodeExpr left;
    NodeExpr right;
    Token compare;
};
struct NodeOutput {
    NodeExpr expr;
};

class Parser
{
    public:
        inline explicit Parser(std::vector<Token> tokens)
            : _tokens(std::move(tokens))
        {
        }

        std::optional<NodeOutput> parse() {
            std::optional<NodeOutput> output;
            while (peek().has_value())
            {
                if (peek().value()._type == TokenType::ABRIR) {
                    consume();
                    auto expr = parse_expr();
                    if (expr.has_value()) {
                        output = NodeOutput{expr.value()};
                    }
                    else {
                        std::cerr << "Error parsing expression" << std::endl;
                        exit(EXIT_FAILURE);
                    }
                    
                }
            }
            
        }

        std::optional<NodeExpr> parse_expr() {
            std::optional<NodeExpr> expr;

            if (peek().value()._type == TokenType::INT_LIT) {
                expr = NodeExpr{consume().value()};
            }
            else if (peek().value()._type == TokenType::IDENTIFIER) {
                Token identifier = consume().value();
                if (peek().value()._type == TokenType::ASSIGN) {
                    consume();
                    auto expr = parse_expr();
                    if (expr.has_value()) {
                        expr = NodeExpr{NodeAssign{identifier, expr.value()}};
                    }
                    else {
                        std::cerr << "Error parsing expression" << std::endl;
                        exit(EXIT_FAILURE);
                    }
                }
            
            return expr;
        }
        }

    private:
        const std::vector<Token> _tokens;
        size_t _index = 0;
        
        std::optional<Token> peek(int ahead = 0) const {
            if (_index + ahead >= _tokens.size()) {
                return std::nullopt;
            }
            return _tokens.at(_index + ahead);
        }

        std::optional<Token> consume() {
            if (_index >= _tokens.size()) {
                return std::nullopt;
            }
            return _tokens.at(_index++);
        }
};