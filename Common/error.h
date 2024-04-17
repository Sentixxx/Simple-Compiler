//error.h
#ifndef ERROR_H
#define ERROR_H
#include <iostream>
#include <stdexcept>
#include <string>
class ParserError {
private:
    std::string errorMessage;

public:
    ParserError(const std::string& message) : errorMessage(message) {}
    virtual const char* what() const noexcept {
        return errorMessage.c_str();
    }
};

#endif  // ERROR_H
