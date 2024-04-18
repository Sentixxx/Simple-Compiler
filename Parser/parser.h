

// parse.h
#ifndef PARSER_H
#define PARSER_H

#include "../Common/compiler.h"
#include "../Common/error.h"
#include "../Common/token.h"
#include "grammar.h"
#include <fstream>
#include <iostream>
#include <string>

class Parser : public Module {
  public:
    Parser()
    {
        this->inputFile  = "../Data/lexer_token.out";
        this->outputFile = "../Data/parser_list.out";
        this->is         = std::ifstream(inputFile);
        this->os         = std::ofstream(outputFile);
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
            s.push("#");
            s.push("Program");
            // std::cout << TL.tok_lis.size() << "\n";
            while (i < TL.tok_lis.size() && !s.empty()) {
                std::string t = s.top();
                std::cout << t << " " << s.size() << "\n";
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
                    // std::cout << handleToken(i, TL).first <<
                    // "\n"; std::cout << "size:" <<
                    // table_unit.size() << " ";
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
            // while (!s.empty() && gram.isNullable(s.top())) {
            //     // std::cerr << s.top() << "\n";
            //     s.pop();
            // }
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
class TopParser : protected Module {
  private:
    int                                 i = 0;
    TokenList&                          TL;
    Grammar&                            G;
    std::pair<std::string, std::string> handleToken()
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
    void next()
    {
        i++;
    }
    void back()
    {
        i--;
    }

  public:
    TopParser(TokenList& TL, Grammar& G) : TL(TL), G(G)
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
    bool parseTYPE()
    {
        // std::cout << "parseTYPE --> ";
        if (handleToken().first == "int" || handleToken().first == "float" ||
            handleToken().first == "string") {
            return true;
        }
        return false;
    }
    bool parseExtDefRest()
    {
        if (handleToken().first == ",") {
            next();
            if (handleToken().first == "id") {
                next();
                if (parseExtDefRest())
                    return true;
                back();
            }
            back();
        }
    }
    bool parseVarListRest()
    {
        if (handleToken().first == ",") {
            next();
            if (parseTYPE()) {
                next();
                if (handleToken().first == "id") {
                    next();
                    if (parseVarListRest())
                        return true;
                    back();
                }
                back();
            }
            back();
        }
        return true;
    }
    bool parseVarList()
    {
        if (parseTYPE()) {
            next();
            if (handleToken().first == "id") {
                next();
                if (parseVarListRest())
                    return true;
                back();
            }
            back();
        }
        return true;
    }
    bool parseFunDec()
    {
        if (handleToken().first == "(") {
            next();
            if (parseVarList())
                if (handleToken().first == ")") {
                    next();
                    return true;
                }
            back();
        }
        return false;
    }
    bool parseCompSt()
    {
        if (handleToken().first == "{") {
            next();
            if (parseStmtList()) {
                if (handleToken().first == "}") {
                    next();
                    return true;
                }
            }
            back();
        }
        return false;
    }

    bool parseExtDef()
    {
        // std::cout << "parseExtDef --> ";
        if (parseTYPE()) {
            next();
            if (handleToken().first == "id") {
                next();
                if (parseExtDefRest())
                    if (handleToken().first == ";") {
                        next();
                        return true;
                    }
                back();
            }
            if (parseFunDec())
                if (parseCompSt())
                    return true;
            back();
        }
        return false;
    }
    bool parseExtDefList()
    {
        // std::cout << "parseExtDefList --> ";
        if (parseExtDef()) {
            if (parseExtDefList())
                return true;
            else {
                return false;
            }
        }
        return true;
    }
    bool parseProgram()
    {
        // std::cout << "parseProgram --> ";
        if (parseExtDefList()) {
            return true;
        }
        return false;
    }
    void lparse()
    {
        try {
            parseProgram();
        }
        catch (const std::string e) {
            std::cerr << e << "\n";
            std::cout << "NO";
        }
    }
};
#endif  // PARSER_H