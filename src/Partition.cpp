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

#include "Partition.h"

using namespace std;

Partition::Partition()
{
    hasErrors = false;
    hasPrependedSymbols = false;
    hasFollowingSymbols = false;
    from = -1;
    to = -1;

    //ctor
}

Partition::~Partition()
{
    //dtor
}

string Partition::content(bool withHas) {
    string result = to_string(from) + ".." + to_string(to);
    if (withHas) {
        if (hasPrependedSymbols) {result += " has prepends";}
        if (hasFollowingSymbols) {result += " has followers";}
        if (hasErrors) {
            result += " has errors";
        } else {
            result += " no errors";
        }
    }
    return result;
}
