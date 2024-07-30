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

#ifndef MACHINEFIELD_HXX
#define MACHINEFIELD_HXX
#include "Box.h"
#include "OPG.h"
#include <vector>
#include <map>
#include <deque>
#include "globals.h"
#include "Boxbasics.h"
#include "Scalar.hxx"
#include "Quintuple.hxx"
#include "Mstate.hxx"
#include "Mmoverule.hxx"
#include "Mentryrule.hxx"
#include "Mchange.hxx"

using namespace std;

template <class T>
class MachineField: public Quintuple<T>
{
    public:
    MachineField<T>():
    Quintuple<T>::Quintuple()
    {
        isRefeed = false;
    }

    void init(string pname, string pstname, string pst2name, string pbtname){
        if (debug_I) {Box::log.logmsg4(DEBUG, MSG4, "init", pname, "MachineField", " ");}
        Quintuple<T>::init(pname, pstname, pst2name, pbtname);
        setVarDefault("row");
        setVarDefault("col");
     }

    string resolve(string text, Boxqentry& bqe, map<string,string>& gvalues) {
        if (debug_R) {Box::log.logmsg4(DEBUG, MSG1, "resolve '" + text + "' MachineField int", "", "", "");}
        string s = text;
         if (text == "row") {
            s = this->xtos(row);
        } else if (text == "col") {
            s = this->xtos(col);
        } else if (text == "len") {
            s = this->xtos(field.size());
        } else if (text == "collen") {
            s = this->xtos(collen);
        } else if (text == "rowlen") {
            s = this->xtos(rowlen);
        } else if (text == "fieldn.type") {
            s = this->xtos((field[col-1 + (row-1) * collen]).spec);
        } else if (text == "fieldn.value") {
            s = this->xtos((field[col-1 + (row-1) * collen]).value);
        } else if (text == "#*") {
            Box::log.logmsg4(ERROR, HASHSTARNOTALLOWED,  " ", Box::myclassname, " ", " ");
        } else {
            s = Quintuple<T>::resolve(text, bqe, gvalues);
        }
        if (debug_R) {Box::log.logmsg4(DEBUG, MSG1, "resolve result '" + s + "'", Box::myclassname , "int", "");}
        return s;
    }

