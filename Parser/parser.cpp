#include "parser.h"

TokenList TL;
ATree AST;
TopParser PR(TL , AST);
int       main() {
    TL.read(PR.getIs());
    PR.lparse();
}