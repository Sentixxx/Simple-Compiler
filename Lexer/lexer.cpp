#include "lexer.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

// enum Token {
//     Error = 0 ,
//     // 标识符 identifier
//     tok_identifier = 1 ,
//     // 保留字 reserved word
//     tok_rw = 2 ,
//     // 字符串常量 string constant
//     tok_str = 3 ,
//     // 数字常量 number constant
//     tok_num = 4 ,
//     // 小数常量 float constant
//     tok_flt = 5 ,
//     // 界符 delimiter
//     tok_del = 6 ,
//     // 四则运算符 basic operator
//     tok_bop = 7 ,
//     // 比较运算符 compare operator
//     tok_cop = 8 ,
//     // 赋值运算符 assign operator
//     tok_aop = 9 ,
//     // 注释 comment
//     tok_comment = 10 ,
// };

const std::string tok_transformed[] = {
    "Error",           "identifier",     "reserved word",
    "string",          "number",         "float",
    "delimiter",       "basic operator", "compare operator",
    "assign operator", "comment",
};

bool comment_flag = false;

const std::string reservedWords[] = {
    "int",   "float",  "string", "if",  "else",
    "while", "return", "break",  "and", "or",

};
const std::string basic_operators[] = {
    "+",
    "-",
    "*",
    "/",
};

const std::string compare_operators[] = {
    "<", ">", "<=", ">=", "==", "!=",
};
const std::string assign_operators[] = {
    "=",
};

std::string   inputFile  = "input.txt";
std::string   outputFile = "out.out";
std::ifstream is         = std::ifstream(inputFile);
std::ofstream os         = std::ofstream(outputFile);

int lineCounter   = 0;
int columnCounter = 0;

char getchar(std::istream& stream)
{
    char ans = stream.get();
    columnCounter++;
    return ans;
}

// struct TokenInfo {
//     int         line;
//     int         column;
//     std::string lexeme;
//     Token       token;
// };

std::vector<TokenInfo> tokens;

Token handleAlpha(std::string& word)
{
    if (word.size() > 32) {
        return Error;
    }
    // deprecated 无视大小写
    // std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    for (int i = 0; i < 10; i++) {
        if (word == reservedWords[i]) {
            return tok_rw;
        }
    }
    int num_cnt = 0;
    for (int i = 0; i < word.size(); i++) {
        if (word[i] <= '9' && word[i] >= '0') {
            num_cnt++;
        }
        if (num_cnt > 10) {
            return Error;
        }
    }
    return tok_identifier;
}

Token handleOperator(std::string& word)
{
    for (int i = 0; i < 10; i++) {
        if (word == basic_operators[i]) {
            return tok_bop;
        }
        else if (word == compare_operators[i]) {
            return tok_cop;
        }
        else if (word == assign_operators[i]) {
            return tok_aop;
        }
    }
    return Error;
}

Token handleDigit(std::string& word)
{
    bool flt = false;
    if (word[word.length() - 1] == '.')
        return Error;
    for (char c : word) {
        if (!isdigit(c) && c != '.') {
            return Error;
        }
        if (c == '.') {
            if (flt) {
                return Error;
            }
            flt = true;
        }
    }
    if (word[0] == '0' && word.length() > 1 && word[1] != '.')
        return Error;
    if (!flt) {
        int chk = 0;
        for (char c : word) {
            chk *= 10;
            chk += c - '0';
            if (chk > 65535)
                return Error;
        }
    }
    if (flt)
        return tok_flt;
    else
        return tok_num;
}

Token sort(std::string& word)
{
    // std::cout << word[0];
    if (isdigit(word[0])) {
        return handleDigit(word);
    }
    else if (isalpha(word[0]) || word[0] == '_') {
        return handleAlpha(word);
    }
    else {
        // std::cout << "Error: Unrecognized token!\n";
        return handleOperator(word);
    }
}

void saveToken(int                     line,
               int                     column,
               std::string             lexeme,
               Token                   token,
               std::vector<TokenInfo>& tokenvector = tokens)
{
    if (lexeme != " " && lexeme != ";" && lexeme != "," && lexeme != "{" &&
        lexeme != "(" && lexeme != "}" && lexeme != ")")
        column = column - lexeme.size();
    tokenvector.push_back({line, column, lexeme, token});
}

void newLine()
{
    lineCounter++;
    columnCounter = 0;
}

std::string convertToVisible(std::string& str)
{
    std::string visibleStr = "";
    for (char c : str) {
        switch (c) {
            case '\n': visibleStr += "\\n"; break;
            case '\r': visibleStr += "\\r"; break;
            case '\t': visibleStr += "\\t"; break;
            case '\v': visibleStr += "\\v"; break;
            case '\f': visibleStr += "\\f"; break;
            default: visibleStr += c; break;
        }
    }
    return visibleStr;
}

bool isdelemiter(char c)
{
    if (c == ' ' || c == '\n' || c == '\t' || c == ';' || c == ',' ||
        c == ')' || c == '(' || c == '{' || c == '}' || c == '=' || c == '+' ||
        c == '-' || c == '/' || c == '*' || c == '>' || c == '<' || c == EOF)
        return true;
    return false;
}

