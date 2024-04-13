#include "../Common/token.h"
#include <fstream>
#include <iostream>

std::string   inputFile  = "../Data/lexer_token.out";
std::string   outputFile = "../Data/parser_list.out";
std::ifstream is         = std::ifstream(inputFile);
std::ofstream os         = std::ofstream(outputFile);

TokenInfo readToken(std::ifstream& in)
{
    if (in.eof())
        exit(0);
    TokenInfo token;
    int       token_type;
    in >> token.line >> token.column >> token.lexeme >> token_type;
    token.token = static_cast<Token>(token_type);
    return token;
}

int main()
{
    while (1) {
        TokenInfo t = readToken(is);
        std::cout << t.line << " " << t.column << " " << t.lexeme << " "
                  << t.token << std::endl;
    }
}