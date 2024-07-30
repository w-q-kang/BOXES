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

#include "Boxaction.h"

using namespace std;

Boxaction::Boxaction(string pname)
{
    name = pname;
    hasIf = false;
    hasIfnot = false;
}

string Boxaction::getName() {
    return name;
}

string Boxaction::getArgument(int i) {
    if (i < (int) arguments.size()) {
        return arguments[i];
    } else {
        return "";
    }
}


void Boxaction::setArguments(vector<string>& v) {
    arguments = v;
}

vector<string> Boxaction::getArguments() {
    return arguments;
}

bool Boxaction::hasArguments() {
    return arguments.size() > 0;
}

int Boxaction::argsize() {
    return (int) arguments.size();
}
vector<string> Boxaction::getArgsTail(int from) {
    vector<string> v;
    for (int i = from; i < (int) arguments.size(); ++i) {
        v.push_back(arguments[i]);
    }
    return v;
}

void Boxaction::setAug(string text) {
    if (text == "if") {
        hasIf = true;
    } else if (text == "ifnot") {
        hasIfnot= true;
    } else if (aug1 == "") {
        aug1 = text;
    } else if (aug2 == "") {
        aug2 = text;
    } else if (aug3 == "") {
        aug3 = text;
    }
}

string Boxaction::getAug1() {
    return aug1;
}

string Boxaction::getAug2() {
    return aug2;
}

string Boxaction::getAug3() {
    return aug3;
}

bool Boxaction::getIf() {
    return hasIf;
}

bool Boxaction::getIfnot() {
    return hasIfnot;
}
