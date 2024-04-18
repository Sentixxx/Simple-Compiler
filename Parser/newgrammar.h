// newgrammar.h
// grammar.h
#ifndef GRAMMAR_H
#define GRAMMAR_H
#include <algorithm>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>

#include <iostream>
#include <vector>
class Grammar {
  private:
    std::string BEGIN;

  public:
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> list;
    std::set<std::string>                                                  T;
    std::set<std::string>                                                  NT;
    std::vector<std::pair<std::string, std::vector<std::string>>>          P;
    std::set<std::string>                                           NULLABLE;
    std::unordered_map<std::string, std::set<std::string>>          first;
    std::unordered_map<int, std::set<std::string>>                  select;
    std::unordered_map<std::string, std::set<std::string>>          follow;
    std::map<std::pair<std::string, std::string>, std::vector<int>> table;
    Grammar()
    {
        init();
    }
    Grammar(std::string BEGIN)
    {
        this->BEGIN = BEGIN;
        init();
    }
    void init()
    {
        // testGrammar();
        initGrammar();
        initTerminals();
        initNonTerminals();
        initP();
        initFirst();
        initFollow();
        initSelect();
        initTable();
    }
    void testGrammar()
    {
        // list["S"]  = {std::vector<std::string>{"a", "F", "A"},
        //               std::vector<std::string>{"+", "a", "F", "A"}};
        // list["A"]  = {std::vector<std::string>{"+", "AP"},
        //               std::vector<std::string>{"EPSILON"}};
        // list["AP"] = {std::vector<std::string>{"a", "F", "A"},
        //               std::vector<std::string>{"B"}};
        // list["F"]  = {
        //     std::vector<std::string>{"*", "a", "B"},
        // };
        // list["B"] = {std::vector<std::string>{"F"},
        //              std::vector<std::string>{"EPSILON"}};
        this->BEGIN = "S";
        list["S"]   = {std::vector<std::string>{"A", "B"},
                       std::vector<std::string>{"b", "C"}};
        list["A"]   = {std::vector<std::string>{"b"},
                       std::vector<std::string>{"EPSILON"}};
        // list["F"] = {
        //     std::vector<std::string>{"*", "a", "B"},
        // };
        list["B"] = {std::vector<std::string>{"a", "D"},
                     std::vector<std::string>{"EPSILON"}};
        list["C"] = {std::vector<std::string>{"b"},
                     std::vector<std::string>{"A", "D"}};
        list["D"] = {std::vector<std::string>{"a", "S"},
                     std::vector<std::string>{"c"}};
    }
    bool merge(std::set<std::string>& a, std::set<std::string>& b)
    {
        int presize = a.size();
        std::set_union(a.begin(), a.end(), b.begin(), b.end(),
                       std::inserter(a, a.begin()));
        int aftersize = a.size();
        return (presize != aftersize);
    }
    bool merge(std::set<std::string>& a, const std::string& b)
    {
        int presize = a.size();
        a.insert(b);
        int aftersize = a.size();
        return (presize != aftersize);
    }
    void initGrammar()
    {
        this->BEGIN = "Program";
        /***程序：由0个或多个全局变量或函数组成***/
        list["Program"]    = {std::vector<std::string>{"ExtDefList"}};
        list["ExtDefList"] = {std::vector<std::string>{"ExtDef", "ExtDefList"},
                              std::vector<std::string>{"EPSILON"}};
        list["ExtDef"]  = {std::vector<std::string>{"TYPE", "id", "ExtDefP"}};
        list["ExtDefP"] = {std::vector<std::string>{"ExtDefRest", ";"},
                           std::vector<std::string>{"FunDec", "CompSt"}};
        list["ExtDefRest"] = {std::vector<std::string>{",", "id", "ExtDefRest"},
                              std::vector<std::string>{"EPSILON"}};
        // 实验一加入了string类
        list["TYPE"] = {std::vector<std::string>{"int"},
                        std::vector<std::string>{"float"},
                        std::vector<std::string>{"string"}};
        /***函数头定义***/
        list["FunDec"]  = {std::vector<std::string>{"(", "VarList", ")"}};
        list["VarList"] = {
            std::vector<std::string>{"TYPE", "id", "VarListRest"},
            std::vector<std::string>{"EPSILON"}};
        list["VarListRest"] = {
            std::vector<std::string>{",", "TYPE", "id", "VarListRest"},
            std::vector<std::string>{"EPSILON"}};
        /*** CompSt：复合语句块***/
        list["CompSt"]   = {std::vector<std::string>{"{", "StmtList", "}"}};
        list["StmtList"] = {std::vector<std::string>{"Stmt", "StmtList"},
                            std::vector<std::string>{"EPSILON"}};
        /*** Stmt：语句***/
        list["Stmt"] = {std::vector<std::string>{"ConditionalStmt"},
                        std::vector<std::string>{"LoopStmt"},
                        std::vector<std::string>{"idStmt"},
                        std::vector<std::string>{"ReturnStmt"},
                        std::vector<std::string>{"BreakStmt"},
                        std::vector<std::string>{"LocalVariableDeclaration"},
                        std::vector<std::string>{"CompSt"},
                        std::vector<std::string>{";"}};
        /***局部变量定义语句***/
        list["LocalVariableDeclaration"]     = {std::vector<std::string>{
            "TYPE", "id", "LocalVariableDeclarationRest", ";"}};
        list["LocalVariableDeclarationRest"] = {
            std::vector<std::string>{",", "id", "LocalVariableDeclarationRest"},
            std::vector<std::string>{"EPSILON"}};
        /***函数调用语句***/
        list["idStmt"]     = {std::vector<std::string>{"id", "idStmtRest"}};
        list["idStmtRest"] = {std::vector<std::string>{"CallStmt"},
                              std::vector<std::string>{"AssignmentStmt"}};
        list["CallStmt"] = {std::vector<std::string>{"(", "ArgList", ")", ";"}};
        /***实参列表***/
        list["ArgList"]     = {std::vector<std::string>{"Exp", "ArgListRest"},
                               std::vector<std::string>{"EPSILON"}};
        list["ArgListRest"] = {
            std::vector<std::string>{",", "Exp", "ArgListRest"},
            std::vector<std::string>{"EPSILON"}};
        /***赋值语句***/
        list["AssignmentStmt"] = {std::vector<std::string>{"=", "Exp", ";"}};
        /***条件语句***/
        list["ConditionalStmt"]     = {std::vector<std::string>{
            "if", "(", "ConditionalExp", ")", "Stmt", "ConditionalStmtRest"}};
        list["ConditionalStmtRest"] = {std::vector<std::string>{"else", "Stmt"},
                                       std::vector<std::string>{"EPSILON"}};
        /***循环语句***/
        list["LoopStmt"] = {std::vector<std::string>{
            "while", "(", "ConditionalExp", ")", "Stmt"}};
        /***返回语句***/
        list["ReturnStmt"] = {
            std::vector<std::string>{"return", "ReturnStmtRest", ";"}};
        list["ReturnStmtRest"] = {std::vector<std::string>{"Exp"},
                                  std::vector<std::string>{"EPSILON"}};
        /***中止语句***/
        list["BreakStmt"] = {std::vector<std::string>{"break", ";"}};

        /***四则表达式***/
        list["Exp"]      = {std::vector<std::string>{"Term", "ExpRest"}};
        list["ExpRest"]  = {std::vector<std::string>{"+", "Term", "ExpRest"},
                            std::vector<std::string>{"-", "Term", "ExpRest"},
                            std::vector<std::string>{"EPSILON"}};
        list["Term"]     = {std::vector<std::string>{"Factor", "TermRest"}};
        list["TermRest"] = {std::vector<std::string>{"*", "Factor", "TermRest"},
                            std::vector<std::string>{"/", "Factor", "TermRest"},
                            std::vector<std::string>{"EPSILON"}};
        list["Factor"]   = {
            std::vector<std::string>{"id"}, std::vector<std::string>{"INTC"},
            std::vector<std::string>{"DECI"}, std::vector<std::string>{"str"},
            std::vector<std::string>{"(", "Exp", ")"}};
        // add str to match string
        /***条件表达式***/
        list["ConditionalExp"] = {
            std::vector<std::string>{"RelationExp", "ConditionalExpRest"}};
        list["ConditionalExpRest"] = {
            std::vector<std::string>{"or", "RelationExp", "ConditionalExpRest"},
            std::vector<std::string>{"EPSILON"}};
        list["RelationExp"] = {
            std::vector<std::string>{"CompExp", "RelationExpRest"}};
        list["RelationExpRest"] = {
            std::vector<std::string>{"and", "CompExp", "RelationExpRest"},
            std::vector<std::string>{"EPSILON"}};
        list["CompExp"] = {std::vector<std::string>{"Exp", "CmpOp", "Exp"}};
        list["CmpOp"]   = {
            std::vector<std::string>{"<"}, std::vector<std::string>{"<="},
            std::vector<std::string>{">"}, std::vector<std::string>{">="},
            std::vector<std::string>{"=="},
            // 实验一将<>修改为了!=
            std::vector<std::string>{"!="}};
    }
    bool hasLeftRecursion()
    {
        for (auto& [key, value] : list) {
            for (auto& v : value) {
                if (v[0] == key) {
                    return true;
                }
            }
        }
        return false;
    }
    void initTerminals()
    {
        for (auto& [key, value] : list) {
            for (auto& production : value) {
                for (auto& symbol : production) {
                    if (symbol != "EPSILON" &&
                        list.find(symbol) == list.end()) {
                        T.insert(symbol);
                    }
                }
            }
        }
    }
    void initNonTerminals()
    {
        for (auto& [key, value] : list) {
            if (T.find(key) == T.end())
                NT.insert(key);
        }
    }
    bool isTerminal(const std::string& symbol)
    {
        return T.find(symbol) != T.end();
    }
    bool isNonTerminal(const std::string& symbol)
    {
        return NT.find(symbol) != NT.end();
    }
    bool has(const std::set<std::string>& S, const std::string& symbol)
    {
        return S.find(symbol) != S.end();
    }
    bool hasEPSILON(const std::set<std::string>& S)
    {
        return has(S, "EPSILON");
    }
    bool mergeExceptEPSILON(std::set<std::string>& A, std::set<std::string>& B)
    {
        bool flag = false;
        if (B.find("EPSILON") != B.end()) {
            B.erase("EPSILON");
            B.erase("EPSILON");
            flag |= merge(A, B);
            B.insert("EPSILON");
        }
        else {
            flag |= merge(A, B);
        }
        return flag;
    }
    void initP()
    {
        for (auto& [key, value] : list) {
            for (auto& production : value) {
                P.push_back({key, production});
            }
        }
    }

