#include "../Common/token.h"
#include "../Common/error.h"
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

class TokenNode {
public:
    TokenInfo token;
    std::string name;
    std::vector<TokenNode*> next;
    TokenNode* parent;
    TokenNode(TokenInfo token) : token(token) , name("Program") {}
    TokenNode(TokenInfo token , std::string name) : token(token) , name(name) {}
};


class Parser {
private:
    std::vector<TokenInfo> token_list;
    TokenNode              root;
    TokenNode* current;
    int                    index;

public:
    Parser(std::vector<TokenInfo> token_list) : token_list(token_list) {
        root = TokenNode(token_list[0]);
        current = &root;
        index = 0;
    }

    void parseType() {
        TokenNode* node = new TokenNode(token_list[index] , "TYPE");
        current->next.push_back(node);
        current = node;
        index++;
    }

    void parseCondStat() {
        TokenNode* node = new TokenNode(token_list[index] , "COND_STAT");
        current->next.push_back(node);
        current = node;
        index++;
    }
    //实验一的类型里面有string,实验二的文法里面没有提到，string要怎么处理？
    void parse() {
        while (index < token_list.size()) {
            TokenInfo token = token_list[index];
            if (token.token == tok_rw) {
                if (token.lexeme == "int" || token.lexeme == "float" || token.lexeme == "string") {
                    parseType();
                }
                else if (token.lexeme == "if") {
                    parseCondStat();
                }
                else if (token.lexeme == "else") {
                    parseIdtElse();
                }
                else if (token.lexeme == "while") {
                    parseIdtWhile();
                }
                else if (token.lexeme == "return") {
                    parseIdtReturn();
                }
                else if (token.lexeme == "break") {
                    parseIdtBreak();
                }
                else if (token.lexeme == "and") {
                    parseIdtAnd();
                }
                else {
                    parseIdentifier();
                }
            }
            else if (token.token == tok_del) {
                if (token.lexeme == "{") {
                    parseDelBlock();
                }
                else if (token.lexeme == ";") {
                    parseDelSemicolon();
                }
                else if (token.lexeme == ",") {
                    parseDelComma();
                }
                else if (token.lexeme == "(") {
                    parseDelLeftParenthesis();
                }
                else if (token.lexeme == ")") {
                    parseDelRightParenthesis();
                }
                else if (token.lexeme == "{") {
                    parseDelLeftBrace();
                }
                else if (token.lexeme == "}") {
                    parseDelRightBrace();
                }
                else {
                    parseDelimiter();
                }
            }
            else if (token.token == tok_bop) {
                parseBasicOperator();
            }
            else if (token.token == tok_cop) {
                parseCompareOperator();
            }
            else if (token.token == tok_aop) {
                parseAssignOperator();
            }
            else if (token.token == tok_str) {
                parseString();
            }
            else {
                parseError();
            }
        }
    }
};

int main() {
    read();
}
