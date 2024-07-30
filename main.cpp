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

#include <iostream>
#include "Enums.h"
#include "globals.h"
#include "Boxbasics.h"
#include "Boxlog.h"
#include "Boxer.h"
#include "Boxparse.h"
#include "Partition.h"
#include "Boxqueue.h"

using namespace std;

// In C++ non-const static members must be both declared in the class definition
// and defined with global scope to properly give the linker something to reference.
//

string Boxlog::filename = "protocol.txt";
Level Boxlog::level = DEBUG;
bool Boxlog::useconsole = true;
bool Boxlog::init = false;
map<LogMsg,vector<string>> Boxlog::messagemap;

// options
// s<n> = scope, where <n> = l -> Lexer, p =-> Lexer + Parser, x -> all (default)
// l<n> = loglevel, where <n> = i -> info, w -> warn, e -> error, f -> fatal
// d<n> = debug, where <n> =
// L = debug Lexer, P -> Parser, X -> Execution,
// Q -> Queue, I -> Inits, R-> resolve varnames, C-> print chaining
// M -> message calls, o -> OPG,RAG q -> Queue Stack, m -> machine


int main(int argc, char *argv[])
{

     string current_version = "0.9.7";
     bool foundFile = false;
     string scope = "x";
     string debugtypes = "";
     string loglevel = "i";
     string filename = "";
     string original = "";
     vector<string> pargs;
     vector<string> lexout;
     int retcode = 0;
     bool retflag = true;
     Boxbasics b;
     Boxlog log;
     Boxer bx;
     Boxparse bp;
     Partition pt;
     string workDirectory = b.getDir(argv[0]);

    if (argc <= 1) {
      cout << "option or program missing" << endl;
    } else {
        // read args
       for (int i = 1; i < argc; ++i) {
        string sargv = argv[i];
        if (foundFile) {
            pargs.push_back(sargv);
        } else if  (sargv.substr(0,2) == "-s") {
           scope = sargv.substr(2);
        } else if  (sargv.substr(0,2) == "-d") {
            loglevel = "d";
            debugtypes = sargv.substr(2);
            debug_C = (debugtypes.find("C") != string::npos);
            debug_L = (debugtypes.find("L") != string::npos);
            debug_P = (debugtypes.find("P") != string::npos);
            debug_X = (debugtypes.find("X") != string::npos);
            debug_Q = (debugtypes.find("Q") != string::npos);
            debug_M = (debugtypes.find("M") != string::npos);
            debug_q = (debugtypes.find("q") != string::npos);
            debug_I = (debugtypes.find("I") != string::npos);
            debug_R = (debugtypes.find("R") != string::npos);
            debug_o = (debugtypes.find("o") != string::npos);
            debug_m = (debugtypes.find("m") != string::npos);
        } else if  (sargv.substr(0,2) == "-l") {
            loglevel = sargv.substr(2);
        } else {
            filename = sargv;
            original = sargv;
            foundFile = true;
        }
       }

       log.setLevelByName(loglevel);
       log.logmsg4(INFO, CURRENTVERSION, current_version ,"" ,"" ,"");
       if (!b.existsFile(filename)) {
            log.logmsg4(FATAL, UNKNOWNFILE, filename ,"" ,"" ,"");
            return -1;
       }

       string currDirectory = b.getDir(filename);

        // lexer
       bx.readSource(filename, NORMAL);
       lexout = bx.splitContent();

       if (debug_L) {
           for (int i = 0; i < (int) lexout.size(); ++i) {
               log.logmsg4(DEBUG, MSG1, lexout[i] ,"" ,"" ,"");
           }
       }

       if (scope != "l") {
           bp.setSymbols(lexout);
           retflag = bp.doParse();
       };

        if ((retcode < 0) || (!retflag)) {
            // log
        } else if (scope == "x") {
            Boxqueue bq = Boxqueue(bp);
            bq.run(currDirectory, pargs);
        };
   }

   return 0;
}
