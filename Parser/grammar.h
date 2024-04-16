// grammar.h
#ifndef GRAMMAR_H
#define GRAMMAR_H
#include <algorithm>
#include <set>
#include <string>
#include <unordered_map>

#include <vector>
#define EPSILON r3315u9heiugfha
class Grammar {
  public:
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> list;
    std::set<std::string>                                                  T;
    std::set<std::string>                                                  NT;
    std::vector<std::pair<std::string, std::vector<std::string>>>          P;
    std::set<std::string>                                  NULLABLE;
    std::unordered_map<std::string, std::set<std::string>> first;
    std::unordered_map<int, std::set<std::string>>         first_s;
    std::unordered_map<std::string, std::set<std::string>> follow;

    Grammar()
    {
        testGrammar();
        // initGrammar();
        initTerminals();
        initNonTerminals();
        initNullable();
        initFirst();
        initFollow();
        initFirstS();
    }
    void testGrammar()
    {
        list["Z"] = {std::vector<std::string>{"d"},
                     std::vector<std::string>{"X", "Y", "Z"}};
        list["Y"] = {std::vector<std::string>{"c"},
                     std::vector<std::string>{"EPSILON"}};
        list["X"] = {std::vector<std::string>{"Y"},
                     std::vector<std::string>{"a"}};
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
        /***程序：由0个或多个全局变量或函数组成***/
        list["Program"]    = {std::vector<std::string>{"ExtDefList"}};
        list["ExtDefList"] = {std::vector<std::string>{"ExtDef", "ExtDefList"},
                              std::vector<std::string>{"EPSILON"}};
        list["ExtDef"] = {std::vector<std::string>{"TYPE", "id", "ExtDefRest"},
                          std::vector<std::string>{"TYPE", "FunDec", "CompSt"}};
        list["ExtDefRest"] = {std::vector<std::string>{",", "id", "ExtDefRest"},
                              std::vector<std::string>{"EPSILON"}};
        // 实验一加入了string类
        list["TYPE"] = {std::vector<std::string>{"int"},
                        std::vector<std::string>{"float"},
                        std::vector<std::string>{"string"}};
        /***函数头定义***/
        list["FunDec"]  = {std::vector<std::string>{"id", "(", "VarList", ")"}};
        list["VarList"] = {
            std::vector<std::string>{"TYPE", "id", "VarListRest"},
            std::vector<std::string>{"EPSILON"}};
        list["VarListRest"] = {
            std::vector<std::string>{",", "TYPE", "id", "VarListRest"},
            std::vector<std::string>{"EPSILON"}};
        /*** CompSt：复合语句块***/
        list["CompSt"]   = {std::vector<std::string>{"{", "StmtList", "}"},
                            std::vector<std::string>{"EPSILON"}};
        list["StmtList"] = {std::vector<std::string>{"Stmt", "StmtList"},
                            std::vector<std::string>{"EPSILON"}};
        /*** Stmt：语句***/
        list["Stmt"] = {std::vector<std::string>{
            "ConditionalStmt", "LoopStmt", "CallStmt", "AssignmentStmt",
            "ReturnStmt", "BreakStmt", "LocalVariableDeclaration", "CompSt",
            ";"}};
        /***局部变量定义语句***/
        list["LocalVariableDeclaration"]     = {std::vector<std::string>{
            "TYPE", "id", "LocalVariableDeclarationRest", ";"}};
        list["LocalVariableDeclarationRest"] = {
            std::vector<std::string>{",", "id", "LocalVariableDeclarationRest"},
            std::vector<std::string>{"EPSILON"}};
        /***函数调用语句***/
        list["CallStmt"] = {
            std::vector<std::string>{"id", "(", "ArgList", ")", ";"}};
        /***实参列表***/
        list["ArgList"]     = {std::vector<std::string>{"Exp", "ArgListRest"},
                               std::vector<std::string>{"EPSILON"}};
        list["ArgListRest"] = {
            std::vector<std::string>{",", "Exp", "ArgListRest"},
            std::vector<std::string>{"EPSILON"}};
        /***赋值语句***/
        list["AssignmentStmt"] = {
            std::vector<std::string>{"id", "=", "Exp", ";"}};
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
        list["Factor"]   = {std::vector<std::string>{"id"},
                            std::vector<std::string>{"INTC"},
                            std::vector<std::string>{"DECI"},
                            std::vector<std::string>{"(", "Exp", ")"}};

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
    void initFirst()
    {
        bool changed = true;
        while (changed) {
            changed = false;
            for (auto& [key, value] : list) {
                if (T.find(key) != T.end())
                    continue;
                for (auto& production : value) {
                    for (auto& symbol : production) {
                        if (T.find(symbol) != T.end()) {
                            if (merge(first[key], symbol)) {
                                changed = true;
                            }
                            break;
                        }
                        if (NT.find(symbol) != NT.end()) {
                            if (merge(first[key], first[symbol])) {
                                changed = true;
                            }
                            if (NULLABLE.find(symbol) == NULLABLE.end()) {
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    void initNullable()
    {
        bool changed = true;
        while (changed) {
            changed = false;
            for (auto& [key, value] : list) {
                if (T.find(key) != T.end())
                    continue;
                for (auto& production : value) {
                    if (production[0] == "EPSILON") {
                        if (merge(NULLABLE, key))
                            changed = true;
                        break;
                    }
                    bool flag = true;
                    for (auto& symbol : production) {
                        if (NULLABLE.find(symbol) == NULLABLE.end()) {
                            flag = false;
                            break;
                        }
                    }
                    if (flag) {
                        if (merge(NULLABLE, key)) {
                            changed = true;
                        }
                    }
                }
            }
        }
    }
    void initFollow()
    {
        bool changed = true;
        while (changed) {
            changed = false;
            for (auto& [key, value] : list) {
                for (auto& production : value) {
                    std::set<std::string> temp = follow[key];
                    for (int i = production.size() - 1; i >= 0; i--) {
                        if (T.find(production[i]) != T.end()) {
                            temp.clear();
                            temp.insert(production[i]);
                        }
                        if (NT.find(production[i]) != NT.end()) {
                            if (merge(follow[production[i]], temp)) {
                                changed = true;
                            }
                            if (NULLABLE.find(production[i]) ==
                                NULLABLE.end()) {
                                temp.clear();
                                temp = first[production[i]];
                            }
                            else {
                                merge(temp, first[production[i]]);
                            }
                        }
                    }
                }
            }
        }
    }
    void initP()
    {
        for (auto& [key, value] : list) {
            for (auto& production : value) {
                P.push_back(std::make_pair(key, production));
            }
        }
    }
    void calculateFirstS(int i)
    {
        for (auto& symbol : P[i].second) {
            if (T.find(symbol) != T.end()) {
                merge(first_s[i], symbol);
                return;
            }
            if (NT.find(symbol) != NT.end()) {
                merge(first_s[i], first[symbol]);
                if (NULLABLE.find(symbol) == NULLABLE.end()) {
                    return;
                }
            }
        }
        merge(first_s[i], follow[P[i].first]);
    }
    void initFirstS()
    {
        initP();
        for (int i = 0; i < P.size(); i++) {
            calculateFirstS(i);
        }
    }
};
#endif