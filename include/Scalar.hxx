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

#ifndef SCALAR_HXX
#define SCALAR_HXX
#include "Box.h"
#include "OPG.h"
#include <vector>
#include <filesystem>
#include "globals.h"
#include "Boxbasics.h"

using namespace std;

template <class T>
class Scalar: public Box
{
    public:
    Scalar()
    :Box()
    {
    }

    virtual void init(string pname, string pstname, string pst2name, string pbtname) {
        if (debug_I) {log.logmsg4(DEBUG, MSG4, "init", pname, "Scalar", " ");}
        Box::init(pname, pstname, pst2name, pbtname);
        setVarDefault("var");
    }
    int callResponse(string s, deque<Boxqentry>& theq, Boxqentry& thebqe, map<string,string>& gvalues) {
        int outerResult = 0;
        int result = 1;
        if (debug_M) {
            vector<string> pars = (thebqe.getParams());
            log.logmsg4(DEBUG, MSG4, "callResponse", s, "with params", bxb.join(pars, ","));}
        vector<Boxaction> actions = responses[s];
        int i = 0;
        bool isRepeated = false;
        while (i < (int) actions.size()) {
            Boxaction a = actions[i];
            result = doCall(a, theq, thebqe, isRepeated, gvalues);
            if (result == 0) {
                if (debug_X) {log.logmsg4(DEBUG, MSG4, "result = 0", "break", " ", " ");}
                break;
            } else if (result == -1) {
                i = -1;
                isRepeated = true;
            } else if (result == -2) {
                outerResult = -2;
                break;
            }
            ++i;
        }
        if (debug_M) {log.logmsg4(DEBUG, MSG4, "finished callResponse", s, " ",  " ");}

        return outerResult;
    }

