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

#ifndef RAG_H
#define RAG_H
#include "Boxbasics.h"
#include "Boxlog.h"
#include <string>
#include <vector>

using namespace std;

class RAG : public Boxbasics
{
    public:
        RAG();
        virtual ~RAG();
        vector<string> run(vector<string>& tokens);
        bool isUnary(string token);
        bool isInfixRR(string token);
        bool isInfixRV(string token);
        bool isSep(string token);
        bool isReversed(string token);
        tuple<vector<string>, bool> doInfixRR(string left, string op, string right);
        tuple<vector<string>, bool> doInfixRV(string left, string op, string right);
        tuple<string, bool> doUnary(string op, string right);
        vector<string> doOps(string op, vector<string>& left, vector<string>& right, bool reversed);
    protected:

    private:
        Boxlog log;
        vector<string> noRanges;
};

#endif // RAG_H
