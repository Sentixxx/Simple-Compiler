

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
    int                                 i    = 0;
    int                                 maxi = 0;
    TokenList&                          TL;
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
        maxi = std::max(i, maxi);
        if (i == TL.tok_lis.size() - 1) {
            std::cerr << "reach the end of file!\n";
        }
        else {
            std::cout << "try handle: " << handleToken().first << "\n";
            i++;
        }
    }
    void back()
    {
        i--;
        std::cerr << "Fail handle: " << handleToken().first << "\n";
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
        if (handleToken().first == "int" || handleToken().first == "float" ||
            handleToken().first == "string") {
            return true;
        }
        return false;
    }
    bool parseCmpOp()
    {
        if (handleToken().first == "==" || handleToken().first == "!=" ||
            handleToken().first == "<" || handleToken().first == ">" ||
            handleToken().first == "<=" || handleToken().first == ">=") {
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
        return true;
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

    bool parseFactor()
    {
        if (handleToken().first == "id" || handleToken().first == "INTC" ||
            handleToken().first == "DECI") {
            next();
            return true;
        }
        else {
            if (handleToken().first == "(") {
                next();
                if (parseExp())
                    if (handleToken().first == ")") {
                        next();
                        return true;
                    }
                back();
            }
        }
        return false;
    }
    bool parseTermRest()
    {
        if (handleToken().first == "*" || handleToken().first == "/") {
            next();
            if (parseFactor())
                return true;
            back();
        }
        return false;
    }
    bool parseTerm()
    {
        if (parseFactor()) {
            while (parseTermRest()) {}
            return true;
        }
        return false;
    }
    bool parseExpRest()
    {
        if (handleToken().first == "+" || handleToken().first == "-") {
            next();
            if (parseTerm())
                return true;
            back();
        }
        return false;
    }
    bool parseExp()
    {
        if (parseTerm()) {
            while (parseExpRest()) {}
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
            if (handleToken().first == "and") {
                next();
                if (parseRelationExp())
                    return true;
                back();
            }
        }
        return false;
    }
    bool parseConditionalExpRest()
    {
        if (handleToken().first == "or") {
            next();
            if (parseRelationExp()) {
                return true;
            }
            back();
        }
        return false;
    }
    bool parseConditionalExp()
    {
        if (parseRelationExp()) {
            while (parseConditionalExpRest()) {}
            return true;
        }
        return false;
    }
    bool parseConditionalStmt()
    {
        if (handleToken().first == "if") {
            next();
            if (handleToken().first == "(") {
                next();
                if (parseConditionalExp())
                    if (handleToken().first == ")") {
                        next();
                        if (parseStmt()) {
                            if (handleToken().first == "else") {
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
            while (handleToken().first == ",") {
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
        if (handleToken().first == "id") {
            next();
            if (handleToken().first == "(") {
                next();
                if (parseActParamList()) {
                    if (handleToken().first == ")") {
                        next();
                        if (handleToken().first == ";") {
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
        if (handleToken().first == "while") {
            next();
            if (handleToken().first == "(") {
                next();
                if (parseConditionalExp()) {
                    if (handleToken().first == ")") {
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
        if (handleToken().first == "id") {
            next();
            if (handleToken().first == "=") {
                next();
                if (parseExp())
                    if (handleToken().first == ";") {
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
        if (handleToken().first == "return") {
            next();
            if (parseExp())
                if (handleToken().first == ";") {
                    next();
                    return true;
                }
            return true;
        }
        return false;
    }
    bool parseBreakStmt()
    {
        if (handleToken().first == "break") {
            next();
            if (handleToken().first == ";") {
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
            if (handleToken().first == "id") {
                next();
                while (handleToken().first == ",") {
                    next();
                    if (handleToken().first == "id") {}
                    else {
                        back();
                        break;
                    }
                }
                if (handleToken().first == ";") {
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
        if (handleToken().first == ";") {
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