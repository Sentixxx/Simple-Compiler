#include "grammar.h"
#include <bits/stdc++.h>

int main()
{
    Grammar G;
    G.findTerminals();
    for (auto i : G.T)
        std::cout << i << std::endl;
    std::cout << "-------------------" << std::endl;
    for (auto i : G.NT)
        std::cout << i << std::endl;
    std::cout << G.hasLeftRecursion();
}