    void initFirst()
    {
        bool flag = true;
        for (auto& t : T) {
            first[t].insert(t);
        }
        while (flag) {
            flag = false;
            for (auto& [key, value] : list) {
                for (auto& production : value) {
                    for (int i = 0; i < production.size(); i++) {
                        if (production[i] == "EPSILON") {
                            flag |= merge(first[key], production[i]);
                            break;
                        }
                        else {
                            if (hasEPSILON(first[production[i]])) {
                                flag |= mergeExceptEPSILON(
                                    first[key], first[production[i]]);
                            }
                            else {
                                flag |= merge(first[key], first[production[i]]);
                                break;
                            }
                            if (i + 1 == production.size()) {
                                flag |= merge(first[key], "EPSILON");
                            }
                        }
                    }
                }
            }
        }
    }
    void initFollow()
    {
        bool flag = true;
        follow[BEGIN].insert("#");
        while (flag) {
            flag = false;
            for (auto& nt : NT) {
                for (int i = 0; i < P.size(); i++) {
                    auto& [left, right] = P[i];
                    for (int j = 0; j < right.size(); j++) {
                        if (nt == right[j]) {
                            if (j + 1 == right.size() ||
                                hasEPSILON(first[right[j + 1]])) {
                                flag |= merge(follow[nt], follow[left]);
                            }
                            else {
                                flag |= mergeExceptEPSILON(follow[nt],
                                                           first[right[j + 1]]);
                            }
                        }
                    }
                }
            }
        }
    }
    void initSelect() {}
    void initTable()
    {
        for (int i = 0; i < P.size(); i++) {
            auto& [left, right] = P[i];
            for (auto& symbol : right) {
                if (isTerminal(symbol))
                    table[{left, symbol}].push_back(i);
                if (hasEPSILON(first[symbol])) {
                    for (auto& follow_symbol : follow[left]) {
                        if (isTerminal(follow_symbol))
                            table[{left, follow_symbol}].push_back(i);
                    }
                    if (has(follow[left], "#"))
                        table[{left, "#"}].push_back(i);
                }
            }
        }
    }
    std::string findProductionL(int i)
    {
        if (i >= P.size()) {
            throw "out of P range!";
        }
        return this->P[i].first;
    }
    std::vector<std::string> findProductionR(int i)
    {
        if (i >= P.size()) {
            throw "out of P range!";
        }
        return this->P[i].second;
    }
};
#endif