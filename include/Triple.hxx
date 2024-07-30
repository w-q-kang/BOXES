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

#ifndef TRIPLE_HXX
#define TRIPLE_HXX
#include "Box.h"
#include "OPG.h"
#include <vector>
#include "globals.h"
#include "Boxbasics.h"
#include "Scalar.hxx"

using namespace std;

template <class T>
class Triple: public Scalar<T>
{
    public:
    Triple<T>():
    Scalar<T>::Scalar()
    {

    }

    void init(string pname, string pstname, string pst2name, string pbtname){
        if (debug_I) {Box::log.logmsg4(DEBUG, MSG4, "init", pname, "Triple", " ");}
        Scalar<T>::init(pname, pstname, pst2name, pbtname);
        setVarDefault("var1");
        setVarDefault("var2");
        setVarDefault("var3");
     }

    string resolve(string text, Boxqentry& bqe, map<string,string>& gvalues) {
        if (debug_R) {Box::log.logmsg4(DEBUG, MSG4, "resolve", text, "Triple", " ");}
        string s = text;
        if (text == "@space") {
            s = " ";
        } else if (text == "@bar") {
            s = "|";
        } else if (text == "@hash") {
            s = "#";
        } else if (text == "@none") {
            s = "";
        } else if ((text == "var") || (text == "var1")) {
            s = this->xtos(Scalar<T>::var);
        } else if (text == "var2") {
            s = this->xtos(var2);
        } else if (text == "var3") {
            s = this->xtos(var3);
        } else if (text[0] == '#') {
            string x1 = text.substr(1);
            int idx = stoi(x1) - 1;
            s = bqe.getParam(idx);
        } else if (text[0] == '$') {
            string x1 = text.substr(1);
            if (x1[0] == '!') {
                x1 = x1.substr(1);
                x1 = resolve(x1,bqe, gvalues);
            }
            s = gvalues[x1];
        }
        return s;
    }

    vector<string> resolveAll(vector<string> par, Boxqentry& bqe, map<string,string>& gvalues) {
        vector<string> result;
        for (int i = 0; i < (int) par.size(); ++i) {
             if (par[i] == "#*") {
                vector<string> w = bqe.getParams();
                Box::bxb.addToVector(result, w);
            } else if (par[i] == "@gvars") {
                for (map<string, string>::iterator it=gvalues.begin(); it!=gvalues.end(); ++it) {
                    result.push_back(it->first);
                }
            } else {
                result.push_back(resolve(par[i], bqe, gvalues));
            }
        }
        return result;
    }


    void setVarFromT(string name, string value) {

    }

    void setVarDefault(string name) {

    }
    protected:
        T var2;
        T var3;
    private:

};

template <> void Triple<long long int>::setVarFromT(string name, string value) {
        if (debug_R) {log.logmsg4(DEBUG, MSG4, "setVarFromT", name, "Triple", "int");}
        if ((name == "var") || (name == "var1")) {
            var = stoll(value);
        } else if (name == "var2") {
            var2 = stoll(value);
        } else if (name == "var3") {
            var3 = stoll(value);
        }

};

template <> void Triple<long double>::setVarFromT(string name, string value) {
        if (debug_R) {log.logmsg4(DEBUG, MSG4, "setVarFromT", name, "Triple", "double");}
        if ((name == "var") || (name == "var1")) {
            var = stold(value);
        } else if (name == "var2") {
            var2 = stold(value);
        } else if (name == "var3") {
            var3 = stold(value);
        }

};


template <> void Triple<string>::setVarFromT(string name, string value) {
    if (debug_R) {log.logmsg4(DEBUG, MSG4, "setVarFromT", name, "Triple", "string");}
         if ((name == "var") || (name =="var1")) {
            var = value;
        } else if (name == "var2") {
            var2 = value;
        } else if (name == "var3") {
            var3 = value;
        }
};
template <> void Triple<long long int>::setVarDefault(string name) {
    if (debug_I) {log.logmsg4(DEBUG, MSG4, "setVarDefault", name, "Triple", "int");}
    if ((name == "var") || (name =="var1")) {
            var = 0;
    } else if (name == "var2") {
            var2 = 0;
    } else if (name == "var3") {
            var3 = 0;
    }
};

template <> void Triple<long double>::setVarDefault(string name) {
    if (debug_I) {log.logmsg4(DEBUG, MSG4, "setVarDefault", name, "Triple", "float");}
    if ((name == "var") || (name =="var1")) {
            var = 0;
    } else if (name == "var2") {
            var2 = 0;
    } else if (name == "var3") {
            var3 = 0;
    }
};

template <> void Triple<string>::setVarDefault(string name) {
    if (debug_I) {log.logmsg4(DEBUG, MSG4, "setVarDefault", name, "Triple", "string");}
    if ((name == "var") || (name =="var1")) {
            var = "";
    } else if (name == "var2") {
            var2 = "";
    } else if (name == "var3") {
            var3 = "";
    }
};
#endif // TRIPLE_HXX