    vector<string> resolveAll(vector<string> par, Boxqentry& bqe, map<string,string>& gvalues) {
        vector<string> result;
        if (debug_R) {Box::log.logmsg4(DEBUG, MSG1, "resolveAll '" + Box::bxb.join(par,",") + "'", Box::myclassname, "int", "");}
        for (int i = 0; i < (int) par.size(); ++i) {
            if (par[i] == "field.value") {
               for (int j = 0; j < (int) field.size(); ++j) {
                  result.push_back(this->xtos((field[j]).value));
                }
            } else if (par[i] == "field.type") {
               for (int j = 0; j < (int) field.size(); ++j) {
                  result.push_back(this->xtos((field[j]).spec));
                }
            } else if (par[i] == "#*") {
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
        if (debug_R) {Box::log.logmsg4(DEBUG, MSG1, "resolveAll result '" + Box::bxb.join(result,",") + "'", Box::myclassname, "int", "");}
        return result;
    }

    void setVarFromT(string name, string value) {

    }

    void setVarDefault(string name) {

    }

    void setHlp(string value) {

    }

    bool isSpecialAction(string action) {
        bool result = false;

        if ( (action == "addentryrule") || (action == "addmoverule") || (action == "branch") || (action == "clear") || (action == "impose")
            || (action == "incindex") || (action == "read") || (action == "write")) {
                 result = true;
        }

        return result;
    }

    bool isSpecialActionT(string action) {
        bool result = false;

        if ( (action == "addstates") || (action == "runmachine") ) {
             result = true;
        }

        return result;
    }


    int doSpecialAction(Boxaction ba, deque<Boxqentry>& theq, Boxqentry& thebqe, map<string,string>& gvalues) {
        int result = 1;
        bool doit;
        string action = ba.getName();
        string aug1 = ba.getAug1();
        string aug2 = ba.getAug2();
        string aug3 = ba.getAug3();

        if ((aug1.length() > 2) && (Box::bxb.first(aug1) == '!')) {
            aug1 = resolve(aug1.substr(1), thebqe, gvalues);
        }
        if ((aug2.length() > 2) && (Box::bxb.first(aug2) == '!')) {
            aug2 = resolve(aug2.substr(1), thebqe, gvalues);
        }
        if ((aug3.length() > 2) && (Box::bxb.first(aug3) == '!')) {
            aug3 = resolve(aug3.substr(1), thebqe, gvalues);
        }

        string spec;
        string two;
        string typ = Box::btname;
        vector<string> v;
        string help = "";
        string filename;
        if (action[0] == '!') {
            action = resolve(action.substr(1),thebqe, gvalues);
        }

        if (action == "addmoverule") {
            v = ba.getArguments();
            v = resolveAll(v, thebqe, gvalues);
            string typ = v.at(0);
            string inp = v.at(1);
            string rel = v.at(2);
            string oldval = v.at(3);
            string dir = v.at(4);
            string out = v.at(5);
            Mmoverule<T> m = Mmoverule<T>(typ, inp);
            m.setRule(rel, oldval, dir, out);
            string key = m.key(Box::iosep);
            vector<Mmoverule<T>> vrt;
            if (moverules.find(key) != moverules.end()) {
               vrt = moverules[key];
            }
            vrt.push_back(m);
            moverules[key] = vrt;
        } else if (action == "addentryrule") {
            v = ba.getArguments();
            v = resolveAll(v, thebqe, gvalues);
            string typ = v.at(0);
            string inp = v.at(1);
            string rel = v.at(2);
            string oldval = v.at(3);
            string op = v.at(4);
            string newval = v.at(5);
            Mentryrule<T> e = Mentryrule<T>(typ, inp);
            e.setRule(rel, oldval, op, newval);
            string key = e.key(Box::iosep);
            if (entryrules.find(key) != entryrules.end()) {
                Box::log.logmsg4(ERROR, REDEFENTRYRULE, action, "", "", "");
                result = 0;
            } else {
                // I really do not know why this doesn't work..
                // throws a no matching function for call to ...
                //entryrules[key] = e;
                // use wrapping in a vector as a work around
                vector<Mentryrule<T>> vert;
                vert.push_back(e);
                entryrules[key] = vert;
            }
        } else if (action == "branch") {
           if ((aug1 == "inc") || (aug2 == "inc")) {
                doIncIndex();
            }
            if (Box::condqueue.size() == 0) {
                Box::log.logmsg4(ERROR, CONDQEMPTY, action, "", "", "");
                result = 0;
            } else {
                doit = ((aug1 == "not") || (aug2 == "not")) ? (Box::condqueue[0] == true) : (Box::condqueue[0] == false);
                if (doit) {
                    if (debug_X) {Box::log.logmsg4(DEBUG, MSG4, "processing ", action, " ", " ");}
                    spec = resolve(ba.getArgument(0),thebqe, gvalues);
                    Boxqentry bqe = Boxqentry(spec);
                    vector<string> v = ba.getArgsTail(1);
                    v = resolveAll(v, thebqe, gvalues);
                    bqe.setParams(v);
                    bqe.setState("new");
                    theq.push_back(bqe);
                    result = 0;
                }
            }
            Box::condqueue.pop_front();
        } else if (action == "clear") {
            if (aug1 == "field") {
                field.clear();
            } else if (aug1 == "rules") {
                moverules.clear();
                entryrules.clear();
            }
        }  else if (action == "impose") {
            spec = resolve(ba.getArgument(0), thebqe, gvalues);
            if (debug_X) {Box::log.logmsg4(DEBUG, MSG4, "impose", spec, " ", " ");}
            collen = stoi(spec);
            rowlen = field.size() / collen;
        } else if (action == "incindex") {
            doIncIndex();
        } else if (action == "read") {
            help = resolve(ba.getArgument(0), thebqe, gvalues);
            filename = Box::currDirectory + help;
            doRead(filename);
        } else if ((action == "write") && (aug1 == "vars")) {
            string file = Box::currDirectory + ba.getArgument(0);
            Scalar<T>::doWriteVars(file, thebqe, gvalues);
        } else if (action == "write") {
            help = resolve(ba.getArgument(0), thebqe, gvalues);
            filename = Box::currDirectory + help;
            v.clear();
            v = ba.getArgsTail(1);
            v = resolveAll(v, thebqe, gvalues);
            if (debug_X) {Box::log.logmsg4(DEBUG, MSG4, "lines to be written", to_string(v.size()), " ", " ");}
            bool oneline = false;
            if (aug1 == "line") {oneline = true;}
            int iolen = 0;
            if (Box::bxb.isLong(aug1)) {iolen = stoll(aug1);}
            Box::bxb.writeFile(filename, v, oneline, Box::iosep, iolen);
        } else {
            Box::log.logmsg4(ERROR, UNKNOWNSPECIALACTION, action, "ranges", typ, "");
        }

        return result;
    }

    virtual int doSpecialActionT(Boxaction ba, deque<Boxqentry>& theq, Boxqentry& thebqe, map<string,string>& gvalues) {
        return 1;
    }

    void doRunMachine(string aug1, int startrow, int startcol, vector<string> input) {}

    void doIncIndex() {
        bool check;
        col++;
        if (col > collen) {
            col = 1;
            row++;
        }
        check = (row * collen  <= (int) field.size());

        Box::condqueue.push_front(check);
    }

    void doRead(string filename) {
        vector<string> v;
        vector<string> w;
        v.clear();
        v = Box::bxb.readFile(filename);

        for (int j = 0; j < (int) v.size(); ++j) {
            string help = v[j];
            vector<string> u;
            if (Box::iosep == "") {
                u = Box::bxb.split0(help);
            } else {
                u = Box::bxb.split2(help, Box::iosep[0], Box::iosep[0]);
            }
            Box::bxb.addToVector(w, u);
        }
        int i = 0;
        while (i < (int) w.size()) {
            string typ = w[i];
            i++;
            string val = w[i];
            i++;
            setHlp(val);
            Mstate<T> s = Mstate<T>(typ, hlp);
            field.push_back(s);
        }


    }
    protected:
        vector<Mstate<T>> field;
        map<string, vector<Mmoverule<T>>> moverules;
        map<string, vector<Mentryrule<T>>> entryrules;
        int row;
        int col;
        int len;
        int collen;
        int rowlen;
        T var2;
        T var3;
        T var4;
        T var5;
        T hlp;
        bool isRefeed;
    private:

};

template <> void MachineField<string>::setHlp(string value) {
    hlp = value;
}

template <> void MachineField<long long int>::setHlp(string value) {
    hlp = stoll(value);
}

template <> void MachineField<long double>::setHlp(string value) {
    hlp = stold(value);
}

template <> void MachineField<string>::setVarFromT(string name, string value) {
        if (debug_R) {log.logmsg4(DEBUG, MSG4, "setVarFromT", name, Box::myclassname, "int");}
        if (name == "row") {
            row = stoll(value);
        } else if (name == "col") {
            col = stoll(value);
        } else if (name == "fieldn.type") {
            (field[col-1 + (row-1) * collen]).spec = value;
        } else if (name == "fieldn.value") {
            (field[col-1 + (row-1) * collen]).value = value;
        } else {
            Quintuple<string>::setVarFromT(name, value);
        }
};

template <> void MachineField<long long int>::setVarFromT(string name, string value) {
        if (debug_R) {log.logmsg4(DEBUG, MSG4, "setVarFromT", name, Box::myclassname, "int");}
        if (name == "row") {
            row = stoll(value);
        } else if (name == "col") {
            col = stoll(value);
        } else if (name == "fieldn.type") {
            (field[col-1 + (row-1) * collen]).spec = value;
        } else if (name == "fieldn.value") {
            (field[col-1 + (row-1) * collen]).value = stoll(value);
        } else {
            Quintuple<long long int>::setVarFromT(name, value);
        }
};

template <> void MachineField<long double>::setVarFromT(string name, string value) {
        if (debug_R) {log.logmsg4(DEBUG, MSG4, "setVarFromT", name, Box::myclassname, "float");}
        if (name == "row") {
            row = stoll(value);
        } else if (name == "col") {
            col = stoll(value);
        } else if (name == "fieldn.type") {
            (field[col-1 + (row-1) * collen]).spec = value;
        } else if (name == "fieldn.value") {
            (field[col-1 + (row-1) * collen]).value = stold(value);
        } else {
            Quintuple<long double>::setVarFromT(name, value);
        }
};

template <> void MachineField<string>::setVarDefault(string name) {
        if (debug_I) {log.logmsg4(DEBUG, MSG4, "setVarDefault", name, Box::myclassname, "string");}

        if (name == "row") {
            row = 0;
        } else if (name == "col") {
            col = 0;
        } else {
            Quintuple<string>::setVarDefault(name);
        }
    }

template <> void MachineField<long long int>::setVarDefault(string name) {
        if (debug_I) {log.logmsg4(DEBUG, MSG4, "setVarDefault", name, Box::myclassname, "int");}
        if (name == "row") {
            row = 0;
        } else if (name == "col") {
            col = 0;
        } else {
            Quintuple<long long int>::setVarDefault(name);
        }
    }

template <> void MachineField<long double>::setVarDefault(string name) {
        if (debug_I) {log.logmsg4(DEBUG, MSG4, "setVarDefault", name, Box::myclassname, "float");}
        if (name == "row") {
            row = 0;
        } else if (name == "col") {
            col = 0;
        } else {
            Quintuple<long double>::setVarDefault(name);
        }
    }

 template <> void MachineField<string>::doRunMachine(string aug1, int startrow, int startcol, vector<string> input) {
    deque<Mchange<string>> newtok;
    int ix = 0;
    int currrow = startrow;
    int currcol = startcol;
    string currinput = input.at(ix);
    bool goon = true;

     while (goon) {
        Mstate<string> m = field.at(currcol - 1 + (currrow - 1) * collen);
        string key = m.spec + Box::iosep + currinput;
        if (debug_m) {log.logmsg4(DEBUG, MSG4, "processing field element", to_string(currrow), to_string(currcol), currinput);}
        string newvalue  = "";
        Optype op = OTSAME;
        if (entryrules.find(key) != entryrules.end()) {
            Mentryrule<string> merl = (entryrules[key])[0];
            newvalue = merl.newvalue;
            op = merl.op;
        }

        if (moverules.find(key) != moverules.end()) {
           vector<Mmoverule<string>> vrt = moverules[key];
           if (debug_m) {log.logmsg4(DEBUG, MSG4, "rule size is", to_string(vrt.size()), "", "");};
           for (int i = 0; i < (int) vrt.size(); ++i) {
                Mmoverule<string> mrl = vrt[i];
                if (mrl.isvalid(m.spec, currinput, m.value)) {
                    bool isvalid = true;
                    int nextcol = currcol;
                    int nextrow = currrow;
                    if (mrl.direction == "L2R") {
                        nextcol++;
                        isvalid = nextcol <= collen;
                    } else if (mrl.direction == "R2L") {
                        nextcol--;
                        isvalid = nextcol >= 1;
                    } else if (mrl.direction == "A2B") {
                        nextrow++;
                        isvalid = nextrow <= rowlen;
                    } else if (mrl.direction == "B2A") {
                        nextrow--;
                        isvalid = nextrow >= 1;
                    }
                    if (isvalid) {
                        Mchange<string> mc = Mchange<string>(currrow, currcol, mrl.output);
                        //if (debug_m) {log.logmsg4(DEBUG, MSG4, "set field change as", to_string(currrow), to_string(currcol), mrl.output);}
                        mc.setNewpos(nextrow, nextcol);
                        mc.setIndex(ix+1);
                        newtok.push_back(mc);
                        if (debug_m) {log.logmsg4(DEBUG, MSG4, "added move for", to_string(currrow)  + "," + to_string(currcol), "->", to_string(nextrow)  + "," + to_string(nextcol));}
                    } else {
                        if (debug_m) {log.logmsg4(DEBUG, MSG4, "move invalid ", to_string(currrow)  + "," + to_string(currcol), "->", to_string(nextrow)  + "," + to_string(nextcol));}
                    }
                } else {
                    if (debug_m) {log.logmsg4(DEBUG, MSG4, "rule invalid ", to_string(currrow)  + "," + to_string(currcol), currinput, m.value);}

                }
           }
        } else {
            if (debug_m) {log.logmsg4(DEBUG, MSG4, "no move rules for ", to_string(currrow)  + "," + to_string(currcol), currinput, m.value);}
        }

        // update old field element
        Mstate<string> m2 = field.at(currcol - 1 + (currrow - 1) * collen);
        m2.update(op, newvalue);
        field[currcol - 1 + (currrow - 1) * collen] = m2;
        if (debug_m) {log.logmsg4(DEBUG, MSG4, "updated field element", to_string(currrow), to_string(currcol), m2.value);}

        if (newtok.size() == 0) {break;}
        // select first change
        Mchange<string> curr = newtok[0];

        // next element
        currrow = curr.newrow;
        currcol = curr.newcol;
        if (aug1 == "refeed") {
            currinput = curr.output;
        } else {
            ix = curr.index;
            currinput = input.at(ix);
        }
        newtok.pop_front();
    }
 }

template <> void MachineField<long long int>::doRunMachine(string aug, int startrow, int startcol, vector<string> input) {

}

template <> void MachineField<long double>::doRunMachine(string aug, int startrow, int startcol, vector<string> input) {

}

template <> int MachineField<string>::doSpecialActionT(Boxaction ba, deque<Boxqentry>& theq, Boxqentry& thebqe, map<string,string>& gvalues) {
        int result = 1;
        string action = ba.getName();
        string aug1 = ba.getAug1();
        string aug2 = ba.getAug2();
        string aug3 = ba.getAug3();
        string spec;
        string typ = "int";
        vector<string> v;
        string help = "";

        if (action[0] == '!') {
            action = resolve(action.substr(1),thebqe, gvalues);
        }

        if (action == "addstates") {
            v = ba.getArguments();
            v = resolveAll(v, thebqe, gvalues);
            int i = 0;
            while (i < (int) v.size()) {
                string typ = v[i];
                i++;
                string val = v[i];
                i++;
                Mstate<string> s = Mstate<string>(typ, val);
                field.push_back(s);
            }
        } else if (action == "runmachine") {
            string startrow = resolve(ba.getArgument(0),thebqe, gvalues);
            string startcol = resolve(ba.getArgument(1),thebqe, gvalues);
            v = ba.getArgsTail(2);
            v = resolveAll(v, thebqe, gvalues);
            doRunMachine(aug1, stoi(startrow), stoi(startcol), v);
        } else {
            Box::log.logmsg4(ERROR, UNKNOWNSPECIALACTION, action, Box::myclassname, typ, "");
        }

        return result;
    }


template <> int MachineField<long long int>::doSpecialActionT(Boxaction ba, deque<Boxqentry>& theq, Boxqentry& thebqe, map<string,string>& gvalues) {
        int result = 1;
        string action = ba.getName();
        string aug1 = ba.getAug1();
        string aug2 = ba.getAug2();
        string aug3 = ba.getAug3();
        string spec;
        string typ = "int";
        vector<string> v;
        string help = "";

        if (action[0] == '!') {
            action = resolve(action.substr(1),thebqe, gvalues);
        }

        if (action == "addstates") {
            v = ba.getArguments();
            v = resolveAll(v, thebqe, gvalues);
            int i = 0;
            while (i < (int) v.size()) {
                string typ = v[i];
                i++;
                string val = v[i];
                i++;
                Mstate<long long int> s = Mstate<long long int>(typ, stoll(val));
                field.push_back(s);
            }
        } else if (action == "runmachine") {
            string startrow = resolve(ba.getArgument(0),thebqe, gvalues);
            string startcol = resolve(ba.getArgument(1),thebqe, gvalues);
            v = ba.getArgsTail(2);
            v = resolveAll(v, thebqe, gvalues);
            doRunMachine(aug1, stoi(startrow), stoi(startcol), v);
        } else {
            Box::log.logmsg4(ERROR, UNKNOWNSPECIALACTION, action, Box::myclassname, typ, "");
        }

        return result;
    }

template <> int MachineField<long double>::doSpecialActionT(Boxaction ba, deque<Boxqentry>& theq, Boxqentry& thebqe, map<string,string>& gvalues) {
        int result = 1;
        string action = ba.getName();
        string aug1 = ba.getAug1();
        string aug2 = ba.getAug2();
        string aug3 = ba.getAug3();
        string spec;
        string typ = "int";
        vector<string> v;
        string help = "";

        if (action[0] == '!') {
            action = resolve(action.substr(1),thebqe, gvalues);
        }

        if (action == "addstates") {
            v = ba.getArguments();
            v = resolveAll(v, thebqe, gvalues);
            int i = 0;
            while (i < (int) v.size()) {
                string typ = v[i];
                i++;
                string val = v[i];
                i++;
                Mstate<long double> s = Mstate<long double>(typ, stold(val));
                field.push_back(s);
            }
        } else if (action == "runmachine") {
            string startrow = resolve(ba.getArgument(0),thebqe, gvalues);
            string startcol = resolve(ba.getArgument(1),thebqe, gvalues);
            v = ba.getArgsTail(2);
            v = resolveAll(v, thebqe, gvalues);
            doRunMachine(aug1, stoi(startrow), stoi(startcol), v);
        } else {
            Box::log.logmsg4(ERROR, UNKNOWNSPECIALACTION, action, Box::myclassname, typ, "");
        }

        return result;
    }



#endif // MACHINEFIELD_HXX
