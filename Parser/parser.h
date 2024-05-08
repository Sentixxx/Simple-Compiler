

// parse.h
#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include "../Common/compiler.h"
#include "../Common/error.h"
#include "../Common/token.h"

class TopParser : public Module {
   private:
    int i = 0;
    int maxi = 0;
    TokenList& TL;
    std::stack<std::string> g;
    std::string handleToken() {
        std::pair<std::string, std::string> token = TL.getTokenType(i);
        if (token.first == "number")
            return "INTC";
        else if (token.first == "float")
            return "DECI";
        else if (token.first == "identifier") {
            return "id";
        } else {
            return token.second;
        }
    }
    void next() {
        maxi = std::max(i, maxi);
        if (i == TL.tok_lis.size() - 1) {
            // std::cerr << "reach the end of file!\n";
        } else {
            // std::cout << "try handle: " << handleToken() << "\n";
            i++;
        }
    }
    void back() {
        i--;
        // std::cerr << "Fail handle: " << handleToken() << "\n";
    }
    bool True() {
        if (g.empty()) {
            throw "Empty stack!";
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
    TopParser(TokenList& TL) : TL(TL) {
        this->inputFile = "../Data/lexer_token.out";
        this->outputFile = "../Data/parser_list.out";
        this->is = std::ifstream(inputFile);
        this->os = std::ofstream(outputFile);
    }
    bool parseTYPE() {
        // std::cout << "parseTYPE --> ";

        g.push("TYPE");
        if (handleToken() == "int" || handleToken() == "float" ||
            handleToken() == "string") {
            return True();
        }
        return False();
    }
    bool parseCmpOp() {
        g.push("CmpOp");
        if (handleToken() == "==" || handleToken() == "!=" ||
            handleToken() == "<" || handleToken() == ">" ||
            handleToken() == "<=" || handleToken() == ">=") {
            return True();
        }
        return False();
    }
    bool parseExtDefRest() {
        g.push("ExtDefRest");

        if (handleToken() == ",") {
            next();
            if (handleToken() == "id") {
                next();
                if (parseExtDefRest())
                    return True();
                back();
            }
            back();
        }
        return True();
    }
    bool parseVarListRest() {
        g.push("VarListRest");

        if (handleToken() == ",") {
            next();
            if (parseTYPE()) {
                next();
                if (handleToken() == "id") {
                    next();
                    if (parseVarListRest())
                        return True();
                    back();
                }
                back();
            }
            back();
        }
        return True();
    }
    bool parseVarList() {
        g.push("VarList");
        if (parseTYPE()) {
            next();
            if (handleToken() == "id") {
                next();
                if (parseVarListRest())
                    return True();
                back();
            }
            back();
        }
        return True();
    }
    bool parseFunDec() {
        g.push("FunDec");

        if (handleToken() == "(") {
            next();
            if (parseVarList())
                if (handleToken() == ")") {
                    next();
                    return True();
                }
            back();
        }
        return False();
    }

    bool parseFactor() {
        g.push("Factor");

        if (handleToken() == "id" || handleToken() == "INTC" ||
            handleToken() == "DECI") {
            next();
            return True();
        } else {
            if (handleToken() == "(") {
                next();
                if (parseExp())
                    if (handleToken() == ")") {
                        next();
                        return True();
                    }
                back();
            }
        }
        return False();
    }
    bool parseTerm() {
        g.push("Term");
        if (parseFactor()) {
            while (handleToken() == "*" || handleToken() == "/") {
                next();
                if (!parseFactor()) {
                    back();
                    break;
                }
            }
            return True();
        }
        return False();
    }
    bool parseExp() {
        g.push("Exp");
        if (parseTerm()) {
            while (handleToken() == "+" || handleToken() == "-") {
                next();
                if (!parseTerm()) {
                    back();
                    break;
                }
            }
            return True();
        }
        return False();
    }
    bool parseCompExp() {
        g.push("CompExp");
        if (parseExp()) {
            if (parseCmpOp()) {
                next();
                if (parseExp())
                    return True();
                back();
            }
        }
        return False();
    }
    bool parseRelationExp() {
        g.push("RelationExp");
        if (parseCompExp()) {
            while (handleToken() == "and") {
                next();
                if (!parseRelationExp()) {
                    back();
                    break;
                }
            }
            return True();
        }
        return False();
    }
    bool parseConditionalExp() {
        g.push("ConditionalExp");
        if (parseRelationExp()) {
            while (handleToken() == "or") {
                next();
                if (!parseRelationExp()) {
                    back();
                    break;
                }
            }
            return True();
        }
        return False();
    }
    bool parseConditionalStmt() {
        g.push("ConditionalStmt");
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
    bool parseActParamList() {
        g.push("ActParamList");
        if (parseExp()) {
            while (handleToken() == ",") {
                next();
                if (!parseExp()) {
                    back();
                    break;
                }
            }
            return True();
        }
        return True();
    }
    bool parseCallStmt() {
        g.push("CallStmt");
        if (handleToken() == "id") {
            next();
            if (handleToken() == "(") {
                next();
                if (parseActParamList()) {
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
    bool parseLoopStmt() {
        g.push("LoopStmt");
        if (handleToken() == "while") {
            next();
            if (handleToken() == "(") {
                next();
                if (parseConditionalExp()) {
                    if (handleToken() == ")") {
                        next();
                        if (parseStmt()) {
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
    bool parseAssignmentStmt() {
        g.push("AssignmentStmt");

        if (handleToken() == "id") {
            next();
            if (handleToken() == "=") {
                next();
                if (parseExp())
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
    bool parseReturnStmt() {
        g.push("ReturnStmt");

        if (handleToken() == "return") {
            next();
            if (parseExp())
                if (handleToken() == ";") {
                    next();
                    return True();
                }
            return True();
        }
        return False();
    }
    bool parseBreakStmt() {
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
    bool parseLocalVariableDeclaration() {
        g.push("LocalVariableDeclaration");

        if (parseTYPE()) {
            next();
            if (handleToken() == "id") {
                next();
                while (handleToken() == ",") {
                    next();
                    if (handleToken() == "id") {
                    } else {
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
    bool parseStmt() {
        g.push("Stmt");

        if (handleToken() == ";") {
            next();
            return True();
        }
        if (parseConditionalStmt() || parseLoopStmt() || parseCallStmt() ||
            parseAssignmentStmt() || parseReturnStmt() || parseBreakStmt() ||
            parseLocalVariableDeclaration() || parseCompSt()) {
            return True();
        }
        return False();
    }
    bool parseStmtList() {
        g.push("StmtList");

        if (parseStmt()) {
            if (parseStmtList())
                return True();
        }
        return True();
    }
    bool parseCompSt() {
        g.push("CompSt");

        if (handleToken() == "{") {
            next();
            if (parseStmtList()) {
                if (handleToken() == "}") {
                    next();
                    return True();
                }
            }
            back();
        }
        return False();
    }

    bool parseExtDef() {
        g.push("ExtDef");

        // std::cout << "parseExtDef --> ";
        if (parseTYPE()) {
            next();
            if (handleToken() == "id") {
                next();
                if (parseExtDefRest())
                    if (handleToken() == ";") {
                        next();
                        return True();
                    }
                if (parseFunDec())
                    if (parseCompSt())
                        return True();
                back();
            }
            back();
        }
        return False();
    }
    bool parseExtDefList() {
        g.push("ExtDefList");

        // std::cout << "parseExtDefList --> ";
        if (parseExtDef()) {
            if (parseExtDefList())
                return True();
            else {
                return False();
            }
        }
        return True();
    }
    bool parseProgram() {
        g.push("Program");

        // std::cout << "parseProgram --> ";
        if (parseExtDefList()) {
            return True();
        }
        return False();
    }
    void lparse() {
        try {
            if (parseProgram() && i == TL.tok_lis.size() - 1) {
                std::cout << "YES";
                return;
            } else {
                std::cout << "NO\n";
                std::cerr << "Error at " << TL.tok_lis[maxi].line << ":"
                          << TL.tok_lis[maxi].column << "\n";
            }
        } catch (const char* e) {
            std::cerr << "Caught Error: " << e << "\n";
        }
    }
};
#endif  // PARSER_H