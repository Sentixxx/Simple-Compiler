#include "parser.h"
#include <bits/stdc++.h>

struct Node;

using NodeData = std::variant<std::string, std::vector<Node>>;

struct Node
{
    std::string type;
    NodeData    data;
};

std::vector<Node> node_list;

void initNode()
{
    Node Program = {"Program", std::vector<Node>{{}}};
    std::cout << Program.data.index() << std::endl;
}

Parser    PR;
TokenList TL;
Grammar   GM;
int       main()
{
    TL.read(PR.getIs());
    // for (int i = 0;i < TL.tok_lis.size();i++) {
    //     std::cout << TL.tok_lis[i].token << "\n";
    // }
    // std::cout << 1;
    // PR.show("Program" , GM , "int");
    PR.lparse(GM, TL);
}