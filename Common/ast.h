// AST.H
#ifndef AST_H
#define AST_H
#include <memory>
#include <string>
#include <variant>
#include <vector>
#include "token.h"
using NodeList = std::vector<std::shared_ptr<Node>>;
using Nodevalue = std::variant<std::string, int, float>;

struct Node {
    std::string type;
    Nodevalue value;
    NodeList son;
};

class AST {
   private:
    Node root;

   public:
    AST() = default;
    ~AST() = default;

    void print();
    void printNode(std::shared_ptr<Node> node, int level);
    void addNode(std::shared_ptr<Node> current_node,
                 std::shared_ptr<Node> node) {
        current_node->son.push_back(node);
    }
    void deleteNode(std::shared_ptr<Node> current_node) {
        current_node->son.erase(current_node->son.begin() +
                                current_node->son.size() - 1);
    }
    void deleteNode(std::shared_ptr<Node> current_node, int index) {
        current_node->son.erase(current_node->son.begin() + index);
    }
    void deleteNode(std::shared_ptr<Node> current_node, Nodevalue value) {
        for (auto it = current_node->son.begin(); it != current_node->son.end();
             it++) {
            if ((*it)->value == value) {
                current_node->son.erase(it);
                break;
            }
        }
    }
    void addNode(std::string type, std::string value) {}
};

#endif  // AST_H