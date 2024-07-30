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

#ifndef BOXPARSE_H
#define BOXPARSE_H

#include <string>
#include <vector>
#include "Partition.h"
#include "Box.h"
#include "Boxlog.h"
#include "Boxaction.h"
#include "Boxbasics.h"

using namespace std;

class Boxparse
{
    public:
        Boxparse();
        virtual ~Boxparse();
        void setSymbols(vector<string> & psymbols);
        Partition findNextPart(int from, int to, vector<string> starttags, vector<string> endtags, bool canNest, bool special);
        bool doParse();
        bool doParseResponses(Box* box, int ip, int pfrom, int pto);
        Partition checkNextAction(int from,int to, vector<string> endtags, bool canNest);
        vector<Boxaction> splitResponse(int from, int to);
        void addQref(string name, int index);
        vector<int> getQref(string name);
        Box* getPointer(int idx);
    protected:

    private:
        Boxlog log;
        Boxbasics b;
        vector<string> symbols;
        map<int, Box*> pointers;
        map<string, vector<int>> qref;
        vector<string> chain;
        string chainlinks;
};

#endif // BOXPARSE_H
