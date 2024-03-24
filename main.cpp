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

    Tokenizer tokenizer(std::move(to_eval));

    std::vector<Token> tokens = tokenizer.tokenize();

    int result = tokenizer.evaluate(tokens);


    std::ofstream Output("tokens.txt");

    if (result == -1) {
        std::cerr << "Could not evaluate" << std::endl;
    } else if (result) {
        std::cout << "True" << std::endl;
        tokenizer.tokens_to_file(Output, tokens);
        Output << "True" << std::endl;
    } else {
        std::cout << "False" << std::endl;
        tokenizer.tokens_to_file(Output, tokens);
        Output << "False" << std::endl;

    }

    Output.close();
}