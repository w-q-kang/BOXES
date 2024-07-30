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

#ifndef MMOVERULE_H
#define MMOVERULE_H
#include <string>
#include "Enums.h"

using namespace std;

template <class T>
class Mmoverule
{
    public:
        Mmoverule(string ptype, string pinput) {
            mtype = ptype;
            input = pinput;
            anyflag = false;
        };
        virtual ~Mmoverule(){
            // dtor
        };

        string key(string sep) {
            return mtype + sep + input;
        }

        virtual void setRule(string poprel, string pold, string pdirection, string poutput) {

        }

        virtual bool isvalid(string ptype, string pin, T oldval) {
            return false;
        }
		string mtype;
		string input;
		bool anyflag;
		Optype oprel;
		T oldvalue;
		string direction;
		string output;

    protected:

    private:

};


template <> void Mmoverule<string>::setRule(string poprel, string pold, string pdirection, string poutput) {
    if (poprel == "==") {
        oprel = OTEQUAL;
    } else if (poprel == "!=") {
        oprel = OTUNEQUAL;
    } else if (poprel == "=~") {
        oprel = OTCONTAINS;
    } else if (poprel == "!~") {
        oprel = OTNOTCONTAINS;
    }
    if (pold == "@any") {
        anyflag = true;
    } else {
        oldvalue = pold;
    }

    direction = pdirection;
    output = poutput;
}

template <> void Mmoverule<long long int>::setRule(string poprel, string pold, string pdirection, string poutput) {
    if (poprel == "==") {
        oprel = OTEQUAL;
    } else if (poprel == "!=") {
        oprel = OTUNEQUAL;
    } else if (poprel == "=~") {
        oprel = OTCONTAINS;
    } else if (poprel == "!~") {
        oprel = OTNOTCONTAINS;
    }

    if (pold == "@any") {
        anyflag = true;
    } else {
        oldvalue = stoll(pold);
    }

    direction = pdirection;
    output = poutput;
}

template <> bool Mmoverule<string>::isvalid(string ptype, string pin, string poldval) {
        if (ptype != mtype) { return false;}
        if (input != pin) {return false;}
        if (anyflag) {
            return true;
        } else if (oprel == OTEQUAL) {
            return oldvalue == poldval;
        } else if (oprel == OTUNEQUAL) {
            return oldvalue != poldval;
        } else if (oprel == OTCONTAINS) {
            return poldval.find(oldvalue) != std::string::npos;
        } else if (oprel == OTNOTCONTAINS) {
            return poldval.find(oldvalue) == std::string::npos;
        };
        return false;
}

template <> bool Mmoverule<long long int>::isvalid(string ptype, string pin, long long int poldval) {
        if (ptype != mtype) { return false;}
        if (input != pin) {return false;}
        if (anyflag) {
            return true;
        } else if (oprel == OTEQUAL) {
            return oldvalue == poldval;
        } else if (oprel == OTUNEQUAL) {
            return oldvalue != poldval;
        };
        return false;
}


#endif // MMOVERULE_H
