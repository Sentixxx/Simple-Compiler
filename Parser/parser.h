

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
    bool end = false;
    int i = 0;
    int maxi = 0;
    TokenList& TL;
    ATree& AST;
    std::shared_ptr<Node> cur;
    std::stack<std::string> g;
    std::stack<std::string> ans;
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
    void in(std::string name) {
        g.push(name);
        // std::cout << "cur_in : " << cur->value << "\n";
        cur = AST.addNode(cur , name);
        // std::cout << "cur_in : " << cur->value << "\n";
        //std::cout << "in: " << name << "\n";
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
            if (!end) {
                g.push(TL.getTokenType(i).second);
                AST.addLeafNode(cur , TL.tok_lis[i]);
            }
            end = true;
            // std::cerr << "reach the end of file!\n";
        }
        else {
            end = false;
            g.push(TL.getTokenType(i).second);
            AST.addLeafNode(cur , TL.tok_lis[i]);
            i++;
        }
    }
    void back() {
        // std::cerr << "FailHandle: " << g.top() << "\n";
        AST.deleteNode(cur);
        g.pop();
        i--;
    }
    bool True() {
        cur = AST.backTrace(cur);
        return true;
    }
    bool False() {
        if (g.empty()) {
            throw "Empty stack!";
        }
        //std::cout << "out:" << g.top() << "\n";
        g.pop();
        cur = AST.deleteNode(cur);
        return false;
    }
    std::string handleFuncName(std::string name) {
        std::string r = "";
        int i = 0;
        while (i < name.size() && name[i] != ':') {
            i++;
        }
        i += 2;
        while (i < name.size() && name[i] != '(') {
            r += name[i];
            i++;
        }
        return r;
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
        in(handleFuncName(__PRETTY_FUNCTION__));
        if (handleToken() == "int" || handleToken() == "float" ||
            handleToken() == "string") {
            next();
            return True();
        }
        return False();
    }
    bool CmpOp() {
        in(handleFuncName(__PRETTY_FUNCTION__));
        if (handleToken() == "==" || handleToken() == "!=" ||
            handleToken() == "<" || handleToken() == ">" ||
            handleToken() == "<=" || handleToken() == ">=") {
            return True();
        }
        return False();
    }
    bool ExtDefRest() {
        in(handleFuncName(__PRETTY_FUNCTION__));
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

        return False();
    }
    bool VarListRest() {
        in(handleFuncName(__PRETTY_FUNCTION__));
        if (handleToken() == ",") {
            next();
            if (TYPE()) {
                // next();
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
        in(handleFuncName(__PRETTY_FUNCTION__));
        if (TYPE()) {
            // next();
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
        in(handleFuncName(__PRETTY_FUNCTION__));
        if (handleToken() == "id") {
            next();
            if (handleToken() == "(") {
                next();
                if (VarList())
                    if (handleToken() == ")") {
                        next();
                        return True();
                    }
                back();
            }
            back();
        }
        return False();
    }

    bool Factor() {
        in(handleFuncName(__PRETTY_FUNCTION__));

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
        in(handleFuncName(__PRETTY_FUNCTION__));
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
        in(handleFuncName(__PRETTY_FUNCTION__));
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
        in(handleFuncName(__PRETTY_FUNCTION__));
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
        in(handleFuncName(__PRETTY_FUNCTION__));
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
        in(handleFuncName(__PRETTY_FUNCTION__));
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
        in(handleFuncName(__PRETTY_FUNCTION__));
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
        in(handleFuncName(__PRETTY_FUNCTION__));
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
        in(handleFuncName(__PRETTY_FUNCTION__));
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
        in(handleFuncName(__PRETTY_FUNCTION__));
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
        in(handleFuncName(__PRETTY_FUNCTION__));

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
        in(handleFuncName(__PRETTY_FUNCTION__));

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
        in(handleFuncName(__PRETTY_FUNCTION__));

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
        in(handleFuncName(__PRETTY_FUNCTION__));

        if (TYPE()) {
            // next();
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
        in(handleFuncName(__PRETTY_FUNCTION__));
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
        in(handleFuncName(__PRETTY_FUNCTION__));

        if (Stmt()) {
            if (StmtList())
                return True();
        }
        return True();
    }
    bool CompSt() {
        in(handleFuncName(__PRETTY_FUNCTION__));

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
        in(handleFuncName(__PRETTY_FUNCTION__));
        // std::cout << "ExtDef --> ";
        if (TYPE()) {
            // next();
            //cur = AST.backTrace(cur);
            if (FunDec()) {
                if (CompSt()) {
                    return True();
                }
            }
            if (handleToken() == "id") {
                next();
                if (ExtDefRest()) {
                    if (handleToken() == ";") {
                        next();
                        return True();
                    }
                }
                back();
            }

            back();
        }
        return False();
    }
    bool ExtDefList() {
        in(handleFuncName(__PRETTY_FUNCTION__));
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
        in(handleFuncName(__PRETTY_FUNCTION__));
        // std::cout << "Program --> ";
        if (ExtDefList()) {
            return True();
        }
        return False();
    }
    void lparse() {
        try {
            if (Program() && i == TL.tok_lis.size() - 1) {
                std::cout << "YES\n";
                // while (!g.empty()) {
                //     std::cout << g.top() << "\n";
                //     g.pop();
                // }
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
    void outNodes(std::ofstream& out) {
        // while (!g.empty()) {
        //     out << g.top() << "\n";
        //     g.pop();
        // }
        AST.NodeDictGenerate(out);
    }
    void outTree(std::ofstream& out) {
        AST.levelOrderTraversal(out);
    }
};
#endif  // PARSER_H