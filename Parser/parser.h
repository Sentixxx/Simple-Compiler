

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
    int         i    = 0;
    int         maxi = 0;
    TokenList&  TL;
    std::string handleToken()
    {
        std::pair<std::string, std::string> token = TL.getTokenType(i);
        if (token.first == "number")
            return "INTC";
        else if (token.first == "float")
            return "DECI";
        else if (token.first == "identifier") {
            return "id";
        }
        else {
            return token.second;
        }
    }
    void next()
    {
        maxi = std::max(i, maxi);
        if (i == TL.tok_lis.size() - 1) {
            std::cerr << "reach the end of file!\n";
        }
        else {
            std::cout << "try handle: " << handleToken() << "\n";
            i++;
        }
    }
    void back()
    {
        i--;
        std::cerr << "Fail handle: " << handleToken() << "\n";
    }

  public:
    TopParser(TokenList& TL) : TL(TL)
    {
        this->inputFile  = "../Data/lexer_token.out";
        this->outputFile = "../Data/parser_list.out";
        this->is         = std::ifstream(inputFile);
        this->os         = std::ofstream(outputFile);
    }
    bool parseTYPE()
    {
        // std::cout << "parseTYPE --> ";
        if (handleToken() == "int" || handleToken() == "float" ||
            handleToken() == "string") {
            return true;
        }
        return false;
    }
    bool parseCmpOp()
    {
        if (handleToken() == "==" || handleToken() == "!=" ||
            handleToken() == "<" || handleToken() == ">" ||
            handleToken() == "<=" || handleToken() == ">=") {
            return true;
        }
        return false;
    }
    bool parseExtDefRest()
    {
        if (handleToken() == ",") {
            next();
            if (handleToken() == "id") {
                next();
                if (parseExtDefRest())
                    return true;
                back();
            }
            back();
        }
        return true;
    }
    bool parseVarListRest()
    {
        if (handleToken() == ",") {
            next();
            if (parseTYPE()) {
                next();
                if (handleToken() == "id") {
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
            if (handleToken() == "id") {
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
        if (handleToken() == "(") {
            next();
            if (parseVarList())
                if (handleToken() == ")") {
                    next();
                    return true;
                }
            back();
        }
        return false;
    }

    bool parseFactor()
    {
        if (handleToken() == "id" || handleToken() == "INTC" ||
            handleToken() == "DECI") {
            next();
            return true;
        }
        else {
            if (handleToken() == "(") {
                next();
                if (parseExp())
                    if (handleToken() == ")") {
                        next();
                        return true;
                    }
                back();
            }
        }
        return false;
    }
    bool parseTerm()
    {
        if (parseFactor()) {
            while (handleToken() == "*" || handleToken() == "/") {
                next();
                if (!parseFactor()) {
                    back();
                    break;
                }
            }
            return true;
        }
        return false;
    }
    bool parseExpRest()
    {
        return false;
    }
    bool parseExp()
    {
        if (parseTerm()) {
            while (handleToken() == "+" || handleToken() == "-") {
                next();
                if (!parseTerm()) {
                    back();
                    break;
                }
            }
            return true;
        }
        return false;
    }
    bool parseCompExp()
    {
        if (parseExp()) {
            if (parseCmpOp()) {
                next();
                if (parseExp())
                    return true;
                back();
            }
        }
        return false;
    }
    bool parseRelationExp()
    {
        if (parseCompExp()) {
            while (handleToken() == "and") {
                next();
                if (!parseRelationExp()) {
                    back();
                    break;
                }
            }
            return true;
        }
        return false;
    }
    bool parseConditionalExp()
    {
        if (parseRelationExp()) {
            while (handleToken() == "or") {
                next();
                if (!parseRelationExp()) {
                    back();
                    break;
                }
            }
            return true;
        }
        return false;
    }
    bool parseConditionalStmt()
    {
        if (handleToken() == "if") {
            next();
            if (handleToken() == "(") {
                next();
                if (parseConditionalExp())
                    if (handleToken() == ")") {
                        next();
                        if (parseStmt()) {
                            if (handleToken() == "else") {
                                next();
                                if (parseStmt())
                                    return true;
                                back();
                            }
                            return true;
                        }
                        back();
                    }
                back();
            }
            back();
        }
        return false;
    }
    bool parseActParamList()
    {
        if (parseExp()) {
            while (handleToken() == ",") {
                next();
                if (!parseExp()) {
                    back();
                    break;
                }
            }
            return true;
        }
        return true;
    }
    bool parseCallStmt()
    {
        if (handleToken() == "id") {
            next();
            if (handleToken() == "(") {
                next();
                if (parseActParamList()) {
                    if (handleToken() == ")") {
                        next();
                        if (handleToken() == ";") {
                            next();
                            return true;
                        }
                        back();
                    }
                }
                back();
            }
            back();
        }
        return false;
    }
    bool parseLoopStmt()
    {
        if (handleToken() == "while") {
            next();
            if (handleToken() == "(") {
                next();
                if (parseConditionalExp()) {
                    if (handleToken() == ")") {
                        next();
                        if (parseStmt()) {
                            return true;
                        }
                        back();
                    }
                }
                back();
            }
            back();
        }
        return false;
    }
    bool parseAssignmentStmt()
    {
        if (handleToken() == "id") {
            next();
            if (handleToken() == "=") {
                next();
                if (parseExp())
                    if (handleToken() == ";") {
                        next();
                        return true;
                    }
                back();
            }
            back();
        }
        return false;
    }
    bool parseReturnStmt()
    {
        if (handleToken() == "return") {
            next();
            if (parseExp())
                if (handleToken() == ";") {
                    next();
                    return true;
                }
            return true;
        }
        return false;
    }
    bool parseBreakStmt()
    {
        if (handleToken() == "break") {
            next();
            if (handleToken() == ";") {
                next();
                return true;
            }
            back();
        }
        return false;
    }
    bool parseLocalVariableDeclaration()
    {
        if (parseTYPE()) {
            next();
            if (handleToken() == "id") {
                next();
                while (handleToken() == ",") {
                    next();
                    if (handleToken() == "id") {}
                    else {
                        back();
                        break;
                    }
                }
                if (handleToken() == ";") {
                    next();
                    return true;
                }
                back();
            }
            back();
        }
        return false;
    }
    bool parseStmt()
    {
        if (handleToken() == ";") {
            next();
            return true;
        }
        if (parseConditionalStmt() || parseLoopStmt() || parseCallStmt() ||
            parseAssignmentStmt() || parseReturnStmt() || parseBreakStmt() ||
            parseLocalVariableDeclaration() || parseCompSt()) {
            return true;
        }
        return false;
    }
    bool parseStmtList()
    {
        if (parseStmt()) {
            if (parseStmtList())
                return true;
        }
        return true;
    }
    bool parseCompSt()
    {
        if (handleToken() == "{") {
            next();
            if (parseStmtList()) {
                if (handleToken() == "}") {
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
            if (handleToken() == "id") {
                next();
                if (parseExtDefRest())
                    if (handleToken() == ";") {
                        next();
                        return true;
                    }
                if (parseFunDec())
                    if (parseCompSt())
                        return true;
                back();
            }
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
            if (parseProgram() && i == TL.tok_lis.size() - 1) {
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