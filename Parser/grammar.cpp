#include "grammar.h"
#include <bits/stdc++.h>

int main()
{
    Grammar G;
    std::cout << "--------T:---------" << std::endl;
    for (auto i : G.T)
        std::cout << i << std::endl;
    std::cout << "--------NT:---------" << std::endl;
    for (auto i : G.NT)
        std::cout << i << std::endl;
    std::cout << "-------FIRST:-------" << std::endl;
    for (auto& [key, value] : G.first) {
        std::cout << key << ": ";
        for (auto i : value)
            std::cout << i << " ";
        std::cout << std::endl;
    }
    std::cout << "------Follow:-------" << std::endl;
    for (auto& [key, value] : G.follow) {
        std::cout << key << ": ";
        for (auto i : value)
            std::cout << i << " ";
        std::cout << std::endl;
    }
    std::cout << G.hasLeftRecursion();
}