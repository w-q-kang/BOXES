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

#include "Boxqueue.h"

Boxqueue::Boxqueue(Boxparse& pboxparse)
{
    if (debug_Q) {log.logmsg4(DEBUG, MSG4, "boxqueue", "start", "", "");}
    boxparse = pboxparse;
    Boxqentry bq = Boxqentry("start");
    bq.setState("new");
    theq.push_back(bq);
}

Boxqueue::~Boxqueue()
{
    //dtor
}

void Boxqueue::add(Boxqentry bqe) {
    theq.push_back(bqe);
};

int Boxqueue::run(string path, vector<string> pargs) {
    int finals = 0;
    if (debug_Q) {log.logmsg4(DEBUG, MSG4, "boxqueue", "runstart", to_string(theq.size()), "");}
    for (int i = 0; i < (int) pargs.size(); ++i) {
        string key = "argument" + to_string(i+1);
        gvalues[key] = pargs[i];
    }
    gvalues["arguments"] = to_string(pargs.size());

    if (debug_Q) {log.logmsg4(DEBUG, MSG4, "gvalues", "runstart", to_string(gvalues.size()), "");}

    currDirectory = path;
    while (theq.size() > 0) {
        if (debug_q) {
            string s;
            for (int i = 0; i < (int) theq.size(); ++i) {
                if (i>0) {s+= "-";}
                 s+= (theq[i]).getName();
            }
            log.logmsg4(DEBUG, MSG4, "boxqueue", "entries:", s, " ");
        }
        Boxqentry bqe = theq[0];
        string name = bqe.getName();
        if (bqe.getState() == "new") {
            // get all relevant boxpointers
            vector<int> refs = boxparse.getQref(name);
            bqe.setState("init");
            bqe.setBoxids(refs);
            if (debug_Q) {log.logmsg4(DEBUG, MSG4, "boxqueue", "setrefs", to_string(refs.size()), " ");}
        }
        int idx = bqe.getNextID();
        theq[0] = bqe;
        if (debug_Q) {log.logmsg4(DEBUG, MSG4, "boxqueue", "get theq[0]", to_string(idx), "");}
        if (idx < 0) {
            theq.pop_front();
            if (debug_Q) {log.logmsg4(DEBUG, MSG4, "boxqueue", "delete entry", " ", " ");}
        } else {
            if (debug_Q) {log.logmsg4(DEBUG, MSG4, "boxqueue", "get pointer", to_string(idx), " ");}
            Box* box = boxparse.getPointer(idx);
            if (debug_Q) {log.logmsg4(DEBUG, MSG4, "box called", box->getInfo(1), "with", name);}
            box->setCurrDir(path);
            box->callResponse(name, theq, bqe, gvalues);
        }
        if ((finals == 0) && (theq.size() == 0)) {
            finals++;
            Boxqentry bq = Boxqentry("final");
            bq.setState("new");
            theq.push_back(bq);
        }
    }
    if (debug_Q) {log.logmsg4(DEBUG, MSG4, "boxqueue", "runend", "", "");}
    return 0;
}

string Boxqueue::getCurrDirectory() {
    return currDirectory;
}
