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

#ifndef BOX_H
#define BOX_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include "Boxaction.h"
#include "Boxlog.h"
#include "Boxqentry.h"
#include "Boxbasics.h"

using namespace std;

class Box
{
    public:
        Box();
        virtual void init(string pname, string pstname, string pst2name, string pbtname);
        string getInfo(int i);
        void setCurrDir(string currdir);
        void setResponse(string s, vector<Boxaction>& a);
        virtual int doPrint(vector<string> v, bool hasNl);
        virtual int callResponse(string s, deque<Boxqentry>& theq, Boxqentry& thebqe,map<string,string> & gvalues);
        int doFinis();
        int doExit();
        int doNop();
        int doSep(string s);
        int doSend(string name, vector<string>& v, deque<Boxqentry>& theq);
        string proper(string text);
        string xtos(size_t l);
        string xtos(int l);
        string xtos(long long int l);
        string xtos(long double ld);
        string xtos(string s);

        long long int xtoll(long long int l);
        long long int xtoll(string s);
        void pushbackT(vector<long long int>& v, string s);
        void pushbackT(vector<long long int>& v, long long int lli);
        void pushbackT(vector<string>& v, string s);
        void pushbackT(vector<long double>& v, string s);
        void pushbackT(vector<long double>& v, long double ld);

        void setvecT(vector<long long int>& v, int index, string s);
        void setvecT(vector<string>& v, int index, string s);
        void setvecT(vector<long double>& v, int index, string s);

    protected:
        Boxlog log;
        Boxbasics bxb;
        string name;
        string stname;
        string st2name;
        string btname;
        deque<bool> condqueue;
        map<string, vector<Boxaction>> responses;
        fstream fsl;
        string currDirectory;
        string iosep;
        string myclassname;
    private:

};

#endif // BOX_H
