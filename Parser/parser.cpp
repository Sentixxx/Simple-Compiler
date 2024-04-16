#include "../Common/error.h"
#include "../Common/token.h"
#include "grammar.h"
#include <bits/stdc++.h>

std::string   inputFile = "../Data/lexer_token.out";
std::string   outputFile = "../Data/parser_list.out";
std::ifstream is = std::ifstream(inputFile);
std::ofstream os = std::ofstream(outputFile);


TokenList TL;

struct Node;

using NodeData = std::variant < std::string , std::vector<Node>>;

struct Node {
    std::string type;
    NodeData    data;
};

std::vector<Node> node_list;

void initNode() {
    Node Program = { "Program",
    std::vector <Node> {
        {}
    }
    };
    std::cout << Program.data.index() << std::endl;
}

bool isTerminal(TokenInfo token) {
    return true;
}

TokenInfo getTerminal(TokenInfo token) {
    return token;
}

void First() {

}

void lparse() {
    int i = 0;
    std::stack <TokenInfo> s;
    s.push(TokenInfo(0 , 0 , "Program" , tok_comment));
    while (!s.empty()) {
        if (isTerminal(s.top())) {
            if (TL.tok_lis[i++] == getTerminal(s.top())) {
                s.pop();
            }
            else {
                throw "Syntax error";
            }
        }
        else {

        }
    }
}

int main() {
    Grammar gram;
    TL.read(is);
}