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

#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
#include <vector>

using namespace std;

extern vector<string> actionwords;
extern vector<string> varints;
extern vector<string> augmentations;
extern bool debug_L;
extern bool debug_P;
extern bool debug_X;
extern bool debug_Q;
extern bool debug_I;
extern bool debug_R;
extern bool debug_C;
extern bool debug_M;
extern bool debug_q;
extern bool debug_o;
extern bool debug_m;
#endif // GLOBALS_H
