#include "parser.h"
#include <bits/stdc++.h>

TokenList TL;
Grammar   GM;
TopParser    PR(TL , GM);
int       main() {
    TL.read(PR.getIs());
    // for (int i = 0;i < TL.tok_lis.size();i++) {
    //     std::cout << TL.tok_lis[i].token << "\n";
    // }
    // std::cout << 1;
    // PR.show("Program" , GM , "int");
    PR.lparse();
}