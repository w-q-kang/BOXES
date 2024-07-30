#include "Lrule.h"

Lrule::Lrule(string pposition, vector<string> pcondition, vector<string> pexpression)
{
    position= pposition;
    condition = pcondition;
    expression = pexpression;
}

Lrule::~Lrule()
{
    //dtor
}

vector<string> Lrule::getCondition() {
    return condition;
}

vector<string> Lrule::getExpression() {
    return expression;
}
string Lrule::getPosition() {
    return position;
}
