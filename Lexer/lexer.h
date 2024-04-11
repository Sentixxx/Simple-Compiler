// lexer.h
#ifndef LEXER_H
#define LEXER_H

#include <fstream>
#include <string>
#include <vector>
#include "../Common/Token.h"



extern const std::string tok_transformed[];

extern const std::string reservedWords[];
extern const std::string basic_operators[];
extern const std::string compare_operators[];
extern const std::string assign_operators[];

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