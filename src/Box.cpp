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

#include "Box.h"
#include "globals.h"

Box::Box()
{
}

void Box::init(string pname, string pstname, string pst2name, string pbtname)
{
    name = pname;
    stname = pstname;
    st2name = pst2name;
    btname = pbtname;
    iosep = "";
    myclassname = proper(pstname) + proper(pst2name);
    if (debug_I) {log.logmsg4(DEBUG, MSG4, "init", pname, "Box", " ");}
}

void Box::setResponse(string s, vector<Boxaction>& a) {
    responses[s] = a;
}

void Box::setCurrDir(string currdir) {
    currDirectory = currdir;
    char l = currDirectory[currDirectory.size()-1];

    if ((l != '/') && (l != '\\')) {
        currDirectory += "/";
    }
}

string Box::proper(string text) {
    string result = text;
    if (text.size() > 0) {
        result = toupper(text[0]) + "";
        if (text.size() > 1) {
            result += text.substr(1);
        }
    }
    return result;
}

// must be defined, though useless
int Box::callResponse(string s, deque<Boxqentry>& theq, Boxqentry& thebqe, map<string,string>& gvalues) {return 0;};

int Box::doPrint(vector<string> v, bool hasNl){return 1;};

int Box::doFinis() {
    if (condqueue.size() > 0) {
        condqueue.pop_front();
    }
    return 1;
}

int Box::doExit() {
    exit(1);
    return 0;
}

int Box::doNop() {return 1;}

int Box::doSep(string s) {
    if (s=="@none") {
        iosep = "";
    } else {
        iosep=s;
    }
    return 1;
}

int Box::doSend(string name, vector<string>& v, deque<Boxqentry>& theq) {
    Boxqentry bqe = Boxqentry(name);
    bqe.setState("new");
    bqe.setParams(v);
    theq.push_back(bqe);

    return 1;
}

string Box::getInfo(int i) {
    string result = name;
    if (i>1) { result += " " + stname;}
    if (i>2) { result += " " + btname;}

    return result;
}

// disambiguation
string Box::xtos(size_t l) {
    return to_string(l);
}
// disambiguation
string Box::xtos(int l) {
    return to_string(l);
}

string Box::xtos(long long int l) {
    return to_string(l);
}

string Box::xtos(long double ld) {
    return to_string(ld);
}

string Box::xtos(string s) {
    return s;
}

long long int Box::xtoll(long long int l) {
    return l;
}

long long int Box::xtoll(string s) {
    return stoll(s);
}

// -------- pushbackT

void Box::pushbackT(vector<long long int>& v, long long int lli) {
    v.push_back(lli);
}

void Box::pushbackT(vector<string>& v, string s) {
    v.push_back(s);
}

void Box::pushbackT(vector<long double>& v, long double ld) {
    v.push_back(ld);
}

void Box::pushbackT(vector<long long int>& v, string s) {
    v.push_back(stoll(s));
}

void Box::pushbackT(vector<long double>& v, string s) {
    v.push_back(stold(s));
}

// ---------- setvecT
void Box::setvecT(vector<string>& v, int index, string s) {
    v.at(index) = s;
};

void Box::setvecT(vector<long long int>& v, int index, string s) {
    v.at(index) = stoll(s);
};

void Box::setvecT(vector<long double>& v, int index, string s) {
    v.at(index) = stold(s);
};
