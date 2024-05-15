#include "parser.h"

TokenList TL;
ATree AST;
TopParser PR(TL , AST);
int       main() {
    TL.read(PR.getIs());
    PR.lparse();
    std::ofstream node = std::ofstream("./Data/nodes.out");
    std::ofstream tree = std::ofstream("./Data/tree.out");
    PR.outTree(tree);
    PR.outNodes(node);
}