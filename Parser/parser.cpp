#include "../Common/token.h"
#include <fstream>
#include <iostream>
#include <vector>

std::string   inputFile = "../Data/lexer_token.out";
std::string   outputFile = "../Data/parser_list.out";
std::ifstream is = std::ifstream(inputFile);
std::ofstream os = std::ofstream(outputFile);

bool read_flag = true;

TokenInfo readToken(std::ifstream& in) {
    if (in.eof())
        read_flag = false;
    TokenInfo token;
    int       token_type;
    in >> token.line >> token.column >> token.lexeme >> token_type;
    token.token = static_cast<Token>(token_type);
    return token;
}

std::vector<TokenInfo> token_list;
void read() {
    while (read_flag) {
        token_list.push_back(readToken(is));
    }
}


//因子->项->表达式->赋值语句->语句->语句列->程序

class TokenNode {
public:
    TokenInfo token;
    TokenNode* next;

    TokenNode(TokenInfo token) : token(token) , next(nullptr) {}
};

TokenNode root = TokenNode(TokenInfo(0 , 0 , "program" , tok_comment));



void handleToken() {

}

int main() {

}
