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

#ifndef BOXER_H
#define BOXER_H
#include <vector>
#include <string>
#include "Enums.h"

using namespace std;

class Boxer
{
    public:
        Boxer();
        virtual ~Boxer();
        void readSource(string filename, Lextype typ);
        vector<string> splitContent();
    protected:

    private:
      vector<string> content;
      vector<string> symbols;
};

#endif // BOXER_H
