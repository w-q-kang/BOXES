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

#include "Boxlog.h"
#include <iostream>
#include <fstream>

Boxlog::Boxlog()
{
    if (!init) {
        init = true;
        useconsole = true;
        // general
        messagemap[MSG1] = {""};
        messagemap[MSG2] = {"", " "};
        messagemap[MSG3] = {"", " ", " "};
        messagemap[MSG4] = {"", " ", " ", " "};

        // lexer
        messagemap[UNKNOWNFILE] = {"file to be executed doesn't exist: "};
        messagemap[UNKNOWNOPTION] = {"Unknown option "};
        messagemap[CURRENTVERSION] = {"current version is "};

        // parser
        messagemap[CANTFINDPARTBEGIN] = {"can't find beginning of ", " in "};
        messagemap[CANTFINDPARTEND] = {"can't find end of ", " ", " in "};
        messagemap[CANTFINDPARTPP] = {"can't find details of ", " ", " in ", " - reason: prepending symbols"};
        messagemap[CANTBENESTED] = {"", " can't be nested"};
        messagemap[STRUCTTYPUNKNOWN] = {"structure or type unknown: ", " "};
        messagemap[AUGUNKNOWN] = {"augmentation ", " unknown for action ", " - will be ignored."};

        // runtime
        messagemap[CONDQEMPTY] = {"condition queue empty for action "};
        messagemap[UNREDUCEDOPS] = {"unreduced operators: "};
        messagemap[UNUSEDOPS] = {"unused operands: "};
        messagemap[NORESULTFOR] = {"no result for "};
        messagemap[UNKNOWNACTION] = {"unknown action ", " - ignored."};
        messagemap[UNKNOWNSPECIALACTION] = {"unknown special action ", " - ignored."};
        messagemap[HASHSTARNOTALLOWED] = {"#* not allowed in this context: " , " ", " ", " "};
        messagemap[NOTADIRNODEL] = {"", " is not a directory - no deletion."};
        messagemap[DIRNOTCREATED] = {"directory ", " could not be created."};
        messagemap[NOEXECONFILE] = {"", ": could not execute ", " on file "};
        messagemap[AMUSTBEGEB] = {"size of string ", " must be greater or equal size of string "};
        messagemap[UNKNOWNMATHTYPE] = {"unknown type for calculations ", " - string used."};
        messagemap[UNRESOLVEDMATHTYPE] = {"can't determine type for ", " ", " ", " "};
        messagemap[REDEFENTRYRULE] = {"you must not redefine an entry rule ", " ", " ", " "};
        messagemap[NOTYETIMPLEMENTED] = {"the function " , " isn't implemented yet"};
        messagemap[RAGNOLEFTALLOWED] = {"no left side allowed for range operator " , " - found", " entries."};
        messagemap[RAGNORIGHTALLOWED] = {"no right side allowed for range operator " , " - found", " entries."};
        messagemap[RAGOPUNKNOWN] = {"unknown range operator " , " - found", " entries."};
        messagemap[RAGCASENOTHANDLED] = {"case not handled: " , "", "", ""};
        messagemap[QADELNEGINDEX] = {"can't delete negative index " , " from queue array", "", ""};
        messagemap[QADELUNKNOWNINDEX] = {"queue array index " , " doesn't exist - can't be deleted", "", ""};
        messagemap[CANTRESOLVEAT] = {"can't resolve @-expression", "", "", ""};
        messagemap[CANTRESOLVEATRO] = {"using @! is restricted to read only", "", "", ""};
        // debug
        messagemap[ENTRY] = {"Entry = "};
    }
}

Boxlog::~Boxlog()
{
    //dtor
}

void Boxlog::setLevelByName(string levelname) {
    if (levelname == "i") {
        level = INFO;
    } else if (levelname == "w") {
        level = WARN;
    } else if (levelname == "e") {
        level = ERROR;
    } else if (levelname == "f") {
        level = FATAL;
    } else if (levelname == "d") {
        level = DEBUG;
    }
}

void Boxlog::logmsg4(Level lv, LogMsg lmsg, string par1, string par2, string par3, string par4) {
    string text = "";
    vector<string> v = getMessage(lmsg);
    int len = v.size();

    if (lv>=level) {

        text += v[0];
        if (par1 != "") {text += par1;}

        if (1<len) {
            text += v[1];
            if (par2 != "") {text += par2;}
        }

        if (2<len) {
            text += v[2];
            if (par3 != "") {text += par3;}
        }

        if (3<len) {
            text += v[3];
            if (par4 != "") {text += par4;}
        }

        if (4<len) {
            text += v[4];
        }
        if (useconsole) {cerr << datum() << " " << levelString(lv) << text << endl;}
        fstream fs;
        fs.open(filename,ios::app);
        if (fs.is_open()){
            fs << datum() << " " << levelString(lv) <<text << "\n";
        }
        fs.close(); //close the file object.
   }
}

string Boxlog::levelString(Level level) {
        string result = "";
        switch(level) {
            case DEBUG:
                result = "DEBUG - ";
                break;
            case INFO:
                result = "INFO - ";
                break;
            case WARN:
                result = "WARN - ";
                break;
            case ERROR:
                result = "ERROR - ";
                break;
            case FATAL:
                result = "FATAL - ";
                break;
        }
        return result;
}

vector<string> Boxlog::getMessage(LogMsg lmsg) {
    return messagemap[lmsg];
}

string Boxlog::datum() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[20];

    sprintf(buffer, "%02d", ltm->tm_mday);
    string result = buffer;
    sprintf(buffer, "%02d", (ltm->tm_mon) + 1);
    result += ".";
    result += buffer;
    sprintf(buffer, "%02d", (ltm->tm_year) - 100);
    result += ".";
    result += buffer;
    sprintf(buffer, "%02d", (ltm->tm_hour));
    result += " ";
    result += buffer;
    sprintf(buffer, "%02d", (ltm->tm_min));
    result += ":";
    result += buffer;
    sprintf(buffer, "%02d", (ltm->tm_sec));
    result += ":";
    result += buffer;

    return result;

}

