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

#ifndef BOXBASICS_H
#define BOXBASICS_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "globals.h"

using namespace std;

class Boxbasics
{
    public:
        Boxbasics();
        virtual ~Boxbasics();
        // File handling ---------------------
        vector<string> readFile(string name);
        int readFileLine(string name, fstream& fsl, string &result);
        bool existsFile(string filename);
        bool writeFile(string filename, vector<string> &v, bool oneline, string sep, int linelen);
        string getDir(string s);
        bool isPath(string s);
        // vector handling
        bool contains(string s, vector<string> v);
        bool contains(string s, vector<long long int> v);
        bool contains(string s, vector<long double> v);
        vector<string> split2(string name, char c1, char c2);
        vector<string> split0(string name);
        string join(vector<string> &v, string sep);
        void addToVector(vector<string> &v, vector<string> &w);
        void addToVector(vector<long long int> &v, vector<string> &w);
        void addToVector(vector<long double> &v, vector<string> &w);
        // String handling --------------------
        bool findInString(char c, string s);
        char last(string name);
        char first(string name);
        bool startsWith(string text, string s);
        string format(string text);
        // tests --------------------------------
        bool isEmpty(string text);
        bool isEmptyOrZero(string text);
        bool testLong(string text);
        bool testNum(string text);
        bool isLong(string text);
        bool isNumber(string text);
        // conversions --------------------------------
        string cstos(const char* a);
    protected:

    private:
};

#endif // BOXBASICS_H
