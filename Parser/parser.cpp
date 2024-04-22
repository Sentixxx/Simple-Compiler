#include "parser.h"
#include <bits/stdc++.h>

TokenList TL;
TopParser PR(TL);
int       main()
{
    TL.read(PR.getIs());
    PR.lparse();
}