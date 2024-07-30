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

#ifndef PAIR_H
#define PAIR_H
#include <string>

using namespace std;

template <class T>
class Pair
{
    public:
        Pair() {
            // default ctor
        }

        Pair(T f, T t) {
            if (f <= t) {
                from = f;
                to = t;
            } else {
                from = t;
                to = f;
            }
        }

        ~Pair()
        {
            //dtor
        }

       string content() {
            return to_string(from) + ".." + to_string(to);
        }

        T from;
        T to;
    protected:

    private:
};

//template <> string Pair<long long int>::content() {
//    return to_string(from) + ".." + to_string(to);
//}
//
//template <> string Pair<long double>::content() {
//    return to_string(from) + ".." + to_string(to);
//}
#endif // PAIR_H
