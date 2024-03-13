// lexer.h
#ifndef LEXER_H
#define LEXER_H

#include <fstream>
#include <string>
#include <vector>

enum Token {
    Error = 0 ,
    // 标识符 identifier
    tok_identifier = 1 ,
    // 保留字 reserved word
    tok_rw = 2 ,
    // 字符串常量 string constant
    tok_str = 3 ,
    // 数字常量 number constant
    tok_num = 4 ,
    // 小数常量 float constant
    tok_flt = 5 ,
    // 界符 delimiter
    tok_del = 6 ,
    // 四则运算符 basic operator
    tok_bop = 7 ,
    // 比较运算符 compare operator
    tok_cop = 8 ,
    // 赋值运算符 assign operator
    tok_aop = 9 ,
    // 注释 comment
    tok_comment = 10 ,
};

extern const std::string tok_transformed[];

extern const std::string reservedWords[];
extern const std::string basic_operators[];
extern const std::string compare_operators[];
extern const std::string assign_operators[];

struct TokenInfo {
    int         line;
    int         column;
    std::string lexeme;
    Token       token;
};

extern std::vector<TokenInfo> tokens;
extern bool                   comment_flag;

// Function declarations
char  getchar(std::istream& stream);
Token handleAlpha(std::string& word);
Token handleOperator(std::string& word);
Token handleDigit(std::string& word);
Token sort(std::string& word);

void        saveToken(int                     line ,
    int                     column ,
    std::string             lexeme ,
    Token                   token ,
    std::vector<TokenInfo>& tokenvector);
void        newLine();
std::string convertToVisible(std::string& str);
void        findToken(char& c , std::vector<TokenInfo>& tokens);
int         Scanner(std::vector<TokenInfo>& tokens);

int  getMaxLength();
void print(std::ostream& out);

#endif  // LEXER_H