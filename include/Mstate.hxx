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

#ifndef MSTATE_H
#define MSTATE_H
#include <string>
#include <iostream>

using namespace std;

template <class T>
class Mstate
{
    public:
        Mstate() {
        }
        Mstate(string pspec, T pvalue) {
            spec = pspec;
            value = pvalue;
        };
        virtual ~Mstate(){
            // dtor
        };

        virtual string content() {
            return "";
        }

        virtual void update(Optype pop, T pvalue) {

        }

		string spec; // meaning may be type or name, depending on Machine it is used in
		T value;
    protected:

    private:

};

template <> string Mstate<string>::content() {
        return spec + "/" + value;
}

template <> string Mstate<long long int>::content() {
        return spec + "/" + to_string(value);
}

template <> void Mstate<string>::update(Optype pop, string pvalue) {
        if (pop == OTSET) {
            value = pvalue;
        } else if (pop == OTAPPEND) {
            value = value + pvalue;
        }
}

template <> void Mstate<long long int>::update(Optype pop, long long int pvalue) {
        if (pop == OTSET) {
            value = pvalue;
        } else if (pop == OTADD) {
            value = value + pvalue;
        } else if (pop == OTMINUS) {
            value = value - pvalue;
        }
}

template <> void Mstate<long double>::update(Optype pop, long double pvalue) {
        if (pop == OTSET) {
            value = pvalue;
        } else if (pop == OTADD) {
            value = value + pvalue;
        } else if (pop == OTMINUS) {
            value = value - pvalue;
        }
}
#endif // MSTATE_H
