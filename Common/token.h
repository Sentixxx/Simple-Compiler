// token.h
#ifndef TOKEN_H
#define TOKEN_H

#include <string>

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

class TokenInfo {
public:
	int         line;
	int         column;
	std::string lexeme;
	Token       token;
	TokenInfo() : line(0) , column(0) , lexeme("") , token(Error) {}
	TokenInfo(int line , int column , std::string lexeme , Token token) : line(line) , column(column) , lexeme(lexeme) , token(token) {}
};

#endif  // TOKEN_H