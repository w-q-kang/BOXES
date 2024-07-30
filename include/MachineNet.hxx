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

#ifndef MACHINENET_HXX
#define MACHINENET_HXX
#include "Box.h"
#include "OPG.h"
#include <vector>
#include <map>
#include <deque>
#include "globals.h"
#include "Boxbasics.h"
#include "Quintuple.hxx"
#include "Mstate.hxx"
#include "Netrule.hxx"

using namespace std;

template <class T>
class MachineNet: public Quintuple<T>
{
    public:
    MachineNet<T>():
    Quintuple<T>::Quintuple()
    {

    }

    void init(string pname, string pstname, string pst2name, string pbtname){
        if (debug_I) {Box::log.logmsg4(DEBUG, MSG4, "init", pname, Box::myclassname, " ");}
        Quintuple<T>::init(pname, pstname, pst2name, pbtname);
     }

    string resolve(string text, Boxqentry& bqe, map<string,string>& gvalues) {
        if (debug_R) {Box::log.logmsg4(DEBUG, MSG4, "resolve '" + text + "'",  Box::myclassname, "int", "");}
        string s = text;
        string x1 = "";
        if (text == "len") {
            s = this->xtos(states.size());
        } else if (Box::bxb.startsWith(text, "svalue.")) {
            x1 = text.substr(7);
            auto a =states.at(x1);
            setHlpT(a.value);
            s = this->xtos(hlp);
        } else if (text == "#*") {
            Box::log.logmsg4(ERROR, HASHSTARNOTALLOWED,  " ", Box::myclassname, " ", " ");
        } else {
            s = Quintuple<T>::resolve(text, bqe, gvalues);
        }
        if (debug_R) {Box::log.logmsg4(DEBUG, MSG4, "resolve result '" + s + "'", "",  "int", "");}
        return s;
    }

