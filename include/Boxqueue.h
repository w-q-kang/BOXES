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

#ifndef BOXQUEUE_H
#define BOXQUEUE_H
#include <deque>
#include <map>
#include "Boxqentry.h"
#include "Boxparse.h"
#include "Boxlog.h"

class Boxqueue
{
    public:
        Boxqueue(Boxparse& pboxparse);
        virtual ~Boxqueue();
        int run(string path, vector<string> pargs);
        void add(Boxqentry bqe);
        string getCurrDirectory();
    protected:

    private:
        Boxlog log;
        deque<Boxqentry> theq;
        Boxparse boxparse;
        string currDirectory;
        map<string, string> gvalues;

};

#endif // BOXQUEUE_H
