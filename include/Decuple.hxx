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

#ifndef DECUPLE_HXX
#define DECUPLE_HXX
#include "Box.h"
#include "OPG.h"
#include <vector>
#include "globals.h"
#include "Boxbasics.h"
#include "Scalar.hxx"
#include "Septuple.hxx"

using namespace std;

template <class T>
class Decuple: public Septuple<T>
{
    public:
    Decuple<T>():
    Septuple<T>::Septuple()
    {

    }

    virtual void init(string pname, string pstname, string pst2name, string pbtname){
        if (debug_I) {Box::log.logmsg4(DEBUG, MSG4, "init", pname, "Decuple", " ");}
        Septuple<T>::init(pname, pstname, pst2name, pbtname);
		setVarDefault("var8");
		setVarDefault("var9");
		setVarDefault("var10");
     }

    virtual string resolve(string text, Boxqentry& bqe, map<string,string>& gvalues) {
        if (debug_R) {Box::log.logmsg4(DEBUG, MSG4, "resolve", text, "Decuple", " ");}
        string s = text;
        if (text == "var8") {
            s = this->xtos(var8);
        } else if (text == "var9") {
            s = this->xtos(var9);
        } else if (text == "var10") {
            s = this->xtos(var10);
        } else {
			s = Septuple<T>::resolve(text, bqe, gvalues) ;
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
		T var8;
		T var9;
		T var10;
    private:

};

template <> void Decuple<long long int>::setVarFromT(string name, string value) {
        if (debug_R) {log.logmsg4(DEBUG, MSG4, "setVarFromT", name, "Decuple", "int");}
        if (name == "var8") {
            var8 = stoll(value);
        } else if (name == "var9") {
            var9 = stoll(value);
        } else if (name == "var10") {
            var10 = stoll(value);
        } else {
            Septuple<long long int>::setVarFromT(name, value);
        }
};

template <> void Decuple<long double>::setVarFromT(string name, string value) {
        if (debug_R) {log.logmsg4(DEBUG, MSG4, "setVarFromT", name, "Decuple", "float");}
        if (name == "var8") {
            var8 = stold(value);
        } else if (name == "var9") {
            var9 = stold(value);
        } else if (name == "var10") {
            var10 = stold(value);
        } else {
            Septuple<long double>::setVarFromT(name, value);
        }
};

template <> void Decuple<string>::setVarFromT(string name, string value) {
    if (debug_R) {log.logmsg4(DEBUG, MSG4, "setVarFromT", name, "Decuple", "string");}
         if (name == "var8") {
            var8 = value;
        } else if (name == "var9") {
            var9 = value;
        } else if (name == "var10") {
            var10 = value;
        } else {
            Septuple<string>::setVarFromT(name, value);
        }
};

template <> void Decuple<long long int>::setVarDefault(string name) {
    if (debug_I) {log.logmsg4(DEBUG, MSG4, "setVarDefault", name, "Decuple", "int");}
    if (name == "var8") {
            var8 = 0;
    } else if (name == "var9") {
            var9 = 0;
    } else if (name == "var10") {
            var10 = 0;
    } else {
        Septuple<long long int>::setVarDefault(name);
    }
};

template <> void Decuple<long double>::setVarDefault(string name) {
    if (debug_I) {log.logmsg4(DEBUG, MSG4, "setVarDefault", name, "Decuple", "float");}
    if (name == "var8") {
            var8 = 0;
    } else if (name == "var9") {
            var9 = 0;
    } else if (name == "var10") {
            var10 = 0;
    } else {
        Septuple<long double>::setVarDefault(name);
    }
};

template <> void Decuple<string>::setVarDefault(string name) {
    if (debug_I) {log.logmsg4(DEBUG, MSG4, "setVarDefault", name, "Decuple", "string");}
    if (name == "var8") {
            var8 = "";
    } else if (name == "var9") {
            var9 = "";
    } else if (name == "var10") {
            var10 = "";
    } else {
        Septuple<string>::setVarDefault(name);
    }
};
#endif // DECUPLE_HXX
