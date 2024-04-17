

// parse.h
#ifndef PARSER_H
#define PARSER_H

#include "../Common/error.h"
#include "../Common/token.h"
#include "grammar.h"
#include <fstream>
#include <iostream>
#include <string>
class Parser {
  private:
    std::string   inputFile;
    std::string   outputFile;
    std::ifstream is;
    std::ofstream os;

  public:
    Parser()
    {
        this->inputFile  = "../Data/lexer_token.out";
        this->outputFile = "../Data/parser_list.out";
        this->is         = std::ifstream(inputFile);
        this->os         = std::ofstream(outputFile);
    }
    std::ifstream& getIs()
    {
        return this->is;
    }
    std::ofstream& getOs()
    {
        return this->os;
    }
    std::pair<std::string, std::string> handleToken(int i, TokenList& TL)
    {
        std::pair<std::string, std::string> token = TL.getTokenType(i);
        if (token.first == "number")
            return {"INTC", token.second};
        else if (token.first == "float")
            return {"DECI", token.second};
        else if (token.first == "identifier") {
            return {"id", token.second};
        }
        else {
            return {token.second, token.second};
        }
    }
    void lparse(Grammar& gram, TokenList& TL)
    {
        int             i = 0;
        std::stack<int> s;
        for (int j = 0; j < gram.P.size(); j++) {
            if (gram.findProductionL(j) == "Program") {
                s.push(j);
                break;
            }
        }

        while (i < TL.tok_lis.size() && !s.empty()) {
            std::string t = gram.findProductionL(s.top());
            std::cout << t << "\n";
            if (gram.isTerminal(t)) {
                if (t == handleToken(i, TL).first) {
                    i++;
                    s.pop();
                }
                else {
                    throw "Syntax Error";
                }
            }
            else if (gram.isNonTerminal(t)) {
                s.pop();
                std::cout << t << "-->";
                for (auto value : gram.table[{t, handleToken(i, TL).first}]) {
                    std::cout << gram.findProductionL(value) << " ";
                    s.push(value);
                }
                std::cout << "\n";
            }
        }
        if (i == TL.tok_lis.size()) {
            std::cout << "Y";
            return;
        }
        std::cout << "N";
    }
};
#endif  // PARSER_H