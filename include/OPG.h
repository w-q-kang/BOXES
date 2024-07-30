/*
Copyright 2023-2024 W. Q. Kang

This file is part of the BOXES Interpreter.

    The BOXES Interpreter is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The BOXES Interpreter is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the BOXES Interpreter.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OPG_H
#define OPG_H
#include "Boxbasics.h"
#include "Boxlog.h"

using namespace std;

class OPG : public Boxbasics
{
    public:
        OPG();
        virtual ~OPG();

        int f(string &token, string &stype);
        int fnum(string &token);
        int fstring(string &token);
        int g(string &token, string &stype);
        int gnum(string &token);
        int gstring(string &token);
        bool isUnary(string &token);
        bool isTernary(string &token);
        bool isFloatOnly(string &token);
        bool isStringOnly(string &token);
        bool isNotString(string &token);
        string calculateLong(string &op, string &first, string &second);
        string calculateDouble(string &op, string &first, string &second);
        string calculateString(string &op, string &first, string &second);
        string calculate(string &op, string &first, string &second, string &stype);
        string calculateLong3(string &op, string &first, string &second, string &third);
        string calculateDouble3(string &op, string &first, string &second, string &third);
        string calculateString3(string &op, string &first, string &second, string &third);
        string calculate3(string &op, string &first, string &second, string &third, string &stype);
        string run(vector<string> tokens, string stype);
        int doMatchFront(string a, string b);
        bool doMatchLeft(string a, string b);
        bool doHeadEq(string a, string b);
        bool doHeadGe(string a, string b);
    protected:

    private:
       Boxlog logging;
};

#endif // OPG_H
