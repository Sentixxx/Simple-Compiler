

// parse.h
#ifndef PARSER_H
#define PARSER_H

#include "../Common/compiler.h"
#include "../Common/error.h"
#include "../Common/token.h"
#include <fstream>
#include <iostream>
#include <string>

class TopParser : public Module {
private:
    int         i = 0;
    int         maxi = 0;
    TokenList& TL;
    std::string handleToken() {
        std::pair<std::string , std::string> token = TL.getTokenType(i);
        if (token.first == "number")
            return "INTC";
        else if (token.first == "float")
            return "DECI";
        else if (token.first == "identifier") {
            return "id";
        }
        else if ((token.second == "float" || token.second == "string" || token.second == "int") && token.first == "reserved word") {
            return "TYPE";
        }
        else if (token.first == "compare operator") {
            return "CmOp";
        }
        else {
            return token.second;
        }
    }

    void next() {
        if (i == TL.tok_lis.size()) {
            throw "Error: Unexpected end of file\n";
            return;
        }
        i++;
    }

    void varList() {
        if (handleToken() == "TYPE") {
            next();
            if (handleToken() == "id") {
                next();
                if (handleToken() == ",") {
                    next();
                    if (handleToken() == "TYPE") {
                        varList();
                    }
                    else {
                        throw "Error: Expected id, but find" + handleToken() + "\n";
                    }
                }
                else {
                    return;
                }
            }
            else {
                throw "Error: Expected id , but find" + handleToken() + "\n";
            }
        }
        else {
            return;
        }
    }

    void FunDec() {
        //(
        next();
        varList();
        if (handleToken() == ")") {
            next();
        }
        else {
            throw "Error: Expected ) , but find" + handleToken() + "\n";
        }
    }

    void Stmt();

    void ConditionalExp() {

    }

    void ConditionalStmt() {
        next();
        if (handleToken() == "(") {
            next();
            ConditionalExp();
            if (handleToken() == ")") {
                next();
                Stmt();
                if (handleToken() == "else") {
                    next();
                    Stmt();
                }
            }
            else {
                throw "Error: Expected ) , but find" + handleToken() + "\n";
            }
        }
    }

    void Stmt() {
        if (handleToken() == ";") {
            next();
            return;
        }
        else if (handleToken() == "if") {
            ConditionalStmt();
        }
        else {
            throw "Error: Expected ( , but find" + handleToken() + "\n";
        }
    }




    void StmtList() {
        if (handleToken() == ";" || handleToken() == "if" || handleToken() == "while" || handleToken() == "return" || handleToken() == "id" || handleToken() == "{" || handleToken() == "TYPE" || handleToken() == "break") {
            Stmt();
            StmtList();
        }
        else {
            return;
        }
    }

    void CompSt() {
        if (handleToken() == "{") {
            next();
            StmtList();
            if (handleToken() == "}") {
                next();
                return;
            }
            else {
                throw "Error: Expected } , but find" + handleToken() + "\n";
            }
        }
        else {
            throw "Error: Expected { , but find" + handleToken() + "\n";
        }
    }

    void ExtDef() {
        //TYPE
        next();
        if (handleToken() == "id") {
            next();
            if (handleToken() == "(") {
                FunDec();
                CompSt();
            }
            else if (handleToken() == ",") {
                ExtDef();
            }
            else if (handleToken() == ";") {
                return;
            }
            else {
                throw "Error: Expected ( or , or ; , but find" + handleToken() + "\n";
            }
        }
        else {
            throw "Error: Expected id , but find" + handleToken() + "\n";
        }
    }

    void ExtDefList() {
        if (handleToken() == "TYPE") {
            ExtDef();
            ExtDefList();
        }
        else {
            //EPSILON
            return;
        }
    }

    void Program() {
        while (i < TL.tok_lis.size()) {
            ExtDefList();
        }
    }


public:
    TopParser(TokenList& TL) : TL(TL) {
        this->inputFile = "../Data/lexer_token.out";
        this->outputFile = "../Data/parser_list.out";
        this->is = std::ifstream(inputFile);
        this->os = std::ofstream(outputFile);
    }
    void lparse() {
        try {
            Program();
            if (i == TL.tok_lis.size() - 1) {
                std::cout << "YES";
                return;
            }
            else {
                std::cout << "NO\n";
                std::cerr << "Error at " << TL.tok_lis[maxi].line << ":"
                    << TL.tok_lis[maxi].column << "\n";
            }
        }
        catch (const char* e) {
            std::cerr << "Caught Error: " << e << "\n";
        }
    }
};
#endif  // PARSER_H