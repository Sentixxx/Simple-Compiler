// token.h
#ifndef TOKEN_H
#    define TOKEN_H
#    include <fstream>
#    include <iomanip>
#    include <iostream>
#    include <string>
#    include <vector>

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
    TokenInfo() : line(0) , column(0) , lexeme("$") , token(Error) {}
    TokenInfo(int line , int column , std::string lexeme , Token token)
        : line(line) , column(column) , lexeme(lexeme) , token(token) {
    }
};

class TokenList {
    bool              read_flag;
    const std::string tok_transformed[11] = {
        "Error",
        "identifier",
        "reserved word",
        "str",
        "number",
        "float",
        "delimiter",
        "basic operator",
        "compare operator",
        "assign operator",
        "comment",
    };

public:
    std::vector<TokenInfo> tok_lis;
    TokenList() : read_flag(true) {
        this->tok_lis.clear();
    };
    std::pair<std::string , std::string> getTokenType(int i) {
        if (i >= this->tok_lis.size()) {
            return { "Out of range!", "Out of range!" };
        }
        return { tok_transformed[this->tok_lis[i].token],
                this->tok_lis[i].lexeme };
    }
    void read(std::ifstream& in) {
        while (in.peek() != EOF) {
            TokenInfo token;
            int       token_type;
            in >> token.line >> token.column >> token.lexeme >> token_type;
            token.token = static_cast<Token>(token_type);
            this->tok_lis.push_back(token);
        }
    }
    void saveToken(int line , int column , std::string lexeme , Token token) {
        if (lexeme != " " && lexeme != ";" && lexeme != "," && lexeme != "{" &&
            lexeme != "(" && lexeme != "}" && lexeme != ")" && lexeme != "+" &&
            lexeme != "-" && lexeme != "*")
            column = column - lexeme.size();
        this->tok_lis.push_back({ line, column, lexeme, token });
    }
    void print(std::ostream& out = std::cout) {
        size_t i = 0;
        for (const auto& token : this->tok_lis) {
            out << token.line << " " << token.column << " " << token.lexeme
                << " " << token.token;
            i++;
            if (i != this->tok_lis.size())
                out << "\n";
        }
    }
    int getMaxLength() {
        int maxLength = 0;
        for (const auto& token : this->tok_lis) {
            int length = token.lexeme.length();
            if (length > maxLength) {
                maxLength = length;
            }
        }
        return maxLength;
    }
    void format_print(std::ostream& out = std::cout) {
        size_t i = 0;
        for (const auto& token : this->tok_lis) {
            if (token.token == Error) {
                out << "Error\n";
            }
            else {
                out << "(" << std::setw(16) << tok_transformed[token.token]
                    << ", " << std::setw(getMaxLength() + 1) << token.lexeme
                    << ", " << std::setw(4) << token.line << ", "
                    << std::setw(4) << token.column << ")";
            }
            i++;
            if (i != this->tok_lis.size())
                out << "\n";
        }
    }
};

#endif
// TOKEN_H