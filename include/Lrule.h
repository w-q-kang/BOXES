#ifndef LRULE_H
#define LRULE_H
#include <string>
#include <vector>

using namespace std;

class Lrule
{
    public:
        Lrule(string pposition, vector<string> pcondition, vector<string> pexpression);
        virtual ~Lrule();
        vector<string> getCondition();
        vector<string> getExpression();
        string getPosition();
    protected:

    private:
        string position;
        vector<string> condition;
        vector<string> expression;
};

#endif // LRULE_H
