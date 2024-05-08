

// .h
#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include "../Common/compiler.h"
#include "../Common/error.h"
#include "../Common/token.h"
#include "../Common/ast.h"

class TopParser : public Module {
private:
    int i = 0;
    int maxi = 0;
    TokenList& TL;
    ATree& AST;
    Node cur;
    std::stack<std::string> g;
    std::string handleToken() {
        std::pair<std::string , std::string> token = TL.getTokenType(i);
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
    bool isToken(std::string s) {
        if (s == "TYPE" || s == "CmpOp" || s == "ExtDefRest" || s == "VarListRest" || s == "VarList" || s == "FunDec" || s == "Factor" || s == "Term" || s == "Exp" || s == "CompExp" || s == "RelationExp" || s == "ConditionalExp" || s == "ConditionalStmt" || s == "ActParamList" || s == "CallStmt" || s == "LoopStmt" || s == "AssignmentStmt" || s == "ReturnStmt" || s == "BreakStmt" || s == "LocalVariableDeclaration" || s == "Stmt" || s == "StmtList" || s == "CompSt" || s == "ExtDef" || s == "ExtDefList" || s == "Program") {
            return false;
        }
        return true;
    }
    void next() {
        maxi = std::max(i , maxi);
        if (i == TL.tok_lis.size() - 1) {
            // std::cerr << "reach the end of file!\n";
        }
        else {
            g.push(TL.getTokenType(i).second);
            //std::cout << "try handle: " << handleToken() << "\n";
            i++;
        }
    }
    void back() {
        std::cerr << "FailHandle: " << g.top() << "\n";
        g.pop();
        i--;
        // std::cerr << "Fail handle: " << handleToken() << "\n";
    }
    bool True() {
        if (g.empty()) {
            throw "Empty stack!";
        }
        while (!g.empty() && isToken(g.top())) {
            std::cout << "Complete: " << g.top() << "\n";
            g.pop();
        }
        std::cout << "handle: " << g.top() << "\n";
        g.pop();
        return true;
    }
    bool False() {
        if (g.empty()) {
            throw "Empty stack!";
        }
        g.pop();
        return false;
    }

public:
    TopParser(TokenList& TL , ATree& AST) : TL(TL) , AST(AST) {
        cur = AST.root;
        this->inputFile = "../Data/lexer_token.out";
        this->outputFile = "../Data/parser_list.out";
        this->is = std::ifstream(inputFile);
        this->os = std::ofstream(outputFile);
    }
    bool TYPE() {
        // std::cout << "TYPE --> ";

        g.push("TYPE");
        if (handleToken() == "int" || handleToken() == "float" ||
            handleToken() == "string") {
            return True();
        }
        return False();
    }
    bool CmpOp() {
        g.push("CmpOp");
        if (handleToken() == "==" || handleToken() == "!=" ||
            handleToken() == "<" || handleToken() == ">" ||
            handleToken() == "<=" || handleToken() == ">=") {
            return True();
        }
        return False();
    }
    bool ExtDefRest() {
        g.push("ExtDefRest");

        if (handleToken() == ",") {
            next();
            if (handleToken() == "id") {
                next();
                if (ExtDefRest())
                    return True();
                back();
            }
            back();
        }
        return True();
    }
    bool VarListRest() {
        g.push("VarListRest");

        if (handleToken() == ",") {
            next();
            if (TYPE()) {
                next();
                if (handleToken() == "id") {
                    next();
                    if (VarListRest())
                        return True();
                    back();
                }
                back();
            }
            back();
        }
        return True();
    }
    bool VarList() {
        g.push("VarList");
        if (TYPE()) {
            next();
            if (handleToken() == "id") {
                next();
                if (VarListRest())
                    return True();
                back();
            }
            back();
        }
        return True();
    }
    bool FunDec() {
        g.push("FunDec");

        if (handleToken() == "(") {
            next();
            if (VarList())
                if (handleToken() == ")") {
                    next();
                    return True();
                }
            back();
        }
        return False();
    }

    bool Factor() {
        g.push("Factor");

        if (handleToken() == "id" || handleToken() == "INTC" ||
            handleToken() == "DECI") {
            next();
            return True();
        }
        else {
            if (handleToken() == "(") {
                next();
                if (Exp())
                    if (handleToken() == ")") {
                        next();
                        return True();
                    }
                back();
            }
        }
        return False();
    }
    bool Term() {
        g.push("Term");
        if (Factor()) {
            while (handleToken() == "*" || handleToken() == "/") {
                next();
                if (!Factor()) {
                    back();
                    break;
                }
            }
            return True();
        }
        return False();
    }
    bool Exp() {
        g.push("Exp");
        if (Term()) {
            while (handleToken() == "+" || handleToken() == "-") {
                next();
                if (!Term()) {
                    back();
                    break;
                }
            }
            return True();
        }
        return False();
    }
    bool CompExp() {
        g.push("CompExp");
        if (Exp()) {
            if (CmpOp()) {
                next();
                if (Exp())
                    return True();
                back();
            }
        }
        return False();
    }
    bool RelationExp() {
        g.push("RelationExp");
        if (CompExp()) {
            while (handleToken() == "and") {
                next();
                if (!RelationExp()) {
                    back();
                    break;
                }
            }
            return True();
        }
        return False();
    }
    bool ConditionalExp() {
        g.push("ConditionalExp");
        if (RelationExp()) {
            while (handleToken() == "or") {
                next();
                if (!RelationExp()) {
                    back();
                    break;
                }
            }
            return True();
        }
        return False();
    }
    bool ConditionalStmt() {
        g.push("ConditionalStmt");
        if (handleToken() == "if") {
            next();
            if (handleToken() == "(") {
                next();
                if (ConditionalExp())
                    if (handleToken() == ")") {
                        next();
                        if (Stmt()) {
                            if (handleToken() == "else") {
                                next();
                                if (Stmt())
                                    return True();
                                back();
                            }
                            return True();
                        }
                        back();
                    }
                back();
            }
            back();
        }
        return False();
    }
    bool ActParamList() {
        g.push("ActParamList");
        if (Exp()) {
            while (handleToken() == ",") {
                next();
                if (!Exp()) {
                    back();
                    break;
                }
            }
            return True();
        }
        return True();
    }
    bool CallStmt() {
        g.push("CallStmt");
        if (handleToken() == "id") {
            next();
            if (handleToken() == "(") {
                next();
                if (ActParamList()) {
                    if (handleToken() == ")") {
                        next();
                        if (handleToken() == ";") {
                            next();
                            return True();
                        }
                        back();
                    }
                }
                back();
            }
            back();
        }
        return False();
    }
    bool LoopStmt() {
        g.push("LoopStmt");
        if (handleToken() == "while") {
            next();
            if (handleToken() == "(") {
                next();
                if (ConditionalExp()) {
                    if (handleToken() == ")") {
                        next();
                        if (Stmt()) {
                            return True();
                        }
                        back();
                    }
                }
                back();
            }
            back();
        }
        return False();
    }
    bool AssignmentStmt() {
        g.push("AssignmentStmt");

        if (handleToken() == "id") {
            next();
            if (handleToken() == "=") {
                next();
                if (Exp())
                    if (handleToken() == ";") {
                        next();
                        return True();
                    }
                back();
            }
            back();
        }
        return False();
    }
    bool ReturnStmt() {
        g.push("ReturnStmt");

        if (handleToken() == "return") {
            next();
            if (Exp())
                if (handleToken() == ";") {
                    next();
                    return True();
                }
            return True();
        }
        return False();
    }
    bool BreakStmt() {
        g.push("BreakStmt");

        if (handleToken() == "break") {
            next();
            if (handleToken() == ";") {
                next();
                return True();
            }
            back();
        }
        return False();
    }
    bool LocalVariableDeclaration() {
        g.push("LocalVariableDeclaration");

        if (TYPE()) {
            next();
            if (handleToken() == "id") {
                next();
                while (handleToken() == ",") {
                    next();
                    if (handleToken() == "id") {
                    }
                    else {
                        back();
                        break;
                    }
                }
                if (handleToken() == ";") {
                    next();
                    return True();
                }
                back();
            }
            back();
        }
        return False();
    }
    bool Stmt() {
        g.push("Stmt");

        if (handleToken() == ";") {
            next();
            return True();
        }
        if (ConditionalStmt() || LoopStmt() || CallStmt() ||
            AssignmentStmt() || ReturnStmt() || BreakStmt() ||
            LocalVariableDeclaration() || CompSt()) {
            return True();
        }
        return False();
    }
    bool StmtList() {
        g.push("StmtList");

        if (Stmt()) {
            if (StmtList())
                return True();
        }
        return True();
    }
    bool CompSt() {
        g.push("CompSt");

        if (handleToken() == "{") {
            next();
            if (StmtList()) {
                if (handleToken() == "}") {
                    next();
                    return True();
                }
            }
            back();
        }
        return False();
    }

    bool ExtDef() {
        g.push("ExtDef");

        // std::cout << "ExtDef --> ";
        if (TYPE()) {
            next();
            if (handleToken() == "id") {
                next();
                if (ExtDefRest())
                    if (handleToken() == ";") {
                        next();
                        return True();
                    }
                if (FunDec())
                    if (CompSt())
                        return True();
                back();
            }
            back();
        }
        return False();
    }
    bool ExtDefList() {
        g.push("ExtDefList");
        // std::cout << "ExtDefList --> ";
        if (ExtDef()) {
            if (ExtDefList())
                return True();
            else {
                return False();
            }
        }
        return True();
    }
    bool Program() {
        g.push("Program");
        // std::cout << "Program --> ";
        if (ExtDefList()) {
            return True();
        }
        return False();
    }
    void lparse() {
        try {
            std::cout << __PRETTY_FUNCTION__ << "\n";
            if (Program() && i == TL.tok_lis.size() - 1) {
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