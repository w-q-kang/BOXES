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

#ifndef BOXLOG_H
#define BOXLOG_H
#include <string>
#include "Enums.h"
#include <map>


#include <vector>
#include <chrono>

using namespace std;
class Boxlog
{
    public:
        Boxlog();
        virtual ~Boxlog();
        static string filename;
        static bool useconsole;
        static Level level;
        static bool init;
        static map<LogMsg,vector<string>> messagemap;
        vector<string> getMessage(LogMsg lmsg);
        void logmsg4(Level lv, LogMsg lmsg, string par1, string par2, string par3, string par4);
        string levelString(Level level);
        void setLevelByName(string levelname);
        string datum();
    protected:

    private:
};

#endif // BOXLOG_H
