#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

enum Token {
    //文件结束符 eof
    tok_eof = 1 ,

    //标识符 identifier
    tok_identifier = 2 ,
    //常量 constant
    tok_con = 3 ,
    //保留字 reserved word
    tok_rw = 4 ,
    //运算符 operator
    tok_op = 5 ,
    //界符 delimiter
    tok_del = 6 ,
    //注释 comment
    tok_comment = 7 ,
};

const std::string reservedWords[] = {
    "program", "var", "integer", "bool", "real", "const", "begin", "if", "then", "else", "while", "do", "repeat", "until", "for", "to"
};
const std::string operators[] = {
    "and" , "or" , "not",
    "+" , "-" , "*" , "/" , "<" , ">" , "<=" , ">=" , "=" , ":="
};

int handle_alpha(std::string word) {
    std::transform(word.begin() , word.end() , word.begin() , ::tolower);
    for (int i = 0; i < sizeof(reservedWords) / sizeof(reservedWords[0]); i++) {
        if (word == reservedWords[i]) {
            return tok_rw;
        }
    }
    return tok_identifier;
}

int handle_operator(std::string word) {
    for (int i = 0; i < sizeof(operators) / sizeof(operators[0]); i++) {
        if (word == operators[i]) {
            return tok_op;
        }
    }
    return 0;
}

int sort(std::string word) {
    if (isdigit(word[0])) {
        return tok_con;
    }
    else if (isalpha(word[0])) {
        return handle_alpha(word);
    }
    else if (word[0] == '/') {
        if (word.size() == 1)
            return tok_op;
        return tok_comment;
    }
    else if (word[0] == '\'' || word[0] == '\"') {
        return tok_con;
    }
    else if (word[0] == '\\') {
        return tok_con;
    }
    else {
        return handle_operator(word);
    }
}

int Scanner() {
    //打开输入源文件，设置行计数器为 0;
    std::string inputFile = "input.txt";
    std::string outputFile = "output.txt";

    std::ifstream is = std::ifstream(inputFile);
    std::ofstream os = std::ofstream(outputFile);
    int lineCounter = 0;


    std::string line;

    while (is.peek() != EOF) {
        std::getline(is , line);
        std::cout << line << "\n";
        //如果源文件没有结束，读入一行到 string，行计数+1，设置列计数器为 0
        lineCounter++;
        int columnCounter = 0;
        if (!line.empty()) {
            std::string word = "";
            //std::cout << word << "\n";
            for (char c : line) {
                if (c == '(' || c == ')' || c == ',' | c == ';') {
                    os << word << " " << sort(word) << "\n";
                    word = "";
                    os << c << " " << tok_del << "\n";
                }
                //
                else if (c == ' ' || c == '\n' || c == '\t') {
                    os << word << " " << sort(word) << "\n";
                    word = "";
                    continue;
                }
                else {
                    word += c;
                    columnCounter++;
                }
            }
        }
        else {
            continue;
        }
        //std::cout << lineCounter << "\n";
        //output << line << std::endl;
    }


    return 1;
}

int main() {
    Scanner();
    return 0;
}