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

#ifndef PARTITION_H
#define PARTITION_H
#include "Enums.h"
#include <string>

using namespace std;

class Partition
{
    public:
        Partition();
        virtual ~Partition();
        string content(bool withHas);
        int from;
        int to;
        LogMsg msg;
        bool hasErrors;
        bool hasPrependedSymbols;
        bool hasFollowingSymbols;
    protected:

    private:
};

#endif // PARTITION_H