    // return codes:
    // 1 = ok, 0 = break,unfulfilled if/ifnot, -1 = repeat, -2 = break.next
    int doCall(Boxaction ba, deque<Boxqentry>& theq, Boxqentry& thebqe, bool isRepeated, map<string,string>& gvalues) {
        int result = 1;
        bool doit;
        string action = ba.getName();
        string aug1 = ba.getAug1();
        string aug2 = ba.getAug2();
        string aug3 = ba.getAug3();

        if (ba.getIf()) {
            if (condqueue.size() == 0) {
                log.logmsg4(ERROR, CONDQEMPTY, action, " ", " ", " ");
                return 0;
            } else if (!condqueue[0]) {
                return 1;
            }
        } else if (ba.getIfnot()) {
            if (condqueue.size() == 0) {
                log.logmsg4(ERROR, CONDQEMPTY, action, " ", " ", " ");
                return 0;
            } else if (condqueue[0]) {
                return 1;
            }
        }

        if ((aug1.length() > 2) && (bxb.first(aug1) == '!')) {
            aug1 = resolve(aug1.substr(1), thebqe, gvalues);
        }
        if ((aug2.length() > 2) && (bxb.first(aug2) == '!')) {
            aug2 = resolve(aug2.substr(1), thebqe, gvalues);
        }
        if ((aug3.length() > 2) && (bxb.first(aug3) == '!')) {
            aug3 = resolve(aug3.substr(1), thebqe, gvalues);
        }
        string spec;
        string two;
        string typ = btname;
        string help = "";
        string filename = "";
        int ihelp = 0;
        if (debug_X) {
            spec = " ";
            if (ba.hasArguments()) {spec = ba.getArgument(0);}
            log.logmsg4(DEBUG, MSG4, "processing ", action, spec, " ");
        }
        if (action[0] == '!') {
            action = resolve(action.substr(1),thebqe, gvalues);
        }

        if (isSpecialAction(action)) {
            result = doSpecialAction(ba, theq, thebqe, gvalues);
        } else if (isSpecialActionT(action)) {
            result = doSpecialActionT(ba, theq, thebqe, gvalues);
        } else if (action == "branch") {
            if (condqueue.size() == 0) {
                log.logmsg4(ERROR, CONDQEMPTY, action, "", "", "");
                result = 0;
            } else {
                doit = (aug1 == "not") ? (condqueue[0] == true) : (condqueue[0] == false);
                if (doit) {
                    if (debug_X) {log.logmsg4(DEBUG, MSG4, "processing ", action, " ", " ");}
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
            condqueue.pop_front();
        } else if (action == "break") {
            if (aug1 == "next") {
                result = -2;
            } else {
                result = 0;
            }
        } else if (action == "cond") {
            vector<string> v = resolveAll(ba.getArguments(), thebqe, gvalues);
            if (aug1=="int") {typ = "int";}
            if (aug1=="float") {typ = "float";}
            if (aug1=="string") {typ = "string";}
            if (aug1=="mixed") {typ = "mixed";}
            help = opg.run(v, typ);

            if (typ == "float") {
                long double ld = stold(help);
                if (ld == 0L) {
                    condqueue.push_front(false);
                } else {
                    condqueue.push_front(true);
                }
            } else if (help == "0") {
                condqueue.push_front(false);
            } else {
                condqueue.push_front(true);
            }
            if (debug_X) {log.logmsg4(DEBUG, MSG4, "cond result '" + help + "'", bxb.join(v, " "), "condlevel:", to_string(condqueue.size()));}
        } else if (action == "decr") {
            spec = ba.getArgument(0);
            help = resolve(spec, thebqe, gvalues);
            help = to_string(stoll(help) - 1);
            setVarFromT(spec, help);
        } else if (action == "do") {
            spec = resolve(ba.getArgument(0),thebqe, gvalues);
            if (debug_X) {log.logmsg4(DEBUG, MSG4, "doing ", spec, " ", " ");}
            Boxqentry localbqe = Boxqentry(spec);
            vector<string> v = ba.getArgsTail(1);
            v = resolveAll(v, thebqe, gvalues);
            localbqe.setParams(v);
            localbqe.setState("local");
            int response = callResponse(spec, theq, localbqe, gvalues);
            if (debug_X) {log.logmsg4(DEBUG, MSG4, " >>>finished doing ", spec, " ", " ");}
            if (response == -2) {result = 0;}
        } else if (action == "dir") {
            spec = resolve(ba.getArgument(0),thebqe, gvalues);
            result = doFile(spec, aug1 + "dir");
        } else if (action == "exists") {
            spec = resolve(ba.getArgument(0),thebqe, gvalues);
            if (gvalues.find(spec) == gvalues.end()) {
                condqueue.push_back(false);
            } else {
                condqueue.push_back(true);
            }
        }else if (action == "file") {
            spec = resolve(ba.getArgument(0),thebqe, gvalues);
            result = doFile(spec, aug1);
        } else if (action == "finis") {
            result = doFinis();
        } else if (action == "if") {
            if (condqueue.size() == 0) {
                log.logmsg4(ERROR, CONDQEMPTY, action, "", "", "");
                result = 0;
            } else {
                if (condqueue[0] == false) {result = 0;}
                condqueue.pop_front();
            }
        } else if (action == "ifnot") {
            if (condqueue.size() == 0) {
                log.logmsg4(ERROR, CONDQEMPTY, action, "", "", "");
                result = 0;
            } else {
                if (condqueue[0] == true) {result = 0;}
                condqueue.pop_front();
            }
        } else if (action == "incr") {
            spec = ba.getArgument(0);
            help = resolve(spec, thebqe, gvalues);
            help = to_string(stoll(help) + 1);
            setVarFromT(spec, help);
        } else if (action == "init") {
            if (!isRepeated) {
                for (int i = 0; i < ba.argsize(); i+=2) {
                    spec = ba.getArgument(i);
                    help = resolve(ba.getArgument(i+1), thebqe, gvalues);
                    setVarFromT(spec, help);
                }
            }
        } else if (action == "input") {
            spec = ba.getArgument(0);
            cout << "?";
            if (aug1 == "line") {
                getline(cin, help);
            } else {
                cin >> help;
            }
            setVarFromT(spec, help);
        } else if (action == "print") {
            result = doPrint(ba.getArguments(), thebqe, gvalues, aug1 == "nl");
        } else if (action == "readline") {
            filename = resolve(ba.getArgument(0),thebqe, gvalues);
            string file = currDirectory + filename;
            spec = ba.getArgument(1);
            ihelp = bxb.readFileLine(file, fsl, help);
            if (ihelp == 1) {
                setVarFromT(spec, help);
            }
            condqueue.push_front(ihelp);
        } else if (action == "readvars") {
            filename = resolve(ba.getArgument(0),thebqe, gvalues);
            string file = currDirectory + filename;
            doReadVars(file);
        }else if (action == "repeat") {
            result = -1;
        } else if (action == "save") {
            spec = resolve(ba.getArgument(0),thebqe, gvalues);

            if (ba.argsize() == 2) {
               // dont call OPG if not necessary
               help = resolve(ba.getArgument(1), thebqe, gvalues);
            } else {
                help = opg.run(resolveAll(ba.getArgsTail(1), thebqe, gvalues), "mixed");
            }
            gvalues[spec]= help;
            if (debug_X) {Box::log.logmsg4(DEBUG, MSG4, "saving", spec, "=", help);}
        } else if (action == "sep") {
            spec = resolve(ba.getArgument(0),thebqe, gvalues);
            doSep(spec);
        } else if (action == "send") {
            spec = resolve(ba.getArgument(0),thebqe, gvalues);
            if (debug_X) {log.logmsg4(DEBUG, MSG4, "sending ", spec, "to", "queue");}
            Boxqentry bqe = Boxqentry(spec);
            vector<string> v = ba.getArgsTail(1);
            v = resolveAll(v, thebqe, gvalues);
            bqe.setParams(v);
            bqe.setState("new");
            theq.push_back(bqe);
         } else if (action == "set") {
            string spec = ba.getArgument(0);

            if (ba.argsize() == 2) {
               // dont call OPG if not necessary
               help = resolve(ba.getArgument(1), thebqe, gvalues);
               setVarFromT(spec, help);
            } else {
                if (bxb.contains(spec, varints) || (aug1=="int")) {
                    typ = "int";
                } else if (aug1== "float") {
                    typ = "float";
                } else if (aug1=="string") {
                    typ = "string";
                } else if (aug1=="mixed") {
                    typ = "mixed";
                }
                help = opg.run(resolveAll(ba.getArgsTail(1), thebqe, gvalues), typ);
                setVarFromT(spec, help);
            }
            if (debug_X) {log.logmsg4(DEBUG, MSG4, "set result",  help, " ", " ");}

        } else if (action == "stop") {
            result = doExit();
        } else if (action == "sl") {
            spec = ba.getArgument(0);
            vector<string> v = ba.getArgsTail(1);
            v = resolveAll(v, thebqe, gvalues);
            doSl(spec, v, thebqe, gvalues);
        } else if (action == "sr") {
            spec = ba.getArgument(0);
            help = resolve(ba.getArgument(1), thebqe, gvalues);
            doSr(spec, help, aug1, thebqe, gvalues);
        }else if (action == "rv") {
            spec = ba.getArgument(0);
            doRv(spec, thebqe, gvalues);
        } else if (action == "tl") {
            spec = ba.getArgument(0);
            vector<string> v = ba.getArgsTail(1);
            v = resolveAll(v, thebqe, gvalues);
            doTl(spec, v, aug1, thebqe, gvalues);
        } else if (action == "translate") {
            spec = ba.getArgument(0);
            vector<string> v = ba.getArgsTail(1);
            v = resolveAll(v, thebqe, gvalues);
            doTranslate(spec, v, thebqe, gvalues);
        } else if ((action == "write") && (aug1 == "vars")) {
            help = resolve(ba.getArgument(0), thebqe, gvalues);
            filename = currDirectory + help;
            doWriteVars(filename, thebqe, gvalues);
        } else if (action == "write") {
            help = resolve(ba.getArgument(0), thebqe, gvalues);
            filename = currDirectory + help;
            vector<string> v = ba.getArgsTail(1);
            v = resolveAll(v, thebqe, gvalues);
            bool oneline = false;
            if (aug1 == "line") {oneline = true;}
            int iolen = 0;
            if (bxb.isLong(aug1)) {iolen = stoll(aug1);}
            bxb.writeFile(filename, v, oneline, iosep, iolen);
        } else {
            log.logmsg4(ERROR, UNKNOWNACTION, action, "", "", "");
        }

        return result;
    }

    int doPrint(vector<string> v, Boxqentry& bqe, map<string,string>& gvalues, bool hasNl) {
        bool suppress = false;
        string s;
        vector<string> w = resolveAll(v, bqe, gvalues);
        int len = w.size();
        for (int i = 0; i < len; ++i) {
            s = bxb.format(w[i]);
            if (bxb.first(s) == '\n') {suppress = true;}
            if ((i>0) && !suppress) {printf(" ");}
            printf("%s", s.c_str());
            suppress = false;
            if (bxb.last(s) == '\n') {suppress = true;}
        }
        if (hasNl) {printf("\n");}
        return 1;
    }

    // stubs for child classes
    virtual bool isSpecialAction(string action) {
        return false;
    }

    // stubs for child classes
    virtual bool isSpecialActionT(string action) {
        return false;
    }

    virtual int doSpecialAction(Boxaction ba, deque<Boxqentry>& theq, Boxqentry& thebqe, map<string,string>& gvalues) {
        return 1;
    }

    virtual int doSpecialActionT(Boxaction ba, deque<Boxqentry>& theq, Boxqentry& thebqe, map<string,string>& gvalues) {
        return 1;
    }

    virtual string resolve(string text, Boxqentry& bqe, map<string,string>& gvalues) {
        if (debug_R) {log.logmsg4(DEBUG, MSG4, "resolve", text, "Scalar", " ");}
        string s = text;
        if (text == "@space") {
            s = " ";
        } else if (text == "@bar") {
            s = "|";
        } else if (text == "@hash") {
            s = "#";
        } else if (text == "@none") {
            s = "";
        } else if (text == "var") {
            s = xtos(var);
        } else if (text[0] == '#') {
            string x1 = text.substr(1);
            int idx = stoi(x1) - 1;
            s = bqe.getParam(idx);
        } else if (text[0] == '$') {
            string x1 = text.substr(1);
            if (x1[0] == '!') {
                x1 = x1.substr(1);
                x1 = resolve(x1,bqe, gvalues);
            }
            s = gvalues[x1];
        }
        return s;
    }

    virtual vector<string> resolveAll(vector<string> par, Boxqentry& bqe, map<string,string>& gvalues) {
        vector<string> result;

        for (int i = 0; i < (int) par.size(); ++i) {
            if (par[i] == "#*") {
                vector<string> w = bqe.getParams();
                bxb.addToVector(result, w);
            } if (par[i] == "@gvars") {
                for (map<string, string>::iterator it=gvalues.begin(); it!=gvalues.end(); ++it) {
                    result.push_back(it->first);
                }
            } else {
                result.push_back(resolve(par[i], bqe, gvalues));
            }
        }
        return result;
    }


    virtual void setVarFromT(string name, string value) {

    }

    virtual void setVarDefault(string name) {

    }

    void doTranslate(string name, vector<string>& v, Boxqentry& bqe, map<string,string>& gvalues) {
        int len = v.size();
        int i = 0;
        while (i < len) {
            if (resolve(name, bqe, gvalues) == v[i]) {
               setVarFromT(name, v[i+1]);
               break;
            }
        i+=2;
        }
    }

    virtual void doTl(string name, vector<string>& v, string aug1, Boxqentry& bqe, map<string,string>& gvalues) {

    }

    int doFile(string filename, string op) {
        int result = 1;
        string x = filename;
        bool bhelp;
        try {
            if (!bxb.isPath(x)) {x=currDirectory+x;}
            if (op == "delete") {
                if (!filesystem::exists(x)) {
                   condqueue.push_back(true);
                } else {
                    int res = remove(x.c_str()); // 0=ok
                    if (res == 0) {
                        condqueue.push_back(true);
                    } else {
                        condqueue.push_back(false);
                    }
                }
            } else if (op == "exists") {
                if (filesystem::exists(x)) {
                    condqueue.push_back(true);
                } else {
                    condqueue.push_back(false);
                }
            } else if (op == "create") {
                ofstream o(x);
                o.close();
                if (filesystem::exists(x)) {
                    condqueue.push_back(true);
                } else {
                    condqueue.push_back(false);
                }
            } else if (op == "deletedir") {
                if (!filesystem::exists(x)) {
                    condqueue.push_back(true);
                } else if (!filesystem::is_directory(x)) {
                    condqueue.push_back(true);
                    log.logmsg4(ERROR, NOTADIRNODEL, x, "", "", "");
                } else {
                    bhelp = filesystem::remove(x);
                    if (bhelp) {
                        condqueue.push_back(true);
                    } else {
                        condqueue.push_back(false);
                    }
                }
            } else if (op == "existsdir") {
                bhelp = filesystem::is_directory(x);
                if (bhelp) {
                    condqueue.push_back(true);
                } else {
                    condqueue.push_back(false);
                }
            } else if (op == "createdir") {
                bhelp = filesystem::create_directory(x);
                if (filesystem::is_directory(x)) {
                    condqueue.push_back(true);
                } else {
                    log.logmsg4(ERROR, DIRNOTCREATED, x, "", "", "");
                    condqueue.push_back(false);
                }
            }
        } catch (exception& e) {
            log.logmsg4(FATAL, NOEXECONFILE, bxb.cstos(e.what()) , op, x, "");
            result = 0;
        }

        return result;
     }

    void doRv(string name, Boxqentry& bqe, map<string,string>& gvalues) {
        string s = resolve(name, bqe, gvalues);
        vector<string> w = bxb.split0(s);
        string help = "";
        for (int j = 0; j < (int) w.size(); ++j) {
            help = w[j] + help;
        }

        setVarFromT(name, help);
    }


    virtual void doSl(string name, vector<string>& v, Boxqentry& bqe, map<string,string>& gvalues) {

    }

    virtual void doSr(string name, string text, string aug1, Boxqentry& bqe, map<string,string>& gvalues) {

    }


    virtual void doWriteVars(string file, Boxqentry& bqe, map<string,string>& gvalues) {
        vector<string> v;
        v.push_back(name);
        vector<string> vars = {"var", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9", "var10"};
        int vmax = 5;
        if (stname == "scalar") {
            vmax = 1;
        } else if (stname == "triple") {
            vmax = 3;
        } else if (stname == "septuple") {
            vmax = 7;
        } else if (stname == "decuple") {
            vmax = 10;
        } // all others are quintuples
        for (int i = 0; i < (int) vars.size(); ++i) {
            if (i<vmax) {
                string s = resolve(vars[i], bqe, gvalues);
                if (s.size() == 0) {s="@none";}
                v.push_back(s);
            }
        }
        bxb.writeFile(file, v, true, " ", 0);
    }

    void doReadVars(string file) {
        vector<string> vars = {"var", "var2", "var3", "var4", "var5", "var6", "var7", "var8", "var9", "var10"};
        int found = 0;
        vector<string> lines = bxb.readFile(file);
        int imax = lines.size() - 1;
        for (int i = imax; i >= 0; --i) {
            string l = lines[i];
            vector<string> entries = bxb.split2(l, ' ', ' ');
            if (entries.size() > 1) {
                if (entries[0] == name) {
                    found = 1;
                    for (int j = 1; j < (int) entries.size(); ++j) {
                        string vname = vars[j-1];
                        string s = entries[j];
                        if  (s == "@none") {s ="";}
                        setVarFromT(vname, s);
                    }
                    break;
                }
            }
        }
        condqueue.push_front(found);
    }

    protected:
        T var;
        OPG opg;
    private:

};

template <> void Scalar<long long int>::setVarFromT(string name, string value) {
    if (debug_R) {log.logmsg4(DEBUG, MSG4, "setVarFromT", name, "Scalar", " ");}
    if (name == "var") {
        var = stoll(value);
    }
};

template <> void Scalar<string>::setVarFromT(string name, string value) {
    if (debug_R) {log.logmsg4(DEBUG, MSG4, "setVarFromT", name, "Scalar", "string");}
    if (name == "var") {
        var =value;
    }
};

template <> void Scalar<long double>::setVarFromT(string name, string value) {
    if (debug_R) {log.logmsg4(DEBUG, MSG4, "setVarFromT", name, "Scalar", "float");}
    if (name == "var") {
        var = stold(value);
    }
};

template <> void Scalar<long long int>::setVarDefault(string name) {
    if (debug_I) {log.logmsg4(DEBUG, MSG4, "setVarDefault", name, "Scalar", "int");}
    if (name == "var") {
        var = 0;
    }
};

template <> void Scalar<long double>::setVarDefault(string name) {
    if (debug_I) {log.logmsg4(DEBUG, MSG4, "setVarDefault", name, "Scalar", "double");}
    if (name == "var") {
        var = 0;
    }
};

template <> void Scalar<string>::setVarDefault(string name) {
    if (debug_I) {log.logmsg4(DEBUG, MSG4, "setVarDefault", name, "Scalar", "string");}
    if (name == "var") {
        var = "";
    }
};


// ------------------ Tl ------------------------------
 template <> void Scalar<string>::doTl(string name, vector<string>& v, string aug1, Boxqentry& bqe, map<string,string>& gvalues) {
        int len = v.size();
        int i = 0;
        bool once = (aug1 == "once");
        bool found = false;
        string s = resolve(name, bqe, gvalues);
        vector<string> w = bxb.split0(s);
        string u = "";
        for (int j = 0; j < (int) w.size(); ++j) {
            string help = w[j];
            if (!once || !found) {
                i = 0;
                while (i < len) {
                    string x = v[i];
                    if (w[j] == x) {
                        help = v[i+1];
                        found = true;
                        break;
                    }
                    i+=2;
                }

                if (help != "@-") {
                    u += help;
                }
            } else {
                u += help;
            }
        }

        setVarFromT(name, u);
    }

    template <> void Scalar<long long int>::doTl(string name, vector<string>& v, string aug1, Boxqentry& bqe, map<string,string>& gvalues) {
        int len = v.size();
        int i = 0;
        string s = resolve(name, bqe, gvalues);
        vector<string> w = bxb.split0(s);
        for (int j = w.size() - 1; j >= 0; --j) {
            i = 0;
            while (i < len) {
                string x = v[i];
//                if (x == "@bar") {x="|";}
//                if (x == "@space") {x=" ";}
                if (w[j] == x) {
                   if (v[i+1] == "@-") {
                        w.erase(w.begin() + j);
                   } else {
                        w[j] = v[i+1];
                   }
                   break;
                }
                i+=2;
            }
        }

        setVarFromT(name, bxb.join(w, ""));
    }

    // ------------------ Sl ------------------------------
    template <> void Scalar<string>::doSl(string name, vector<string>& v, Boxqentry& bqe, map<string,string>& gvalues) {
        int len = v.size();
        int i = 0;
        string s = resolve(name, bqe, gvalues);
        int l = s.length();
        string u = "";
        string help = "";
        int i1 = 0;
        int i2 = 0;

        i = 0;
        while (i < len) {
            help = v[i];
            if (help == "@last") {
                i1 = l - 1;
            } else if (help[0] == '@') {
                auto pos = s.find(help[1]);
                i1 = (int) pos;
            }  else {
                i1 = stoi(help) - 1; // indexing is 1-based
            }
            i+=1;
            help = v[i];
            if (help == "@last") {
                i2 = l - 1;
            } else if (help[0] == '@') {
                auto pos = s.find(help[1],i1);
                i2 = (int) pos;
            }  else {
                i2 = stoi(help) - 1;// indexing is 1-based
            }
            i+=1;
             u+=s.substr(i1, i2 - i1 + 1);
        }


        setVarFromT(name, u);
    }


    template <> void Scalar<string>::doSr(string name, string text, string aug1, Boxqentry& bqe, map<string,string>& gvalues) {
        vector<string> w = bxb.split0(text);
        if (aug1 == "desc") {
            sort (w.begin(), w.end(), greater <> ());
        } else {
            sort (w.begin(), w.end());
        }
        string help = bxb.join(w, "");
        setVarFromT(name, bxb.join(w, ""));
        if (debug_X) {log.logmsg4(DEBUG, MSG4, "sorted" + aug1, text, "to", help);}
    }
#endif // SCALAR_HXX
