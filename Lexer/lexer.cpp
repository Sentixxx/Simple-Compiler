#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

enum Token {
    Error = 0,
    // 文件结束符 eof
    tok_eof = 1,

    // 标识符 identifier
    tok_identifier = 2,
    // 常量 constant
    tok_con = 3,
    // 保留字 reserved word
    tok_rw = 4,
    // 运算符 operator
    tok_op = 5,
    // 界符 delimiter
    tok_del = 6,
    // 注释 comment
    tok_comment = 7,
};

const std::string tok_str[] = {"Error",     "eof",           "identifier",
                               "constant",  "reserved word", "operator",
                               "delimiter", "comment"};

bool comment_flag = false;

const std::string reservedWords[] = {"procedure", "def",  "if",    "else",
                                     "while",     "call", "begin", "end",
                                     "and",       "or"

};
const std::string operators[]     = {"+", "-",  "*",  "/",  "=", "<",
                                     ">", "<=", ">=", "<>", "=="};

std::string   inputFile  = "input.txt";
std::string   outputFile = "output.txt";
std::ifstream is         = std::ifstream(inputFile);
std::ofstream os         = std::ofstream(outputFile);

int lineCounter   = 0;
int columnCounter = 0;

char getchar(std::istream& stream)
{
    char ans = stream.get();
    if (ans != ' ' && ans != '\t')
        columnCounter++;
    return ans;
}
struct TokenInfo
{
    int         line;
    std::string lexeme;
    Token       token;
};

std::vector<TokenInfo> tokens;

Token handleAlpha(std::string word)
{
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    for (int i = 0; i < sizeof(reservedWords) / sizeof(reservedWords[0]); i++) {
        if (word == reservedWords[i]) {
            return tok_rw;
        }
    }
    return tok_identifier;
}

Token handleOperator(std::string word)
{
    for (int i = 0; i < sizeof(operators) / sizeof(operators[0]); i++) {
        if (word == operators[i]) {
            return tok_op;
        }
    }
    return Error;
}

Token sort(std::string& word)
{
    // std::cout << word[0];
    if (isdigit(word[0])) {
        return tok_con;
    }
    else if (isalpha(word[0]) || word[0] == '_') {
        return handleAlpha(word);
    }
    else if (word[0] == '*' || word[0] == '+' || word[0] == '-') {
        return tok_op;
    }
    else if (word[0] == '\'' || word[0] == '\"') {
        return tok_con;
    }
    else if (word[0] == '\\') {
        return tok_con;
    }
    else {
        // std::cout << "Error: Unrecognized token!\n";
        return handleOperator(word);
    }
}

void saveToken(int line, std::string lexeme, Token token)
{
    tokens.push_back({line, lexeme, token});
}

void newLine()
{
    lineCounter++;
    columnCounter = 0;
}

void findToken(char& c)
{
    if (isalpha(c) || c == '_') {
        int         n    = 0;
        std::string word = "";
        while (isdigit(c) || isalpha(c) || c == '_') {
            if (n < 20) {
                word += c;
                n++;
            }
            c = getchar(is);
        }
        saveToken(lineCounter, word, sort(word));
    }
    else {
        if (isdigit(c)) {
            if (c == '0') {
                saveToken(lineCounter, std::string(c, 1), Error);
                c = getchar(is);
            }
            std::string word = "";
            while (isdigit(c)) {
                word += c;
                c = getchar(is);
            }
            saveToken(lineCounter, word, sort(word));
        }
        else {
            if (c == '/' || c == '=' || c == '>' || c == '<') {
                // std::cout << c;
                std::string word = "";
                word += c;
                c = getchar(is);
                // std::cout << c;
                if (word == "/") {
                    if (c == '*') {
                        comment_flag = true;
                        saveToken(lineCounter, "/*", tok_comment);
                        c = getchar(is);
                    }
                    else if (c == '/') {
                        while (c != '\n') {
                            word += c;
                            c = getchar(is);
                        }
                        saveToken(lineCounter, "//", tok_comment);
                        newLine();
                        c = getchar(is);
                    }
                    else {
                        saveToken(lineCounter, word, tok_op);
                        c = getchar(is);
                    }
                }
                else if (word == "<") {
                    if (c == '>' || c == '=') {
                        word += c;
                        saveToken(lineCounter, word, tok_op);
                        c = getchar(is);
                    }
                    else {
                        saveToken(lineCounter, word, tok_op);
                        findToken(c);
                    }
                }
                else if (word == "=") {
                    if (c == '=') {
                        word += c;
                        saveToken(lineCounter, word, tok_op);
                        c = getchar(is);
                    }
                    else {
                        saveToken(lineCounter, word, tok_op);
                        findToken(c);
                    }
                }
                else if (word == ">") {
                    if (c == '=') {
                        word += c;
                        saveToken(lineCounter, word, tok_op);
                        c = getchar(is);
                    }
                    else {
                        saveToken(lineCounter, word, tok_op);
                        findToken(c);
                    }
                }
            }
            else {
                if (c == '\n' || c == ' ' || c == '\t') {
                    c = getchar(is);
                    return;
                }
                else if (c == ')' || c == '(' || c == ';' || c == ',') {
                    std::string word = "";
                    word += c;
                    saveToken(lineCounter, word, tok_del);
                    c = getchar(is);
                }
                else {
                    // std::cout << c << "\n";
                    std::string word = "";
                    word += c;
                    saveToken(lineCounter, word, sort(word));
                    c = getchar(is);
                }
            }
        }
    }
}

int Scanner()
{
    lineCounter = 1;

    std::string end_comment = "";

    char c = getchar(is);

    while (is.peek() != EOF) {
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
                    saveToken(lineCounter, "*/", tok_comment);
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
            findToken(c);
        }
    }

    return 1;
}

void print()
{
    for (const auto& token : tokens) {
        std::cout << "Line: " << token.line << "  ||  Lexeme:  " << token.lexeme
                  << " ||  Token: " << tok_str[token.token] << "\n";
    }
}

void fout()
{
    int maxLength = 0;
    for (const auto& token : tokens) {
        int length = token.lexeme.length();
        if (length > maxLength) {
            maxLength = length;
        }
    }

    for (const auto& token : tokens) {
        os << "Line: " << std::setw(4) << token.line
           << "  ||  Lexeme:  " << std::setw(maxLength) << token.lexeme
           << " ||  Token: " << std::setw(14) << tok_str[token.token] << "\n";
    }
}

int main()
{
    Scanner();
    // print();
    fout();
    return 0;
}