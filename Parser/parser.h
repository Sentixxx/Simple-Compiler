

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
        try {
            int                     i = 0;
            std::stack<std::string> s;
            s.push("Program");
            // std::cout << TL.tok_lis.size() << "\n";
            while (i < TL.tok_lis.size() && !s.empty()) {
                std::string t = s.top();
                // std::cout << t << " " << s.size() << "\n";
                if (gram.isTerminal(t)) {
                    std::cout << "handle: " << handleToken(i, TL).first << "\n";
                    if (t == handleToken(i, TL).first) {
                        i++;
                        s.pop();
                    }
                    else {
                        std::cout << "N\n";
                        return;
                    }
                }
                else if (gram.isNonTerminal(t)) {
                    std::cout << s.top() << "-->";
                    s.pop();
                    auto table_unit = gram.table[{t, handleToken(i, TL).first}];
                    // std::cout << handleToken(i, TL).first << "\n";
                    // std::cout << "size:" << table_unit.size() << " ";
                    if (table_unit.size() > 1 || table_unit.size() == 0) {
                        std::cerr << table_unit.size() << ", "
                                  << handleToken(i, TL).first << "\n";
                        std::string e = "Syntax Error";
                        e += " at ";
                        e += std::to_string(TL.tok_lis[i].line) + ":" +
                             std::to_string(TL.tok_lis[i].column);
                        if (table_unit.size() == 0)
                            throw e;
                        throw "LL1 Error";
                    }
                    auto production = gram.findProductionR(table_unit[0]);
                    for (int j = production.size() - 1; j >= 0; j--) {
                        if (production[j] == "EPSILON")
                            continue;
                        s.push(production[j]);
                        // std::cout << production[j] << " ";
                    }
                    // std::cout << "\n";
                }
            }
            while (!s.empty() && gram.isNullable(s.top())) {
                // std::cerr << s.top() << "\n";
                s.pop();
            }
            if (i == TL.tok_lis.size() && s.empty()) {
                std::cout << "YES";
                return;
            }
            std::cout << "NO";
        }
        catch (const std::string errorMessage) {
            std::cerr << "Caught exception: " << errorMessage << std::endl;
            std::cout << "NO";
        }
    }
    void show(std::string L, Grammar& G, std::string t)
    {
        auto x = G.table[{L, t}];
        for (auto& j : x) {
            std::cout << G.findProductionL(j) << "->";
            auto k = G.findProductionR(j);
            for (auto& value : k) {
                std::cout << value << " ";
            }
            std::cout << std::endl;
        }
    }
};
#endif  // PARSER_H