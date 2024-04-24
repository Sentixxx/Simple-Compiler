

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
        std::cerr << "trying " + handleToken() + "\n";
        if (i == TL.tok_lis.size()) {
            throw "Error: Unexpected end of file\n";
            return;
        }
        std::cerr << "success " + handleToken() + "\n";
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
                        throw "Error: Expected id, but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";
                    }
                }
                else {
                    return;
                }
            }
            else {
                throw "Error: Expected id , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";
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
            throw "Error: Expected ) , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";
        }
    }

    void Factor() {
        if (handleToken() == "INTC" || handleToken() == "DECI" || handleToken() == "id") {
            next();
        }
        else if (handleToken() == "(") {
            next();
            Exp();
            if (handleToken() == ")") {
                next();
            }
            else {
                throw "Error: Expected ) , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";
            }
        }
        else {
            throw "Error: Expected Factor , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";
        }
    }

    void Term() {
        if (handleToken() == "INTC" || handleToken() == "DECI" || handleToken() == "id" || handleToken() == "(") {
            Factor();
            while (handleToken() == "*" || handleToken() == "/") {
                next();
                if (handleToken() == "INTC" || handleToken() == "DECI" || handleToken() == "id" || handleToken() == "(")
                    Factor();
                else
                    throw "Error: Expected Factor , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";
            }
        }
        else
            throw "Error: Expected Factor , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";
    }

    void Exp() {
        Term();
        while (handleToken() == "+" || handleToken() == "-") {
            next();
            if (handleToken() == "INTC" || handleToken() == "DECI" || handleToken() == "id" || handleToken() == "(")
                Term();
            else
                throw "Error: Expected Factor , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";
        }
    }

    void CompExp() {
        Exp();
        if (handleToken() == "CmOp") {
            next();
            Exp();
        }
        else {
            throw "Error: Expected Compare Operator , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";
        }
    }
    void RelationExp() {
        CompExp();
        while (handleToken() == "and") {
            next();
            if (handleToken() == "id" || handleToken() == "INTC" || handleToken() == "DECI" || handleToken() == "(") {
                CompExp();
            }
            else {
                throw "Error: Expected Compare Expression , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";
            }
        }
    }
    void ConditionalExp() {
        RelationExp();
        while (handleToken() == "or") {
            next();
            if (handleToken() == "id" || handleToken() == "INTC" || handleToken() == "DECI" || handleToken() == "(") {
                RelationExp();
            }
            else {
                throw "Error: Expected Compare Expression , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";
            }
        }
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
                throw "Error: Expected ) , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";
            }
        }
    }

    void ArgParamList() {
        if (handleToken() == "INTC" || handleToken() == "DECI" || handleToken() == "id" || handleToken() == "(") {
            Exp();
            if (handleToken() == ",") {
                next();
                if (handleToken() == "INTC" || handleToken() == "DECI" || handleToken() == "id" || handleToken() == "(")
                    ArgParamList();
                else {
                    throw "Error: Expected Expression , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";

                }
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
        else if (handleToken() == "break") {
            next();
            if (handleToken() == ";") {
                next();
            }
            else {
                throw "Error: Expected ; , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";
            }
        }
        else if (handleToken() == "return") {
            next();
            if (handleToken() == "INTC" || handleToken() == "DECI" || handleToken() == "id" || handleToken() == "(")
                Exp();
            if (handleToken() == ";") {
                next();
            }
            else {
                throw "Error: Expected ; , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";
            }
        }
        else if (handleToken() == "while") {
            next();
            if (handleToken() == "(") {
                next();
                ConditionalExp();
                if (handleToken() == ")") {
                    next();
                    Stmt();
                }
                else {
                    throw "Error: Expected ) , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";
                }
            }
            else {
                throw "Error: Expected ( , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";

            }
        }
        else if (handleToken() == "id") {
            next();
            if (handleToken() == "=") {
                next();
                Exp();
                if (handleToken() == ";") {
                    next();
                }
                else {
                    throw "Error: Expected ; , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";
                }
            }
            else if (handleToken() == "(") {
                next();
                ArgParamList();
                if (handleToken() == ")") {
                    next();
                    if (handleToken() == ";")
                        next();
                    else {
                        throw "Error: Expected ; , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";
                    }
                }
                else {
                    throw "Error: Expected ) , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";
                }
            }
            else {
                throw "Error: Expected = or ( , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";

            }
        }
        else if (handleToken() == "{") {
            CompSt();
        }
        else if (handleToken() == "TYPE") {
            next();
            if (handleToken() == "id") {
                next();
                while (handleToken() == ",") {
                    next();
                    if (handleToken() == "id") {
                        next();
                    }
                    else {
                        throw "Error: Expected id , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";
                    }
                }
                if (handleToken() == ";") {
                    next();
                }
                else {
                    throw "Error: Expected ; , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";
                }
            }
            else {
                throw "Error: Expected id , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";
            }

        }
        else {
            throw "Error: Expected Stmt , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";
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
                throw "Error: Expected } , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";
            }
        }
        else {
            throw "Error: Expected { , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";
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
                next();
                return;
            }
            else {
                throw "Error: Expected ( or , or ; , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";
            }
        }
        else {
            throw "Error: Expected id , but find " + handleToken() + " at " + std::to_string(TL.tok_lis[i].line) + ":" + std::to_string(TL.tok_lis[i].column) + "\n";
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
        ExtDefList();
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
            if (i == TL.tok_lis.size()) {
                std::cout << "YES";
                return;
            }
            else {
                std::cout << "NO";
            }
        }
        catch (const std::string e) {
            std::cout << "NO\n";
            std::cerr << "Caught Error: " << e << "\n";
        }
    }
};
#endif  // PARSER_H