void findToken(char& c, std::vector<TokenInfo>& tokens)
{
    if (isalpha(c) || (c == '_')) {
        int         n    = 0;
        std::string word = "";
        while (!isdelemiter(c)) {
            if (n < 32) {
                word += c;
                n++;
            }
            c = getchar(is);
        }
        saveToken(lineCounter, columnCounter, word, sort(word), tokens);
    }
    else {
        if (isdigit(c)) {
            std::string word = "";
            while (!isdelemiter(c)) {
                word += c;
                c = getchar(is);
            }
            saveToken(lineCounter, columnCounter, word, sort(word), tokens);
        }
        else {
            if (c == '/' || c == '=' || c == '>' || c == '<') {
                // std::cout << c;
                std::string word = "";
                word += c;
                c = getchar(is);
                // std::cout << c;
                if (word == "/") {
                    // 跳过注释
                    if (c == '*') {
                        comment_flag = true;
                        // saveToken(lineCounter, columnCounter, "/*",
                        //           tok_comment);
                        c = getchar(is);
                    }
                    else if (c == '/') {
                        while (c != '\n') {
                            word += c;
                            c = getchar(is);
                        }
                        // saveToken(lineCounter, columnCounter, "//",
                        //           tok_comment);
                        newLine();
                        c = getchar(is);
                    }
                    else {
                        saveToken(lineCounter, columnCounter, word, tok_bop,
                                  tokens);
                        c = getchar(is);
                    }
                }
                else if (word == "<") {
                    if (c == '=') {
                        word += c;
                        saveToken(lineCounter, columnCounter, word, tok_cop,
                                  tokens);
                        c = getchar(is);
                    }
                    else {
                        saveToken(lineCounter, columnCounter, word, tok_cop,
                                  tokens);
                        findToken(c, tokens);
                    }
                }
                else if (word == "=") {
                    if (c == '=') {
                        word += c;
                        std::cout << c << "\n";
                        saveToken(lineCounter, columnCounter, word, tok_cop,
                                  tokens);
                        c = getchar(is);
                    }
                    else {
                        saveToken(lineCounter, columnCounter, word, tok_aop,
                                  tokens);
                        findToken(c, tokens);
                    }
                }
                else if (word == ">") {
                    if (c == '=') {
                        word += c;
                        saveToken(lineCounter, columnCounter, word, tok_cop,
                                  tokens);
                        c = getchar(is);
                    }
                    else {
                        saveToken(lineCounter, columnCounter, word, tok_cop,
                                  tokens);
                        findToken(c, tokens);
                    }
                }
            }
            else if (c == '!') {
                std::string word = "";
                word += c;
                c = getchar(is);
                if (c == '=') {
                    word += c;
                    saveToken(lineCounter, columnCounter, word, tok_cop,
                              tokens);
                    c = getchar(is);
                }
                else {
                    saveToken(lineCounter, columnCounter, word, Error, tokens);
                    findToken(c, tokens);
                    std::cout << c << "\n";
                    c = getchar(is);
                }
            }
            else {
                if (c == '\n' || c == ' ' || c == '\t') {
                    c = getchar(is);
                    return;
                }
                else if (c == ')' || c == '(' || c == ';' || c == ',' ||
                         c == '{' || c == '}') {
                    std::string word = "";
                    word += c;
                    saveToken(lineCounter, columnCounter, word, tok_del,
                              tokens);
                    c = getchar(is);
                }
                else if (c == '"') {
                    c                = getchar(is);
                    std::string word = std::string(1, c);
                    int         line = lineCounter, column = columnCounter;
                    while (c != '"' && c != EOF) {
                        word += c;
                        c = getchar(is);
                    }
                    saveToken(line, column + word.length() - 1, word, tok_str,
                              tokens);
                    c = getchar(is);
                }
                else {
                    // std::cout << c << "\n";
                    std::string word = "";
                    word += c;
                    saveToken(lineCounter, columnCounter, word, sort(word),
                              tokens);
                    c = getchar(is);
                }
            }
        }
    }
}

int Scanner(std::vector<TokenInfo>& tokens)
{
    lineCounter = 1;

    char c = getchar(is);

    while (c != EOF) {
        // std::cout << c;
        //  c = getchar(is);
        if (c == '\n') {
            newLine();
        }
        // std::cout << line << "\n";
        //  如果源文件没有结束，读入一行到 string，行计数+1，设置列计数器为
        //  0

        if (comment_flag) {
            // std::cout << c;
            if (c == '*') {
                c = getchar(is);
                if (c == '/') {
                    // std::cout << columnCounter << " " << c << "\n";
                    comment_flag = false;
                    // saveToken(lineCounter, columnCounter, "*/",
                    // tok_comment);
                    c = getchar(is);
                }
            }
            else {
                c = getchar(is);
            }
        }
        else {
            // 空格类字符继续读取下一个字符
            if (c == ' ' || c == '\n' || c == '\t') {
                c = getchar(is);
                continue;
            }
            findToken(c, tokens);
        }
    }

    return 1;
}

int getMaxLength()
{
    int maxLength = 0;
    for (const auto& token : tokens) {
        int length = token.lexeme.length();
        if (length > maxLength) {
            maxLength = length;
        }
    }
    return maxLength;
}

void print(std::ostream& out = std::cout)
{
    for (const auto& token : tokens) {
        if (token.token == Error) {
            out << "Error\n";
        }
        else {
            out << "(" << std::setw(16) << tok_transformed[token.token] << ", "
                << std::setw(getMaxLength() + 1) << token.lexeme << ", "
                << std::setw(4) << token.line << ", " << std::setw(4)
                << token.column << ")"
                << "\n";
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc > 1) {
        inputFile = argv[1];
        is        = std::ifstream(inputFile);
    }
    Scanner(tokens);
    print(os);
    print(std::cout);
    return 0;
}