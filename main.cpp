#include <iostream>
#include <string>
#include "Parser.h"

int main()
{
    Parser par("(300+23)*(43-21)/(84+7)");
    par.convertPostfix();
    if(par.isComputable())
        par.calculate();

    //if the expression is valid then display answer, else only display expression
    if(par.isValidExpression())
    {
        std::cout << "Infix:\n" << par.getInfix() << "\n\n";
        std::cout << "Postfix:\n" << par.getPostfix() << "\n\n";
        if(par.isComputable())
            std::cout << "Answer:\n" << par.getAnswer() << "\n\n";
    }
    else
        std::cout << "Invalid Expression:\n" << par.getInfix() << "\n\n";

    return 0;
}
