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

#ifndef BOXQENTRY_H
#define BOXQENTRY_H
#include <string>
#include <vector>

using namespace std;

class Boxqentry
{
    public:
        Boxqentry(string pname);
        Boxqentry();
        void setName(string pname);
        virtual ~Boxqentry();
        void setState(string pstate);
        void setBoxids(vector<int>& v);
        void setParams(vector<string>& v);

        string getName();
        string getState();
        int getNextID();
        bool hasParams();
        vector<string> getParams();
        string getParam(int i);
    protected:

    private:
        string name;
        string typ;
        string state;
        vector<int> boxids;
        int index;
        bool hasInt;
        vector<string> params;
};

#endif // BOXQENTRY_H
