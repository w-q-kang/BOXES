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

#ifndef MCHANGE_H
#define MCHANGE_H
#include <string>
#include "Enums.h"

using namespace std;

template <class T>
class Mchange
{
    public:
        Mchange(int poldrow, int poldcol, string pout) {
            oldrow = poldrow;
            oldcol = poldcol;
            output = pout;
        };

        virtual ~Mchange(){
            // dtor
        };

        void setNewpos(int prow, int pcol) {
            newrow = prow;
            newcol = pcol;
        }

        void setIndex(int idx) {
            index = idx;
        }

 		int oldrow;
		int oldcol;
		string output;
		int newrow;
		int newcol;
		int index; // current input index
    protected:

    private:

};

#endif // MCHANGE_H
