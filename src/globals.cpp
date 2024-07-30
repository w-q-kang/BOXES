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

#include "globals.h"

using namespace std;

vector<string> actionwords = {"addstates", "addentryrule", "addmoverule", "addrule", "append", "arrange",
        "branch", "break",
        "change", "clear", "collect", "compact", "cond", "copy",
        "decr", "delete", "dir", "do", "exists",
        "file", "find", "finis", "if", "ifnot",
        "impose", "incr", "incindex", "init", "input", "insert", "intersect",
        "localchange", "map", "minus", "partition", "print",
        "qadd", "qdel", "qget", "qins", "qset",
        "read", "readline", "readvars", "repeat", "reverse", "runmachine", "rv",
        "save", "select", "send", "sep", "set", "setrange", "sl", "sort", "sr", "stop", "swap",
        "tl", "translate", "with", "write"};

vector<string> varints = {"idx", "idy", "idz"};
vector<string> augmentations = {"asc", "at", "col", "create", "delete", "desc", "else", "exists", "field", "float",
                                "idx", "if", "ifnot", "inc", "inclt", "int", "keep", "line", "lt", "max", "min", "mixed", "new", "next", "nl", "not",
                                "once", "plus", "q", "refeed", "reverse", "row", "rules", "string", "vars"};
bool debug_L = false;
bool debug_P = false;
bool debug_X = false;
bool debug_Q = false;
bool debug_I = false;
bool debug_R = false;
bool debug_C = false;
bool debug_M = false;
bool debug_o = false;
bool debug_q = false;
bool debug_r = false;
bool debug_m = false;

