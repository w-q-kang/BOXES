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

#include "Boxqentry.h"

using namespace std;

Boxqentry::Boxqentry(string pname)
{
    name = pname;
    index = -1;
    hasInt = false;
}

Boxqentry::Boxqentry()
{
    index = -1;
    hasInt = false;
}

Boxqentry::~Boxqentry()
{
    //dtor
}

void Boxqentry::setName(string pname) {
    name = pname;
}

void Boxqentry::setState(string pstate) {
    state = pstate;
}

void Boxqentry::setBoxids(vector<int>& v) {
    boxids = v;
}

void Boxqentry::setParams(vector<string>& v) {
    params = v;
}

string Boxqentry::getName() {
    return name;
}

string Boxqentry::getState() {
    return state;
}

int Boxqentry::getNextID() {
    int result = -1;
    index++;
    if (index < (int) boxids.size()) {
        result = boxids[index];
    }
    return result;
}

vector<string> Boxqentry::getParams(){
    return params;
}

bool Boxqentry::hasParams() {
    return params.size() > 0;
}

string Boxqentry::getParam(int i) {
    return params[i];
}
