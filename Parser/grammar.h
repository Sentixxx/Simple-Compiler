//grammar.h
#ifdef GRAMMAR_H
#define GRAMMAR_H
#define EPSILON 135ru9heiugfha 
class Grammar {
public:
    std::unordered_map <std::string , std::vector<std::vector<std::string>>> grammar;
    Grammar() {
        /***程序：由0个或多个全局变量或函数组成***/
        grammar["Program"] = {
            std::vector<std::string> {
                "ExtDefList"
            }
        };
        grammar["ExtDefList"] = {
            std::vector<std::string> {
                "ExtDef", "ExtDefList"
            } ,
            std::vector < std::string> {
                "EPSILON"
            }
        };
        grammar["ExtDef"] = { std::vector<std::string> {
            "TYPE" , "id" , "ExtDefRest"
            } ,
        std::vector<std::string> {
            "TYPE" , "FunDec" , "CompSt"
            }
        };
        //实验一加入了string类
        grammar["TYPE"] = {
            std::vector<std::string> {
            "int" , "float" , "string"
            }
        };
        /***函数头定义***/
        grammar["FunDec"] = {
            std::vector<std::string> {
                "id" , "(" , "VarList" , ")"
            }
        };
        grammar["VarList"] = {
            std::vector<std::string> {
                "TYPE" , "id", "VarListRest"
            } ,
            std::vector < std::string> {
                "EPSILON"
            }
        };
        grammar["VarListRest"] = {
            std::vector < std::string> {
                "," , "TYPE" , "id" , "VarListRest"
                } ,
            std::vector < std::string> {
                "EPSILON"
                }
        };
        /*** CompSt：复合语句块***/
        grammar["CompSt"] = {
            std::vector < std::string> {
                "{" , "StmtList" , "}"
            },
            std::vector < std::string> {
                "EPSILON"
            }
        };
        grammar["StmtList"] = {
            std::vector < std::string> {
                "Stmt" , "StmtList"
            },
            std::vector < std::string> {
                "EPSILON"
            }
        };
        /*** Stmt：语句***/
        grammar["Stmt"] = {
            std::vector < std::string> {
                "ConditionalStmt" , "LoopStmt" , "CallStmt", "AssignmentStmt" , "ReturnStmt" , "BreakStmt" , "LocalVariableDeclaration" , "CompSt" , ";"
            }
        };
        /***局部变量定义语句***/
        grammar["LocalVariableDeclaration"] = {
            std::vector<std::string> {
                "TYPE" , "id" , "LocalVariableDeclarationRest", ";"
            }
        };
        grammar["LocalVariableDeclarationRest"] = {
            std::vector<std::string> {
                "," , "id" , "LocalVariableDeclarationRest"
            } ,
            std::vector < std::string> {
                "EPSILON"
            }
        };
        /***函数调用语句***/
        grammar["CallStmt"] = {
            std::vector<std::string> {
                "id" , "(" , "ArgList" , ")" , ";"
            }
        };
        /***实参列表***/
        grammar["ArgList"] = {
            std::vector<std::string> {
                "Exp" , "ArgListRest"
            } ,
            std::vector < std::string> {
                "EPSILON"
            }
        };
        grammar["ArgListRest"] = {
            std::vector<std::string> {
                "," , "Exp" , "ArgListRest"
            } ,
            std::vector < std::string> {
                "EPSILON"
            }
        };
        /***赋值语句***/
        grammar["AssignmentStmt"] = {
            std::vector<std::string> {
                "id" , "=" , "Expression" , ";"
            }
        };
        /***条件语句***/
        grammar["ConditionalStmt"] = {
            std::vector<std::string> {
                "if" , "(" , "ConditionalExp" , ")" , "Stmt" , "ConditionalStmtRest"
            }
        };
        grammar["ConditionalStmtRest"] = {
            std::vector<std::string> {
                "else" , "Stmt"
            } ,
            std::vector < std::string> {
                "EPSILON"
            }
        };
        /***循环语句***/
        grammar["LoopStmt"] = {
            std::vector<std::string> {
                "while" , "(" , "ConditionalExp" , ")" , "Stmt"
            }
        };
        /***返回语句***/
        grammar["ReturnStmt"] = {
            std::vector<std::string> {
                "return" , "ReturnStmtRest" , ";"
            }
        };
        grammar["ReturnStmtRest"] = {
            std::vector<std::string> {
                "Exp"
            },
            std::vector<std::string> {
                "EPSILON"
            }
        };
        /***中止语句***/
        grammar["BreakStmt"] = {
            std::vector<std::string> {
                "break", ";"
            }
        };

        /***四则表达式***/
        grammar["Exp"] = {
            std::vector<std::string> {
                "Term" , "ExpRest"
            }
        };
        grammar["ExpRest"] = {
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
        grammar["Term"] = {
            "Factor" , "TermRest"
        };
        grammar["TermRest"] = {
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
        grammar["Factor"] = {
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
        grammar["ConditionalExp"] = {
            std::vector<std::string> {
                "RelationExp" , "ConditionalExpRest"
            }
        };
        grammar["ConditionalExpRest"] = {
            std::vector<std::string> {
                "or" , "RelationExp" , "ConditionExpRest"
            },
            std::vector<std::string> {
                "EPSILON"
            }
        };
        grammar["RelationExp"] = {
            std::vector<std::string> {
                "CompExp" , "RelationExpRest"
            }
        };
        grammar["RelationExpRest"] = {
            std::vector<std::string> {
                "and" , "CompExp" , "RelationExpRest"
            },
            std::vector<std::string> {
                "EPSILON"
            }
        };
        grammar["CompExp"] = {
            std::vector<std::string> {
                "Exp" , "CmpOp" , "Exp"
            }
        };
        grammar["CmpOp"] = {
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
}

#endif