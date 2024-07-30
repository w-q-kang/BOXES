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

#ifndef QUINTUPLE_HXX
#define QUINTUPLE_HXX
#include "Box.h"
#include "OPG.h"
#include <vector>
#include "globals.h"
#include "Boxbasics.h"
#include "Scalar.hxx"

using namespace std;

template <class T>
class Quintuple: public Scalar<T>
{
    public:
    Quintuple<T>():
    Scalar<T>::Scalar()
    {

    }

    virtual void init(string pname, string pstname, string pst2name, string pbtname){
        if (debug_I) {Box::log.logmsg4(DEBUG, MSG4, "init", pname, "Quintuple", " ");}
        Scalar<T>::init(pname, pstname, pst2name, pbtname);
        setVarDefault("var1");
        setVarDefault("var2");
        setVarDefault("var3");
        setVarDefault("var4");
        setVarDefault("var5");
     }

    virtual string resolve(string text, Boxqentry& bqe, map<string,string>& gvalues) {
        if (debug_R) {Box::log.logmsg4(DEBUG, MSG4, "resolve", text, "Quintuple", " ");}
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
        } else if (text == "var4") {
            s = this->xtos(var4);
        } else if (text == "var5") {
            s = this->xtos(var5);
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

    virtual vector<string> resolveAll(vector<string> par, Boxqentry& bqe, map<string,string>& gvalues) {
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


    virtual void setVarFromT(string name, string value) {

    }

    virtual void setVarDefault(string name) {

    }
    protected:
        T var2;
        T var3;
        T var4;
        T var5;
    private:

};

template <> void Quintuple<long long int>::setVarFromT(string name, string value) {
        if (debug_R) {log.logmsg4(DEBUG, MSG4, "setVarFromT", name, "Quintuple", "int");}
        if ((name == "var") || (name == "var1")) {
            var = stoll(value);
        } else if (name == "var2") {
            var2 = stoll(value);
        } else if (name == "var3") {
            var3 = stoll(value);
        } else if (name == "var4") {
            var4 = stoll(value);
        } else if (name == "var5") {
            var5 = stoll(value);
        }

};

template <> void Quintuple<long double>::setVarFromT(string name, string value) {
        if (debug_R) {log.logmsg4(DEBUG, MSG4, "setVarFromT", name, "Quintuple", "float");}
        if ((name == "var") || (name == "var1")) {
            var = stold(value);
        } else if (name == "var2") {
            var2 = stold(value);
        } else if (name == "var3") {
            var3 = stold(value);
        } else if (name == "var4") {
            var4 = stold(value);
        } else if (name == "var5") {
            var5 = stold(value);
        }

};

template <> void Quintuple<string>::setVarFromT(string name, string value) {
    if (debug_R) {log.logmsg4(DEBUG, MSG4, "setVarFromT", name, "Quintuple", "string");}
         if ((name == "var") || (name =="var1")) {
            var = value;
        } else if (name == "var2") {
            var2 = value;
        } else if (name == "var3") {
            var3 = value;
        } else if (name == "var4") {
            var4 = value;
        } else if (name == "var5") {
            var5 = value;
        }
};
template <> void Quintuple<long long int>::setVarDefault(string name) {
    if (debug_I) {log.logmsg4(DEBUG, MSG4, "setVarDefault", name, "Quintuple", "int");}
    if ((name == "var") || (name =="var1")) {
            var = 0;
    } else if (name == "var2") {
            var2 = 0;
    } else if (name == "var3") {
            var3 = 0;
    } else if (name == "var4") {
            var4 = 0;
    } else if (name == "var5") {
            var5 = 0;
    }
};

template <> void Quintuple<long double>::setVarDefault(string name) {
    if (debug_I) {log.logmsg4(DEBUG, MSG4, "setVarDefault", name, "Quintuple", "float");}
    if ((name == "var") || (name =="var1")) {
            var = 0;
    } else if (name == "var2") {
            var2 = 0;
    } else if (name == "var3") {
            var3 = 0;
    } else if (name == "var4") {
            var4 = 0;
    } else if (name == "var5") {
            var5 = 0;
    }
};

template <> void Quintuple<string>::setVarDefault(string name) {
    if (debug_I) {log.logmsg4(DEBUG, MSG4, "setVarDefault", name, "Quintuple", "string");}
    if ((name == "var") || (name =="var1")) {
            var = "";
    } else if (name == "var2") {
            var2 = "";
    } else if (name == "var3") {
            var3 = "";
    } else if (name == "var4") {
            var4 = "";
    } else if (name == "var5") {
            var5 = "";
    }
};
#endif // QUINTUPLE_HXX
