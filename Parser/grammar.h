//grammar.h
#ifndef GRAMMAR_H
#define GRAMMAR_H
#include <unordered_map>
#include <vector>
#include <string>
#define EPSILON 135ru9heiugfha 
class Grammar {
public:
    std::unordered_map <std::string , std::vector<std::vector<std::string>>> list;
    Grammar() {
        /***程序：由0个或多个全局变量或函数组成***/
        list["Program"] = {
            std::vector<std::string> {
                "ExtDefList"
            }
        };
        list["ExtDefList"] = {
            std::vector<std::string> {
                "ExtDef", "ExtDefList"
            } ,
            std::vector < std::string> {
                "EPSILON"
            }
        };
        list["ExtDef"] = { std::vector<std::string> {
            "TYPE" , "id" , "ExtDefRest"
            } ,
        std::vector<std::string> {
            "TYPE" , "FunDec" , "CompSt"
            }
        };
        //实验一加入了string类
        list["TYPE"] = {
            std::vector<std::string> {
            "int" , "float" , "string"
            }
        };
        /***函数头定义***/
        list["FunDec"] = {
            std::vector<std::string> {
                "id" , "(" , "VarList" , ")"
            }
        };
        list["VarList"] = {
            std::vector<std::string> {
                "TYPE" , "id", "VarListRest"
            } ,
            std::vector < std::string> {
                "EPSILON"
            }
        };
        list["VarListRest"] = {
            std::vector < std::string> {
                "," , "TYPE" , "id" , "VarListRest"
                } ,
            std::vector < std::string> {
                "EPSILON"
                }
        };
        /*** CompSt：复合语句块***/
        list["CompSt"] = {
            std::vector < std::string> {
                "{" , "StmtList" , "}"
            },
            std::vector < std::string> {
                "EPSILON"
            }
        };
        list["StmtList"] = {
            std::vector < std::string> {
                "Stmt" , "StmtList"
            },
            std::vector < std::string> {
                "EPSILON"
            }
        };
        /*** Stmt：语句***/
        list["Stmt"] = {
            std::vector < std::string> {
                "ConditionalStmt" , "LoopStmt" , "CallStmt", "AssignmentStmt" , "ReturnStmt" , "BreakStmt" , "LocalVariableDeclaration" , "CompSt" , ";"
            }
        };
        /***局部变量定义语句***/
        list["LocalVariableDeclaration"] = {
            std::vector<std::string> {
                "TYPE" , "id" , "LocalVariableDeclarationRest", ";"
            }
        };
        list["LocalVariableDeclarationRest"] = {
            std::vector<std::string> {
                "," , "id" , "LocalVariableDeclarationRest"
            } ,
            std::vector < std::string> {
                "EPSILON"
            }
        };
        /***函数调用语句***/
        list["CallStmt"] = {
            std::vector<std::string> {
                "id" , "(" , "ArgList" , ")" , ";"
            }
        };
        /***实参列表***/
        list["ArgList"] = {
            std::vector<std::string> {
                "Exp" , "ArgListRest"
            } ,
            std::vector < std::string> {
                "EPSILON"
            }
        };
        list["ArgListRest"] = {
            std::vector<std::string> {
                "," , "Exp" , "ArgListRest"
            } ,
            std::vector < std::string> {
                "EPSILON"
            }
        };
        /***赋值语句***/
        list["AssignmentStmt"] = {
            std::vector<std::string> {
                "id" , "=" , "Expression" , ";"
            }
        };
        /***条件语句***/
        list["ConditionalStmt"] = {
            std::vector<std::string> {
                "if" , "(" , "ConditionalExp" , ")" , "Stmt" , "ConditionalStmtRest"
            }
        };
        list["ConditionalStmtRest"] = {
            std::vector<std::string> {
                "else" , "Stmt"
            } ,
            std::vector < std::string> {
                "EPSILON"
            }
        };
        /***循环语句***/
        list["LoopStmt"] = {
            std::vector<std::string> {
                "while" , "(" , "ConditionalExp" , ")" , "Stmt"
            }
        };
        /***返回语句***/
        list["ReturnStmt"] = {
            std::vector<std::string> {
                "return" , "ReturnStmtRest" , ";"
            }
        };
        list["ReturnStmtRest"] = {
            std::vector<std::string> {
                "Exp"
            },
            std::vector<std::string> {
                "EPSILON"
            }
        };
        /***中止语句***/
        list["BreakStmt"] = {
            std::vector<std::string> {
                "break", ";"
            }
        };

        /***四则表达式***/
        list["Exp"] = {
            std::vector<std::string> {
                "Term" , "ExpRest"
            }
        };
        list["ExpRest"] = {
            std::vector<std::string> {
                "+" , "Term" , "ExpRest"
            },
            std::vector<std::string> {
                "-" , "Term" , "ExpRest"
            },
            std::vector<std::string> {
                "EPSILON"
            }
        };
        list["Term"] = {
            "Factor" , "TermRest"
        };
        list["TermRest"] = {
            std::vector<std::string> {
                "*" , "Factor" , "TermRest"
            },
            std::vector<std::string> {
                "/" , "Factor" , "TermRest"
            },
            std::vector<std::string> {
                "EPSILON"
            }
        };
        list["Factor"] = {
            std::vector<std::string> {
                "id"
            },
            std::vector<std::string> {
                "INTC"
            },
            std::vector<std::string> {
                "DECI"
            },
            std::vector<std::string> {
                "(" , "Exp" , ")"
            }
        };

        /***条件表达式***/
        list["ConditionalExp"] = {
            std::vector<std::string> {
                "RelationExp" , "ConditionalExpRest"
            }
        };
        list["ConditionalExpRest"] = {
            std::vector<std::string> {
                "or" , "RelationExp" , "ConditionExpRest"
            },
            std::vector<std::string> {
                "EPSILON"
            }
        };
        list["RelationExp"] = {
            std::vector<std::string> {
                "CompExp" , "RelationExpRest"
            }
        };
        list["RelationExpRest"] = {
            std::vector<std::string> {
                "and" , "CompExp" , "RelationExpRest"
            },
            std::vector<std::string> {
                "EPSILON"
            }
        };
        list["CompExp"] = {
            std::vector<std::string> {
                "Exp" , "CmpOp" , "Exp"
            }
        };
        list["CmpOp"] = {
            std::vector<std::string> {
                "<"
            },
            std::vector<std::string> {
                "<="
            },
            std::vector<std::string> {
                ">"
            },
            std::vector<std::string> {
                ">="
            },
            std::vector<std::string> {
                "=="
            },
            //实验一将<>修改为了!=
            std::vector<std::string> {
                "!="
            }
        };
    }
};
#endif