#include "grammar.h"
#include <bits/stdc++.h>

int main()
{
    Grammar G;
    std::cout << "______NULLABLE______" << std::endl;
    for (auto i : G.NULLABLE)
        std::cout << i << std::endl;
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
    std::cout << "-------FIRSTs:-------" << std::endl;
    for (int i = 0; i < G.P.size(); i++) {
        std::cout << i << ": ";
        for (auto& key : G.first_s[i]) {
            std::cout << key << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "------Follow:-------" << std::endl;
    for (auto& [key, value] : G.follow) {
        std::cout << key << ": ";
        for (auto i : value)
            std::cout << i << " ";
        std::cout << std::endl;
    }
    std::cout << "------Productions:------" << std::endl;
    for (int i = 0; i < G.P.size(); i++) {
        std::cout << i << ":" << G.P[i].first << " -> ";
        for (auto k : G.P[i].second)
            std::cout << k << " ";
        std::cout << std::endl;
        std::cout << "First: ";
        for (auto k : G.first_s[i])
            std::cout << k << " ";
        std::cout << std::endl;
    }
    std::cout << "------Table:------" << std::endl;
    for (auto& j : G.T) {
        std::cout << j << "|";
    }
    std::cout << std::endl;
    for (auto& i : G.NT) {
        std::cout << i << ":";
        for (auto& j : G.T) {
            if (G.table[{i, j}].size() > 1)
                std::cout << "Error!!!! ";
            for (auto k : G.table[{i, j}]) {
                std::cout << j << ":" << k << ", ";
            }
            // std::cout << "|";
        }
        std::cout << "\n";
    }
}