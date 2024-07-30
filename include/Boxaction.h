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

#ifndef BOXACTION_H
#define BOXACTION_H
#include <string>
#include <vector>

using namespace std;

class Boxaction
{
    public:
        Boxaction(string pname);
        string getName();
        void setArguments(vector<string>& v);
        vector<string> getArguments();
        bool hasArguments();
        int argsize();
        vector<string> getArgsTail(int from); // only tail of arguments
        string getArgument(int i);
        void setAug(string text);
        string getAug1();
        string getAug2();
        string getAug3();
        bool getIf();
        bool getIfnot();
    protected:

    private:
        string name;
        string aug1;
        string aug2;
        string aug3;
        vector<string> arguments;
        bool hasIf;
        bool hasIfnot;
};

#endif // BOXACTION_H
