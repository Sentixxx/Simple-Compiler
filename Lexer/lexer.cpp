#include <algorithm>
#include <fstream>
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

bool isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

Token sort(std::string word)
{
    // std::cout << word[0];
    if (isdigit(word[0])) {
        return tok_con;
    }
    else if (isAlpha(word[0]) || word[0] == '_') {
        return handleAlpha(word);
    }
    else if (word[0] == '/') {
        if (word.size() == 1)
            return tok_op;
        if (word[1] == '*')
            comment_flag = true;
        return tok_comment;
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

int Scanner()
{
    // 打开输入源文件，设置行计数器为 0;
    std::string inputFile  = "input.txt";
    std::string outputFile = "output.txt";

    std::ifstream is            = std::ifstream(inputFile);
    std::ofstream os            = std::ofstream(outputFile);
    int           lineCounter   = 0;
    int           columnCounter = 0;
    std::string   end_comment   = "";
    char          c;
    std::string   line;
    std::string   word = "";

    auto findToken = [&]() {
        if (comment_flag) {
            // std::cout << 1;
            if (end_comment == "*" && c == '/') {
                std::cout << 2;
                comment_flag = false;
                end_comment  = "";
                saveToken(lineCounter, "*/", tok_comment);
                return;
            }
            end_comment = c;
        }
        else {
            if (c == '(' || c == ')' || c == ',' | c == ';') {
                if (word != "")
                    saveToken(lineCounter, word, sort(word));
                word = "";
                saveToken(lineCounter, std::string(1, c), tok_del);
            }

            else if (c == '+' || c == '-') {
                if (word != "")
                    saveToken(lineCounter, word, sort(word));
                word = "";
                saveToken(lineCounter, std::string(1, c), tok_op);
            }
            else if (c == '=' || c == '<' || c == '>' || c == '*' || c == '/') {
                if (word == "*" || word == ">" || word == "/" || word == "<" ||
                    word == "=") {
                    word += c;
                    saveToken(lineCounter, word, sort(word));
                }
                else {
                    if (word != "")
                        saveToken(lineCounter, word, sort(word));
                }
            }
            //
            else if (c == ' ' || c == '\n' || c == '\t') {
                if (word != "")
                    saveToken(lineCounter, word, sort(word));
                word = "";
                return;
            }
            else {
                word += c;
                columnCounter++;
            }
        }
    };

    while (is.peek() != EOF) {
        std::getline(is, line);
        // std::cout << line << "\n";
        //  如果源文件没有结束，读入一行到 string，行计数+1，设置列计数器为 0
        lineCounter++;
        columnCounter = 0;
        if (!line.empty()) {
            if (comment_flag) {
                for (char x : line) {
                    c = x;
                    findToken();
                }
            }
            else {
                // std::cout << 4;
                word = "";
                // std::cout << word << "\n";
                for (char x : line) {
                    c = x;
                    findToken();
                }
            }
        }
        else {
            continue;
        }
        // std::cout << lineCounter << "\n";
        // output << line << std::endl;
    }

    return 1;
}

int main()
{
    Scanner();

    for (const auto& token : tokens) {
        std::cout << "Line: " << token.line << "  ||  Lexeme:  " << token.lexeme
                  << " ||  Token: " << tok_str[token.token] << "\n";
    }

    return 0;
}