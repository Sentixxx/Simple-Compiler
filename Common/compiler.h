#ifndef COMPILER_H
#define COMPILER_H
#include <fstream>
#include <iostream>
#include <string>
class Module {
  protected:
    std::string   inputFile;
    std::string   outputFile;
    std::ifstream is;
    std::ofstream os;

  public:
    std::ifstream& getIs()
    {
        return this->is;
    }
    std::ofstream& getOs()
    {
        return this->os;
    }
};
#endif