    vector<string> resolveAll(vector<string> par, Boxqentry& bqe, map<string,string>& gvalues) {
        vector<string> result;
        if (debug_R) {Box::log.logmsg4(DEBUG, MSG4, "resolveAll '" + Box::bxb.join(par,",") + "'", Box::myclassname, "int", "");}
        for (int i = 0; i < (int) par.size(); ++i) {
            if (par[i] == "states.name") {
               for (auto it=states.begin(); it!=states.end(); ++it) {
                    result.push_back(it->first);
                }
            } else if (par[i] == "states.value") {
               for (auto it=states.begin(); it!=states.end(); ++it) {
                  setHlpT((it->second).value);
                  result.push_back(this->xtos(hlp));
                }
            } else if (par[i] == "output") {
                for (int j = 0; j < (int) output.size(); ++j) {
                    result.push_back(output[j]);
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
        if (debug_R) {Box::log.logmsg4(DEBUG, MSG1, "resolveAll result '" + Box::bxb.join(result,",") + "' MachineField int", "", "", "");}
        return result;
    }


    string resolveRuleInput(string text, vector<string> input) {
        string result = text;
        if (Box::bxb.startsWith(text, "input.")) {
            string x1 = text.substr(6);
            int ix = stoi(x1) - 1;
            result = input.at(ix);
        }
        return result;
    }
    void setVarFromT(string name, string value) {

    }

    void setVarDefault(string name) {

    }

    void setHlp(string value) {

    }

    void setHlpT(T value) {
    }

    bool isSpecialAction(string action) {
        bool result = false;

        if ( (action == "addrule") || (action == "clear") || (action == "write")) {
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
        string name = "";
        string input = "";
        string rel = "";
        string oldval = "";
        string newstate = "";
        string op = "";
        string newvalue = "";
        string out = "";
        string filename = "";
        if (action[0] == '!') {
            action = resolve(action.substr(1),thebqe, gvalues);
        }

        if (action == "addrule") {
            v = ba.getArguments();
            v = resolveAll(v, thebqe, gvalues);
            Netrule<T> m = Netrule<T>();
            if (aug1 == "else") {
                name = v.at(0);
                newstate = v.at(1);
                op = v.at(2);
                newvalue = v.at(3);
                if (v.size() >= 5) {out = v.at(4);}
                m = Netrule<T>(name, newstate);
                m.setElserule(op, newvalue,out);
            } else {
                name = v.at(0);
                input = v.at(1);
                rel = v.at(2);
                oldval = v.at(3);
                newstate = v.at(4);
                op = v.at(5);
                newvalue = v.at(6);
                if (v.size() >= 8) {out = v.at(7);}
                m = Netrule<T>(name, newstate);
                m.setRule(input, rel, oldval, op, newvalue, out);
            }
            if (debug_m) {Box::log.logmsg4(DEBUG, MSG4, "set rule " +  name + " " + input + " " + rel +  " " + oldval, newstate, op + " " + newvalue, out);}
            string key = m.key(Box::iosep);
            vector<Netrule<T>> vrt;
            if (netrules.find(key) != netrules.end()) {
               vrt = netrules[key];
            }
            vrt.push_back(m);
            netrules[key] = vrt;
        } else if (action == "clear") {
            if (aug1 == "states") {
                states.clear();
            } else if (aug1 == "rules") {
                netrules.clear();
            } else if (aug1 == "output") {
                output.clear();
            }
        } else if ((action == "write") && (aug1 == "vars")) {
            help = resolve(ba.getArgument(0), thebqe, gvalues);
            filename = Box::currDirectory + help;
            Scalar<T>::doWriteVars(filename, thebqe, gvalues);
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
            Box::log.logmsg4(ERROR, UNKNOWNSPECIALACTION, action, "machine net", typ, "");
        }

        return result;
    }

    virtual int doSpecialActionT(Boxaction ba, deque<Boxqentry>& theq, Boxqentry& thebqe, map<string,string>& gvalues) {
        return 1;
    }

    void doRunMachine(string startstate, vector<string>& input, Boxqentry& bqe, map<string,string>& gvalues) {}

    protected:
        map<string, Mstate<T>> states;
        map<string, vector<Netrule<T>>> netrules;
        int len;
        T var2;
        T var3;
        T var4;
        T var5;
        T hlp;
        vector<string> output;
        vector<string> input;
    private:

};

template <> void MachineNet<string>::setHlp(string value) {
    hlp = value;
}

template <> void MachineNet<long long int>::setHlp(string value) {
    hlp = stoll(value);
}

template <> void MachineNet<long double>::setHlp(string value) {
    hlp = stold(value);
}

template <> void MachineNet<string>::setHlpT(string value) {
    hlp = value;
}

template <> void MachineNet<long long int>::setHlpT(long long int value) {
    hlp = value;
}

template <> void MachineNet<long double>::setHlpT(long double value) {
    hlp = value;
}

template <> void MachineNet<string>::setVarFromT(string name, string value) {
        if (debug_R) {log.logmsg4(DEBUG, MSG4, "setVarFromT", name, "v", "int");}
        Quintuple<string>::setVarFromT(name, value);
};

template <> void MachineNet<long long int>::setVarFromT(string name, string value) {
        if (debug_R) {log.logmsg4(DEBUG, MSG4, "setVarFromT", name, "v", "int");}
        Quintuple<long long int>::setVarFromT(name, value);
};

template <> void MachineNet<long double>::setVarFromT(string name, string value) {
        if (debug_R) {log.logmsg4(DEBUG, MSG4, "setVarFromT", name, "v", "int");}
        Quintuple<long double>::setVarFromT(name, value);
};

template <> void MachineNet<string>::setVarDefault(string name) {
        if (debug_I) {log.logmsg4(DEBUG, MSG4, "setVarDefault", name, "MachineField", "string");}
        Quintuple<string>::setVarDefault(name);
    }

template <> void MachineNet<long long int>::setVarDefault(string name) {
        if (debug_I) {log.logmsg4(DEBUG, MSG4, "setVarDefault", name, "MachineField", "int");}
        Quintuple<long long int>::setVarDefault(name);
    }

template <> void MachineNet<long double>::setVarDefault(string name) {
        if (debug_I) {log.logmsg4(DEBUG, MSG4, "setVarDefault", name, "MachineField", "float");}
        Quintuple<long double>::setVarDefault(name);
    }

 template <> void MachineNet<string>::doRunMachine(string startstate, vector<string>& input, Boxqentry& bqe, map<string,string>& gvalues) {
     string current = startstate;
     bool goon = true;
     while (goon) {
        goon = false;
        if (debug_m) {Box::log.logmsg4(DEBUG, MSG4, "current state is ", current, "", "");}
        Mstate<string> ms = states.at(current);
        if (netrules.find(current) == netrules.end()) {
            break;
        }
        vector<Netrule<string>> r = netrules.at(current);
        if (debug_m) {Box::log.logmsg4(DEBUG, MSG4, "found ", to_string(r.size()), "rules", "");}
        for (int i = 0; i < (int) r.size(); ++i) {
            if (debug_m) {Box::log.logmsg4(DEBUG, MSG4, "processing ", "rule", to_string(i), "");}
            Netrule nr = r[i];
            string inp = resolveRuleInput(nr.input, input);
             if (nr.isvalid(current, inp)) {
                // update output
                if (!nr.noOutput) {
                    string out = resolveRuleInput(nr.output, input);
                    output.push_back(out);
                }
                // switch to new state
                current = nr.newstate;
                Mstate<string> ns = states.at(current);
                // update new state on entry
                ns.update(nr.op, nr.newvalue);
                states[current] = ns;
                // process next state with current input
                goon = true;
                // stop after first rule
                break;
            }
        }
     }
}


template <> void MachineNet<long long int>::doRunMachine(string startstate, vector<string>& input, Boxqentry& bqe, map<string,string>& gvalues) {
     string current = startstate;
     if (debug_m) {Box::log.logmsg4(DEBUG, MSG4, "starting machine with ", startstate, Box::bxb.join(input, ","), "");}
     bool goon = true;
     while (goon) {
        goon = false;
        if (debug_m) {Box::log.logmsg4(DEBUG, MSG4, "current state is ", current, "", "");}
        Mstate<long long int> ms = states.at(current);
        if (netrules.find(current) == netrules.end()) {
            break;
        }
        vector<Netrule<long long int>> r = netrules.at(current);
        if (debug_m) {Box::log.logmsg4(DEBUG, MSG4, "found ", to_string(r.size()), "rules", "");}
        for (int i = 0; i < (int) r.size(); ++i) {
            if (debug_m) {Box::log.logmsg4(DEBUG, MSG4, "processing ", "rule", to_string(i), "");}
            Netrule nr = r[i];
            string inp = resolveRuleInput(nr.input, input);
            if (nr.isvalid(current, inp)) {
                // update output
                if (!nr.noOutput) {
                    string out = resolveRuleInput(nr.output, input);
                    if (debug_m) {Box::log.logmsg4(DEBUG, MSG4, "adding " + nr.output + "=", out, "to output", "");}
                    output.push_back(out);
                }
                // switch to new state
                current = nr.newstate;
                Mstate<long long int> ns = states.at(current);
                // update new state on entry
                ns.update(nr.op, nr.newvalue);
                states[current] = ns;
                // process next state with current input
                goon = true;
                // stop after first rule
                break;
            }
        }
     }
}

template <> void MachineNet<long double>::doRunMachine(string startstate, vector<string>& input, Boxqentry& bqe, map<string,string>& gvalues) {
     string current = startstate;
     bool goon = true;
     while (goon) {
        goon = false;
        if (debug_m) {Box::log.logmsg4(DEBUG, MSG4, "current state is ", current, "", "");}
        Mstate<long double> ms = states.at(current);
        if (netrules.find(current) == netrules.end()) {
            break;
        }
        vector<Netrule<long double>> r = netrules.at(current);
        if (debug_m) {Box::log.logmsg4(DEBUG, MSG4, "found ", to_string(r.size()), "rules", "");}
        for (int i = 0; i < (int) r.size(); ++i) {
            if (debug_m) {Box::log.logmsg4(DEBUG, MSG4, "processing ", "rule", to_string(i), "");}
            Netrule nr = r[i];
            string inp = resolveRuleInput(nr.input, input);
            if (nr.isvalid(current, inp)) {
                // update output
                if (!nr.noOutput) {
                    string out = resolveRuleInput(nr.output, input);
                    output.push_back(out);
                }
                // switch to new state
                current = nr.newstate;
                Mstate<long double> ns = states.at(current);
                // update new state on entry
                ns.update(nr.op, nr.newvalue);
                states[current] = ns;
                // process next state with current input
                goon = true;
                // stop after first rule
                break;
            }
        }
     }
}

template <> int MachineNet<string>::doSpecialActionT(Boxaction ba, deque<Boxqentry>& theq, Boxqentry& thebqe, map<string,string>& gvalues) {
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
                string name = v[i];
                i++;
                string value = v[i];
                i++;
                Mstate<string> s = Mstate<string>(name, value);
                states[name] = s;
            }
        } else if (action == "runmachine") {
            string startstate = resolve(ba.getArgument(0),thebqe, gvalues);
            v = ba.getArgsTail(1);
            v = resolveAll(v, thebqe, gvalues);
            if (debug_m) {Box::log.logmsg4(DEBUG, MSG4, "startstate ", startstate, "args", to_string(v.size()));}
            doRunMachine(startstate, v, thebqe, gvalues);
        } else {
            Box::log.logmsg4(ERROR, UNKNOWNSPECIALACTION, action, "Machine Net", typ, "");
        }

        return result;
    }


template <> int MachineNet<long long int>::doSpecialActionT(Boxaction ba, deque<Boxqentry>& theq, Boxqentry& thebqe, map<string,string>& gvalues) {
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
                string name = v[i];
                i++;
                string value = v[i];
                i++;
                Mstate<long long int> s = Mstate<long long int>(name, stoll(value));
                states[name] = s;
            }
        } else if (action == "runmachine") {
            string s = resolve(ba.getArgument(0),thebqe, gvalues);
            v = ba.getArgsTail(1);
            v = resolveAll(v, thebqe, gvalues);
            if (debug_m) {Box::log.logmsg4(DEBUG, MSG4, "startstate ", s, "args", to_string(v.size()));}
            doRunMachine(s, v, thebqe, gvalues);
        } else {
            Box::log.logmsg4(ERROR, UNKNOWNSPECIALACTION, action, "Machine Net", typ, "");
        }

        return result;
    }

template <> int MachineNet<long double>::doSpecialActionT(Boxaction ba, deque<Boxqentry>& theq, Boxqentry& thebqe, map<string,string>& gvalues) {
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
                string name = v[i];
                i++;
                string value = v[i];
                i++;
                Mstate<long double> s = Mstate<long double>(name, stold(value));
                states[name] = s;
            }
        } else if (action == "runmachine") {
            string startstate = resolve(ba.getArgument(0),thebqe, gvalues);
            v = ba.getArgsTail(1);
            v = resolveAll(v, thebqe, gvalues);
            if (debug_m) {Box::log.logmsg4(DEBUG, MSG4, "startstate ", startstate, "args", to_string(v.size()));}
            doRunMachine(startstate, v, thebqe, gvalues);
        } else {
            Box::log.logmsg4(ERROR, UNKNOWNSPECIALACTION, action, "Machine Net", typ, "");
        }

        return result;
    }



#endif // MACHINENET_HXX
