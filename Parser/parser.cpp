#include "../Common/token.h"
#include <fstream>
#include <iostream>
#include <vector>

std::string   inputFile  = "../Data/lexer_token.out";
std::string   outputFile = "../Data/parser_list.out";
std::ifstream is         = std::ifstream(inputFile);
std::ofstream os         = std::ofstream(outputFile);

bool read_flag = true;

TokenInfo readToken(std::ifstream& in)
{
    if (in.eof())
        read_flag = false;
    TokenInfo token;
    int       token_type;
    in >> token.line >> token.column >> token.lexeme >> token_type;
    token.token = static_cast<Token>(token_type);
    return token;
}

std::vector<TokenInfo> token_list;

int main() {}
