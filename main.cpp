#include <iostream>
#include <string>
#include <optional>
#include <vector>
#include <fstream>

#include "./Tokenizer.hpp"

int main()
{
    std::cout << "Enter string to evaluate: ";
    
    std::string to_eval = "";

    std::getline(std::cin, to_eval);

    Tokenizer tokenizer(to_eval);

    std::vector<Token> tokens = tokenizer.tokenize();

    bool is_valid = tokenizer.validate(tokens);

    if (is_valid) {
        std::cout << "Valid" << std::endl;
    } else {
        std::cout << "Invalid" << std::endl;
    }

    std::ofstream Output("tokens.txt");

    tokenizer.tokens_to_file(Output, tokens);

    Output.close();
}
