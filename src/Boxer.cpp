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

#include "Boxer.h"
#include "Boxbasics.h"

using namespace std;

Boxer::Boxer()
{
    //ctor
}

Boxer::~Boxer()
{
    //dtor
}

void Boxer::readSource(string filename, Lextype typ) {
    Boxbasics b;
    if (typ == NORMAL) {
        content = b.readFile(filename);
    } else if (typ == APPEND) {
        vector<string> v = b.readFile(filename);
        content.insert(content.end(), v.begin(), v.end());
    }
}

    // make a lexical analysis of line no. index
vector<string> Boxer::splitContent() {
    Boxbasics b;
    vector<string> liste;
    bool inside = false;

    for (int i = 0; i < (int) content.size(); ++i) {
        string s = content[i];
        string word = "";
        int len = s.size();
        for (int j = 0; j < len; ++j) {
            char ch = s[j];
            if (ch == '/') {
                if (j+1 < len) {
                    if (s[j+1] == '/') {
                        break;
                    }
                }
            }
            if (inside) {
                word += ch;
            } else if (b.findInString(ch, " \t")) {
                if (word.size() > 0) {
                    liste.push_back(word);
                    word = "";
                }
                if (b.findInString(ch, ",")) {
                    word = ch;
                    liste.push_back(word);
                    word = "";
                }
            } else {
                word += ch;
            }

            if (ch == '"') {inside = !inside;}
        }

        if (word.size() > 0) {liste.push_back(word);}


    }

     return liste;
}
