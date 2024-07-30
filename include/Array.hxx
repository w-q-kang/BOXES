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

#ifndef ARRAY_HXX
#define ARRAY_HXX
#include "Box.h"
#include "OPG.h"
#include "RAG.h"
#include <vector>
#include "globals.h"
#include "Boxbasics.h"
#include "Scalar.hxx"
#include "Lrule.h"

using namespace std;

template <class T>
class Array: public Scalar<T>
{
    public:
    Array<T>():
    Scalar<T>::Scalar()
    {
    }

    virtual void init(string pname, string pstname, string pst2name, string pbtname){
        if (debug_I) {Box::log.logmsg4(DEBUG, MSG4, "init", pname, "Array", " ");}
        Scalar<T>::init(pname, pstname, pst2name, pbtname);
        setVarDefault("var");
        setVarDefault("var2");
        setVarDefault("var3");
        setVarDefault("var4");
        setVarDefault("var5");
        setVarDefault("idx");
        setVarDefault("idy");
        setVarDefault("idz");
        collen = 1;
        rowlen = 0;
        dimension = 1;
     }

    virtual string resolve(string text, Boxqentry& bqe, map<string,string>& gvalues) {
        if (debug_R) {Box::log.logmsg4(DEBUG, MSG4, "resolve", text, "Array", " ");}
        string s = text;
        if (text == "@space") {
            s = " ";
        } else if (text == "@bar") {
            s = "|";
        } else if (text == "@hash") {
            s = "#";
        } else if (text == "@none") {
            s = "";
        } else if ((text == "var") || (text == "var1")) {
            s = this->xtos(Scalar<T>::var);
        } if (text == "var2") {
            s = this->xtos(var2);
        } if (text == "var3") {
            s = this->xtos(var3);
        } if (text == "var4") {
            s = this->xtos(var4);
        } if (text == "var5") {
            s = this->xtos(var5);
        } else if (text == "idx") {
            s = this->xtos(idx);
        } else if (text == "idy") {
            s = this->xtos(idy);
        } else if (text == "idz") {
            s = this->xtos(idz);
        } else if (text == "vecx") {
            s = this->xtos(vec.at(idx-1));
        } else if (text == "vecn") {
            if (dimension == 1) {
                idn = idx - 1;
            } else if (dimension == 2) {
                idn = idx - 1 + (idy - 1) * collen;
            } else {
                idn = idx - 1 + (idy - 1) * collen + (idz-1)*collen * rowlen;
            }
            s = this->xtos(vec.at(idn));
        } else if (text == "len") {
            s = this->xtos(vec.size());
        } else if (text == "collen") {
            s = this->xtos(collen);
        } else if (text == "rowlen") {
            s = this->xtos(rowlen);
        } else if (text == "#*") {
            Box::log.logmsg4(ERROR, HASHSTARNOTALLOWED, Box::myclassname, " ", " ", " ");
        } else if (text[0] == '@') {
            string x1 = text.substr(1);
            if (Box::bxb.isLong(x1)) {
                int ix = stoi(x1) - 1;
                s = this->xtos(vec.at(ix));
            } else if (x1[0] == '!') {
                x1 = x1.substr(1);
                x1 = resolve(x1,bqe, gvalues);
                if (Box::bxb.isLong(x1)) {
                    int ix = stoi(x1) - 1;
                    s = this->xtos(vec.at(ix));
                } else {
                    Box::log.logmsg4(WARN, CANTRESOLVEAT, Box::myclassname, text, " ", " ");
                }
            }
         } else if (text[0] == '#') {
            string x1 = text.substr(1);
            int ix = stoi(x1) - 1;
            s = bqe.getParam(ix);
        } else if (text[0] == '$') {
            string x1 = text.substr(1);
            if (x1[0] == '!') {
                x1 = x1.substr(1);
                x1 = resolve(x1,bqe, gvalues);
            }
            if (debug_X) {Box::log.logmsg4(DEBUG, MSG4, "resolving", text, "as", x1);}
            s = gvalues[x1];
        }
        return s;
    }

    virtual vector<string> resolveAll(vector<string> par, Boxqentry& bqe, map<string,string>& gvalues) {
        vector<string> result;
        for (int i = 0; i < (int) par.size(); ++i) {
            if (par[i] == "vec") {
               for (int j = 0; j < (int) vec.size(); ++j) {
                  result.push_back(this->xtos(vec[j]));
                }
           } else if (par[i] == "#*") {
                vector<string> w = bqe.getParams();
                Box::bxb.addToVector(result, w);
           } else if (par[i] == "@gvars") {
                for (map<string, string>::iterator it=gvalues.begin(); it!=gvalues.end(); ++it) {
                    result.push_back(it->first);
                }
            }else {
                result.push_back(resolve(par[i], bqe, gvalues));
           }
        }
        return result;
    }


    virtual void setVarFromT(string name, string value) {

    }

    virtual void setVarDefault(string name) {

    }

    virtual bool isSpecialAction(string action) {
        bool result = false;

        if ((action == "append") || (action == "branch") || (action == "change") || (action == "clear") || (action == "collect")
            || (action == "compact") || (action == "delete") || (action == "find") || (action == "impose") || (action == "incindex")
            || (action == "insert") || (action == "intersect") || (action == "localchange") || (action == "map")|| (action == "minus")
            || (action == "read") || (action == "reverse")
            || (action == "select") || (action == "setrange") || (action == "sort")  || (action == "with") || (action == "write")) {
                 result = true;
        }

        return result;
    }

    virtual bool isSpecialActionT(string action) {
        bool result = false;

        if ((action == "arrange") || (action == "partition")) {
             result = true;
        }

        return result;
    }

    virtual int doSpecialAction(Boxaction ba, deque<Boxqentry>& theq, Boxqentry& thebqe, map<string,string>& gvalues) {
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
        string spec2;
        string spec3;
        string two;
        string typ = Box::btname;
        vector<string> v;
        string help = "";
        string filename = "";
        if (action[0] == '!') {
            action = resolve(action.substr(1),thebqe, gvalues);
        }

        if (action == "append") {
            spec = ba.getArgument(0);
            v = ba.getArgsTail(1);
            v = resolveAll(v, thebqe, gvalues);
            pt = getPointer(spec);
            if (aug1 == "new") {
              for (int i=0;i <(int) v.size();++i) {
                if (!Box::bxb.contains(v[i], *pt)) {
                    Box::pushbackT(*pt, v[i]);
                };
              }
            } else {
                Box::bxb.addToVector(*pt, v);
            }
        } else if (action == "branch") {
            if ((aug1 == "inc") || (aug2 == "inc")) {
                doIncIndex("inc");
            } else if ((aug1 == "inclt") || (aug2 == "inclt")) {
                 doIncIndex("inclt");
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
        } else if (action == "change") {
            spec = ba.getArgument(0);
            vector<string> v = ba.getArgsTail(1);
            v = resolveAll(v, thebqe, gvalues);
            doChange(spec, v);
        } else if (action == "clear") {
            spec = ba.getArgument(0);
            pt = getPointer(spec);
            pt->clear();
        } else if (action == "collect") {
            spec = ba.getArgument(0);
            spec2 = ba.getArgument(1);
            vector<string> v = ba.getArgsTail(2);
            if (aug1.size() > 0) {
                doCollect(spec, spec2, v, aug1, thebqe, gvalues);
            } else {
                doCollect(spec, spec2, v, typ, thebqe, gvalues);
            }
        } else if (action == "compact") {
            spec = ba.getArgument(0);
            spec2 = ba.getArgument(1);
            vector<string> v = ba.getArgsTail(2);
            v = resolveAll(v, thebqe, gvalues);
            doCompact(spec, spec2, v, aug1);
        } else if (action == "delete") {
            spec = ba.getArgument(0);
            if (aug1 == "at") {
                spec2 = resolve(ba.getArgument(1), thebqe, gvalues);
                doDelete(spec, aug1, spec2);
            } else {
                doDelete(spec, aug1, "");
            }
        } else if (action == "find") {
            spec = ba.getArgument(0);
            spec2 = ba.getArgument(1);
            pt = getPointer(spec2);
            two = ba.getArgument(2);
            two = resolve(two, thebqe, gvalues);
            if (aug1 == "idx") {
                int i = stoi(two);
                help = pt->at(i-1);
            } else {
                help = "0";
                for (int i = 0; i < (int) pt->size(); ++i) {
                    if (Box::xtos(pt->at(i)) == two) {
                        help = to_string(i+1);
                        break;
                    }
                }
            }
            setVarFromT(spec, help);
        } else if (action == "impose") {
            dimension = ba.argsize() + 1;

            if (dimension > 1) {
                spec = resolve(ba.getArgument(0), thebqe, gvalues);
                if (debug_X) {Box::log.logmsg4(DEBUG, MSG4, "impose", spec, " ", " ");}
                collen = stoi(spec);
            } else {
                collen = 1;
            }

            if (dimension > 2) {
                two = resolve(ba.getArgument(1), thebqe, gvalues);
                if (debug_X) {Box::log.logmsg4(DEBUG, MSG4, "impose", two, " ", " ");}
                if (two.length() > 0) {
                    rowlen = stoi(two);
                }
            } else {
                rowlen = 1;
            }
        } else if (action == "incindex") {
            doIncIndex(aug1);
        } else if (action == "insert") {
            spec = ba.getArgument(0);
            v = ba.getArgsTail(1);
            v = resolveAll(v, thebqe, gvalues);
            doInsert(spec, v, aug1);
        } else if (action == "intersect") {
            spec = ba.getArgument(0);
            v = ba.getArgsTail(1);
            v = resolveAll(v, thebqe, gvalues);
            pt = getPointer(spec);
            vector<T> vnew;
            for (int i=0;i <(int) pt->size();++i) {
                if (Box::bxb.contains(Box::xtos(pt->at(i)), v)) {
                    vnew.push_back(pt->at(i));
                };
              }
            *pt = vnew;
        } else if (action == "localchange") {
            spec = ba.getArgument(0);
            vector<string> v = ba.getArgsTail(1);
            v = resolveAll(v, thebqe, gvalues);
            if (aug1 == "plus") {
                doLocalChangePlus(spec, v[0], v[1], v[2]);
            } else {
                v.push_back(":");
                doLocalChange(spec, v, typ);
            }
        } else if (action == "map") {
            spec = ba.getArgument(0);
            vector<string> v = ba.getArgsTail(1);
            v = resolveAll(v, thebqe, gvalues);
            doMap(spec, v, typ);
        } else if (action == "minus") {
            // the is something like the reverse of intersect
            spec = ba.getArgument(0);
            v = ba.getArgsTail(1);
            v = resolveAll(v, thebqe, gvalues);
            pt = getPointer(spec);
            vector<T> vnew;
            for (int i=0;i <(int) pt->size();++i) {
                if (!Box::bxb.contains(Box::xtos(pt->at(i)), v)) {
                    vnew.push_back(pt->at(i));
                };
              }
            *pt = vnew;
        } else if (action == "read") {
            help = resolve(ba.getArgument(0), thebqe, gvalues);
            filename = Box::currDirectory + help;
            v = Box::bxb.readFile(filename);
            vec.clear();
            for (int i = 0; i < (int) v.size(); ++i) {
                Box::pushbackT(vec, v[i]);
            }
        } else if (action == "reverse") {
            spec = ba.getArgument(0);
            pt = getPointer(spec);
            vector<T> vnew;
            for (int j=(int) pt->size() - 1;j>=0;--j) {
                vnew.push_back(pt->at(j));
            }
            *pt = vnew;
        } else if (action == "select") {
            spec = ba.getArgument(0);
            vector<string> v = ba.getArgsTail(1);
            v = resolveAll(v, thebqe, gvalues);
            doSelect(spec, v, aug1);
        } else if (action == "setrange") {
            spec = ba.getArgument(0);
            vector<string> v = ba.getArgsTail(1);
            v = resolveAll(v, thebqe, gvalues);
            v.push_back("::");
            doSetrange(spec, v);
        } else if (action == "sort") {
            spec = ba.getArgument(0);
            pt = getPointer(spec);
            if (aug1 == "desc") {
                sort (pt->begin(), pt->end(), greater <> ());
            } else {
                sort (pt->begin(), pt->end());
            }
        } else if (action == "with") {
            spec = ba.getArgument(0);
            spec2 = ba.getArgument(1);
            if (Box::bxb.contains("int", {aug1,aug2,aug3})) {typ = "int";}
            if (Box::bxb.contains("float", {aug1,aug2,aug3})) {typ = "float";}
            if (Box::bxb.contains("string", {aug1,aug2,aug3})) {typ = "string";}
            if (Box::bxb.contains("mixed", {aug1,aug2,aug3})) {typ = "mixed";}
            help = "row";
            if (Box::bxb.contains("col", {aug1,aug2,aug3})) {help = "col";}
            int index1  = stoi(resolve(ba.getArgument(2),thebqe, gvalues));
            int index2  = stoi(resolve(ba.getArgument(3),thebqe, gvalues));
            spec3  = resolve(ba.getArgument(4),thebqe, gvalues); // junctor
            vector<string> v = ba.getArgsTail(5);
            v = resolveAll(v, thebqe, gvalues);
            doWith(spec, spec2, help, index1, index2, spec3, v, typ);
        } else if ((action == "write") && (aug1 == "vars")) {
            string file = Box::currDirectory + ba.getArgument(0);
            Scalar<T>::doWriteVars(file, thebqe, gvalues);
        } else if (action == "write") {
            help = resolve(ba.getArgument(0), thebqe, gvalues);
            filename = Box::currDirectory + help;
            v.clear();
            v = ba.getArgsTail(1);
            v = resolveAll(v, thebqe, gvalues);
            bool oneline = false;
            if (aug1 == "line") {oneline = true;}
            int iolen = 0;
            if (Box::bxb.isLong(aug1)) {iolen = stoll(aug1);}
            Box::bxb.writeFile(filename, v, oneline, Box::iosep, iolen);
        }  else {
            Box::log.logmsg4(ERROR, UNKNOWNSPECIALACTION, action, Box::myclassname, typ, "");
        }

        return result;
    }

    virtual int doSpecialActionT(Boxaction ba, deque<Boxqentry>& theq, Boxqentry& thebqe, map<string,string>& gvalues) {
        return 1;
    }

    void doArrange(string spec, Boxqentry& bqe, map<string,string>& gvalues) {
        vector<string> vnew;
        string help;
        if (spec == "grid to rows") {
            int myrowlen = vec.size() / collen;
            for (int i=0; i< myrowlen;i++) {
                help = "";
                for (int j=0;j<collen;j++) {
                    help += vec[i*collen+j];
                }
                vnew.push_back(help);
            }
        } else if (spec == "grid to cols") {
            int myrowlen = vec.size() / collen;
            for (int i=0; i< collen;i++) {
                help = "";
                for (int j=0;j<myrowlen;j++) {
                    help += vec[i+j*collen];
                }
                vnew.push_back(help);
            }
        } else if (spec == "rows to grid") {
            for (int i=0; i< (int) vec.size();i++) {
                vector<string> vhelp = Box::bxb.split0(vec[i]);
                for (int j=0;j< (int) vhelp.size();j++) {
                    vnew.push_back(vhelp[j]);
                }
            }
        } else if (spec == "cols to grid") {
            int maxlen = 0;
            for (int j=0;j< (int) vec.size();j++) {
                help = vec[j];
                maxlen = max(maxlen, (int) help.size());
            }
            for (int i=0; i< maxlen;i++) {
                for (int j=0;j< (int) vec.size();j++) {
                    help = vec[j];
                    if (i < (int) help.size()) {
                        vnew.push_back(help.substr(i,1));
                    }
                }
            }
        } else if ((spec == "rows to cols") || (spec == "cols to rows")) {
            int maxlen = 0;
            for (int j=0;j< (int) vec.size();j++) {
                help = vec[j];
                maxlen = max(maxlen, (int) help.size());
            }
            for (int i=0; i< maxlen;i++) {
                string help2 = "";
                for (int j=0;j< (int) vec.size();j++) {
                    help = vec[j];
                    if (i < (int) help.size()) {
                         help2 += help.substr(i,1);
                    }
                }
                vnew.push_back(help2);
            }
        }

        vec = vnew;
    }

    void doPartition(string spec, string spec2, string noption, string option, vector<string> v, Boxqentry bqe, map<string,string>& gvalues) {
        pt = getPointer(spec);
        if (debug_X) {Box::log.logmsg4(DEBUG, MSG4, "partition-start:", spec2, noption, option);}

        bool writeUnknown = (option == "keep") || (option == "reverse"); // default
        bool writeKnown = option != "reverse";
        int n = (Box::bxb.isLong(noption)) ? stoi(noption) : 0;
        pt->clear();
        string word = "";
        string rest = spec2;
        string item = "";
        int reduce = 0;

        while (rest.length() > 0) {
            reduce = 0;
            for (int j=0;j <(int) v.size();++j) {
                item = v[j];
                if (Box::bxb.startsWith(rest, item)) {
                    if (word.length()>0) {
                        if (writeUnknown) {pt->push_back(word);}
                        word = "";
                    }
                    if (writeKnown) {pt->push_back(item);}
                    reduce = item.length();
                    break;
                }
            }
            if (reduce > 0) {
                rest = rest.substr(reduce);
            } else {
                 word += rest[0];
                if ((int) word.length() == n) {
                   if (writeUnknown) {pt->push_back(word);}
                   word = "";
                }
                rest = rest.substr(1);
            }
        }
        if (word.length()>0) {
            if (writeUnknown) {pt->push_back(word);}
        }

        if (debug_X) {Box::log.logmsg4(DEBUG, MSG4, "partition-end:", noption, option, to_string(pt->size()));}
    }

    void doSelect(string spec, vector<string> v, string aug) {
        pt = getPointer(spec);
        vector<T> vnew;
        if (aug == "idx") {
            for (int j=0;j <(int) v.size();++j) {
                int ix = stoi(v[j]);
                if ( ix <= len) {
                    vnew.push_back(pt->at(ix-1));
                }
            }
            *pt = vnew;
        } else {
             for (int j=0;j <(int) pt->size();++j) {
                if (Box::bxb.contains(Box::xtos(pt->at(j)), v)) {
                    vnew.push_back(pt->at(j));
                }
            }
            *pt = vnew;
        }
    }

    void doSetrange(string spec, vector<string> v) {
        pt = getPointer(spec);
        RAG rag = RAG();
        vector<string> vresult = rag.run(v);
        pt->clear();
        for (int i=0;i <(int) vresult.size();++i) {
            Box::pushbackT(*pt, vresult[i]);
        }
    }

    void doCompact(string spec, string spec2, vector<string> v, string aug) {
        pt = getPointer(spec2);
        if (debug_X) {Box::log.logmsg4(DEBUG, MSG4, "compact with", " ", " ", to_string(v.size()));}
        string result = "";
        int j = 0;
        int vlen = v.size();
        if ((dimension == 2) && (aug== "row")) {
            int n = (idy - 1) * collen;
            for (int i=n;i <n+collen;++i) {
                if ((vlen > 0) && (i>n)) {
                    if (j >= vlen) {j=0;}
                    result += v.at(j);
                    j++;
                }
                result += Box::xtos(pt->at(i));
            }

        } else if ((dimension == 2) && (aug== "col")) {
            int n = idx - 1;
            for (int i=n;i < (int) pt->size();i+=collen) {
                if ((vlen > 0) && (i>n)) {
                    if (j >= vlen) {j=0;}
                    result += v.at(j);
                    j++;
                }
                result += Box::xtos(pt->at(i));
            }
        } else {
            for (int i=0;i <(int) pt->size();++i) {
                if ((vlen > 0) && (i>0)) {
                    if (j >= vlen) {j=0;}
                    result += v.at(j);
                    j++;
                }
                result += Box::xtos(pt->at(i));
            }
        }
        setVarFromT(spec, result);
    }

    void doMap(string spec, vector<string> v, string typ) {
        pt = getPointer(spec);
        vector<string> vcurr;
        vector<T> vnew;
        int from = 0;
        int to = pt->size() - 1;
        string help;
        if (Box::bxb.contains("@prev", v)) {from++;}
        if (Box::bxb.contains("@succ", v)) {to--;}
        for (int i=from;i <= to;++i) {
            vcurr.clear();
            for (int j=0;j< (int) v.size();j++) {
                if (v.at(j) == "@curr") {
                    vcurr.push_back(Box::xtos(pt->at(i)));
                } else if (v.at(j) == "@prev") {
                    vcurr.push_back(Box::xtos(pt->at(i-1)));
                } else if (v.at(j) == "@succ") {
                    vcurr.push_back(Box::xtos(pt->at(i+1)));
                } else {
                    vcurr.push_back(v.at(j));
                }
            }
            help = Scalar<T>::opg.run(vcurr, typ);
            Box::pushbackT(vnew, help);
        }
        *pt = vnew;
    }

    void doLocalChange(string spec, vector<string> v, string typ) {
        pt = getPointer(spec);
        map<string,vector<Lrule>> m;
        int phase = 0;
        // first convert input to rules
        string name = "";
        vector<string> con;
        vector<string> exp;
        vector<string> rules;
        if (debug_X) {Box::log.logmsg4(DEBUG, MSG4, "localchange", "rules", typ, " ");}
        for (int i=0;i < (int) v.size();++i) {
            string item = v.at(i);
            if (phase == 0) {
                con.clear();
                exp.clear();
                name = item;
                phase++;
            } else if (item == ":") {
                phase = 0;
                Lrule lr = Lrule(name, con, exp);
                if (!Box::bxb.contains(name, rules)) {
                    rules.push_back(name);
                }
                vector<Lrule> vr;
                if (m.find(name) == m.end()) {
                    vr.push_back(lr);
                } else {
                    vr = m[name];
                    vr.push_back(lr);
                }
                m[name] = vr;
            } else if (item == "=>") {
                phase++;
            } else if (phase == 1) {
                con.push_back(item);
            } else if (phase == 2) {
                exp.push_back(item);
            }
        }

        //if (debug_X) {Box::log.logmsg4(DEBUG, MSG4, "localchange", "array", " ", " ");}
        // now process array
        vector<T> vnew;

        int total = pt->size();
        int rows = total / collen;
        int maxcol = collen - 1;
        int maxrow = rows -1;
         // init
        for (int i=0;i < total;++i) {
            vnew.push_back(pt->at(i));
        }
        //if (debug_X) {Box::log.logmsg4(DEBUG, MSG4, "localchange", "copy", " ", " ");}
        string help;
        bool change = false;
        int n=0;
        for (int i=0;i < rows;++i) {
            for (int j=0;j< collen;j++) {
                for (int r=0;r< (int) rules.size();r++) {
                    string rule = rules[r];
                    bool doit = false;
                    if (rule == "@ul") {
                        doit = (i>0) && (j>0);
                        if (doit) {n = (i-1) * collen + j-1;}
                    } else if (rule == "@um") {
                        doit = (i>0);
                        if (doit) {n = (i-1) * collen + j;}
                    } else if (rule == "@ur") {
                        doit = (i>0) && (j<maxcol);
                        if (doit) {n = (i-1) * collen + j+1;}
                    } else if ((rule == "@cl") && (j>0)){
                        doit = j>0;
                        if (doit) {n = i * collen + j-1;}
                    } else if (rule == "@cm"){
                        doit = true;
                        n = i * collen + j;
                    } else if (rule == "@cr") {
                        doit = j<maxcol;
                        if (doit) {n = i * collen + j+1;}
                    } else if (rule == "@dl") {
                        doit = (i<maxrow) && (j>0);
                        if (doit) {n = (i+1) * collen + j-1;}
                    } else if (rule == "@dm") {
                        doit = (i<maxrow);
                        if (doit) {n = (i+1) * collen + j;}
                    } else if (rule == "@dr") {
                        doit = (i<maxrow) && (j<maxcol);
                        if (doit) {n = (i+1) * collen + j+1;}
                    }
                    if (doit) {
                        tie(change,help) = applyLC(i, j, m[rule], *pt, typ);
                        if (change) {Box::setvecT(vnew, n, help);}
                    }
                }
            }
        }
        *pt = vnew;
    }

    tuple<bool,string> applyLC(int row, int col, vector<Lrule>& vlr, vector<T>& vold, string typ) {
        bool change = false;
        string result = "";
        string help;
        // apply each rule
        for (int i=0;i < (int) vlr.size();++i) {
            Lrule lr = vlr.at(i);
            vector<string> vcurr = lr.getCondition();
            for (int j=0;j < (int) vcurr.size();++j) {
                vcurr.at(j) = resolveLC(vcurr.at(j), row, col, vold);
            }
            //if (debug_X) {Box::log.logmsg4(DEBUG, MSG4, "localchange testing", Box::bxb.join(vcurr, " " ), "for", lr.getPosition());}
            help = Scalar<T>::opg.run(vcurr, typ);
            //if (debug_X) {Box::log.logmsg4(DEBUG, MSG4, "localchange cond=", help, "for", lr.getPosition());}
            if ((help != "0") && (help != "")) {
                change = true;
                vector<string> vcurr = lr.getExpression();
                for (int j=0;j < (int) vcurr.size();++j) {
                    vcurr.at(j) = resolveLC(vcurr.at(j), row, col, vold);
                }
                if (vcurr.size() == 1) {
                    result = vcurr.at(0);
                } else {
                    result = Scalar<T>::opg.run(vcurr, typ);
                }
            }
        }
        return make_tuple(change, result);
    }

    // not correct: must check whether position is allowed in all cases
    string resolveLC(string text, int row, int col, vector<T>& vold) {
        string result = text;
        if (text == "@ul") {
            result = Box::xtos(vold.at((row-1) * collen + col-1));
        } else if (text == "@um") {
            result = Box::xtos(vold.at((row-1) * collen + col));
        } else if (text == "@ur") {
            result = Box::xtos(vold.at((row-1) * collen + col+1));
        } else if (text == "@cl") {
            result = Box::xtos(vold.at(row * collen + col-1));
        } else if (text == "@cm") {
            result = Box::xtos(vold.at(row * collen + col));
        } else if (text == "@cr") {
            result = Box::xtos(vold.at(row * collen + col+1));
        } else if (text == "@dl") {
            result = Box::xtos(vold.at((row+1) * collen + col-1));
        } else if (text == "@dm") {
            result = Box::xtos(vold.at((row+1) * collen + col));
        } else if (text == "@dr") {
            result = Box::xtos(vold.at((row+1) * collen + col+1));
        }
        return result;
    }

    void doLocalChangePlus(string spec, string cvalue, string oldvalue, string newvalue) {
        pt = getPointer(spec);

        vector<T> vnew;

        int total = pt->size();
        int rows = total / collen;
        int maxcol = collen - 1;
        int maxrow = rows -1;
         // init
        for (int i=0;i < total;++i) {vnew.push_back(pt->at(i));}

        string help;
        int n=0;
        int m = 0;

        for (int i=0;i < rows;++i) {
            for (int j=0;j< collen;j++) {
                n = i * collen + j;
                if (Box::xtos(vec.at(n)) == cvalue) {
                    if (i>0) {
                        m = n - collen;
                        if (Box::xtos(vec.at(m)) == oldvalue) {
                            Box::setvecT(vnew, m, newvalue);
                        };
                    }

                    if (j>0) {
                        m = n - 1;
                        if (Box::xtos(vec.at(m)) == oldvalue) {
                            Box::setvecT(vnew, m, newvalue);
                        };
                    }
                    if (j<maxcol) {
                        m = n + 1;
                        if (Box::xtos(vec.at(m)) == oldvalue) {
                            Box::setvecT(vnew, m, newvalue);
                        };
                    }
                    if (i<maxrow) {
                        m = n + collen;
                       if (Box::xtos(vec.at(m)) == oldvalue) {
                            Box::setvecT(vnew, m, newvalue);
                        };
                    }
                }
            }
        }
        *pt = vnew;
    }


    void doChange(string spec, vector<string>& v) {
        pt = getPointer(spec);
        vector<T> vnew;
        string help;
        string veci;
        int times = 1;
        for (int i=0;i < (int) pt->size();++i) {
            veci = Box::xtos(pt->at(i));
            times = 1;
            for (int j=0;j< (int) v.size();j+=2) {
                string first = v[j];
                string second = v[j+1];
                if ((first[0] == '@')) {
                    string x = first.substr(1);
                    int x1 = stoi(x);
                    if (x1 == i) {veci = second;}
                    break;
                } else if (first == veci) {
                    if (second == "@+") {
                        times = 2;
                    } else if (second == "@-") {
                        times = 0;
                    } else if (second[0] == '@') {
                        string x = second.substr(1);
                        int x1 = stoi(x);
                        veci = Box::xtos(pt->at(x1));
                    } else {
                        veci = second;
                    }
                    break;
                }
            }
            if (times > 0) {Box::pushbackT(vnew, veci);}
            if (times == 2) {Box::pushbackT(vnew, veci);}

        }
        *pt = vnew;
    }

    void doCollect(string name, string spec, vector<string> v, string typ,Boxqentry& thebqe, map<string,string>& gvalues) {
        pt = getPointer(spec);
        vector<string> vcurr;
        vector<string> vnew;
        int from = 0;
        int to = pt->size() - 1;
        string help;
        if (Box::bxb.contains("@prev", v)) {from++;}
        if (Box::bxb.contains("@succ", v)) {to--;}
        for (int i=from;i <= to;++i) {
            // must be reevaluated for each iteration
            // because some values will have changed
            vnew = resolveAll(v, thebqe, gvalues);
            vcurr.clear();
            for (int j=0;j< (int) vnew.size();j++) {
                if (vnew.at(j) == "@curr") {
                    vcurr.push_back(Box::xtos(pt->at(i)));
                } else if (v.at(j) == "@prev") {
                    vcurr.push_back(Box::xtos(pt->at(i-1)));
                } else if (v.at(j) == "@succ") {
                    vcurr.push_back(Box::xtos(pt->at(i+1)));
                } else {
                    vcurr.push_back(vnew.at(j));
                }
            }
            help = Scalar<T>::opg.run(vcurr, typ);
            if (debug_X) {Box::log.logmsg4(DEBUG, MSG4, "collecting", Box::bxb.join(vcurr, ","), "as", help);}
            setVarFromT(name, help);
        }
    }

    void doWith(string name, string spec, string aug, int index1, int index2, string junctor, vector<string> v, string typ) {
        pt = getPointer(spec);
        int myrowlen = vec.size() / collen;
        int n = 0;
        vector<string> vexpr;
        string help;

        if (aug == "row") {
            if (debug_X) {Box::log.logmsg4(DEBUG, MSG4, "doing with/row ", to_string(index1), to_string(index2), to_string(collen));}
            for (int i=0;i < collen;++i) {
                if (i==0) {
                    vexpr.clear();
                } else {
                    vexpr.push_back(junctor);
                }
                for (int j=0;j< (int) v.size();j++) {
                    if (v.at(j) == "@first") {
                        n = i + (index1 - 1) * collen;
                        vexpr.push_back(Box::xtos(pt->at(n)));
                    } else if (v.at(j) == "@second") {
                        n = i + (index2 - 1) * collen;
                        vexpr.push_back(Box::xtos(pt->at(n)));
                    }  else {
                        vexpr.push_back(v[j]);
                    }
                }
            }
            help = Scalar<T>::opg.run(vexpr, typ);
            if (debug_X) {Box::log.logmsg4(DEBUG, MSG4, "with done:", Box::bxb.join(vexpr, ","), "as", help);}
            setVarFromT(name, help);
        } else if (aug == "col") {
            if (debug_X) {Box::log.logmsg4(DEBUG, MSG4, "doing with/col ", to_string(index1), to_string(index2), to_string(myrowlen));}
            for (int i=0;i < myrowlen;++i) {
                if (i==0) {
                    vexpr.clear();
                } else {
                    vexpr.push_back(junctor);
                }
                for (int j=0;j< (int) v.size();j++) {
                    if (v.at(j) == "@first") {
                        n = (index1 - 1) + i * collen;
                        vexpr.push_back(Box::xtos(pt->at(n)));
                    } else if (v.at(j) == "@second") {
                        n = (index2 - 1) + i * collen;
                        vexpr.push_back(Box::xtos(pt->at(n)));
                    }  else {
                        vexpr.push_back(v[j]);
                    }
                }
            }
            help = Scalar<T>::opg.run(vexpr, typ);
            if (debug_X) {Box::log.logmsg4(DEBUG, MSG4, "with done:", Box::bxb.join(vexpr, ","), "as", help);}
            setVarFromT(name, help);
        }
    }

    void doIncIndex(string aug) {
        bool check;
        if (dimension == 1) {
          idx++;
          if ((aug == "lt") || (aug == "inclt")) {
            check = (idx < (int) vec.size());
          } else {
            check = (idx <= (int) vec.size());
          }

        } else if (dimension == 2) {
            idx++;
            if (idx > collen) {
                idx = 1;
                idy++;
            }
            check = (idy * collen  <= (int) vec.size());
        }

        Box::condqueue.push_front(check);
    }

    void doDelete(string spec, string aug, string index) {
        pt = getPointer(spec);
        if (aug == "at") {
            int ix = stoi(index);
            pt->erase(pt->begin() + ix - 1);
        } else if (dimension == 1) {
            pt->erase(pt->begin() + idx - 1);
        } else if (dimension == 2) {
            if (aug == "row") {
                int n = (idy - 1) * collen;
                pt->erase(pt->begin() + n, pt->begin()+n+collen);
                idy--;
             } else {
                // rowlen isnt set, so must be calculated
                int n = pt->size() / collen;
                //
                for (int j=n;j>0;j--) {
                    pt->erase(pt->begin()+idx - 1 + (j-1)*collen);
                }
                idx--;
                collen--;
            }
        }
    }

    void doInsert(string spec, vector<string> v, string aug) {
        pt = getPointer(spec);
        vector<T> w;
        int k;
        int n;
        if (dimension == 2) {
            if (aug == "row") {
                k = 0;
                while((int) w.size() < collen) {
                    Box::pushbackT(w, v[k]);
                    k++;
                    if (k>= (int) v.size()) {k=0;}
                }
                n = (idy - 1) * collen;
                pt->insert(pt->begin() + n, w.begin(), w.end());
                idy++;

            } else {
                k = 0;
                // rowlen isnt set, so must be calculated
                n = pt->size() / collen;
                for (int i=0;i< n;i++) {
                    Box::pushbackT(w, v[k]);
                    k++;
                    if (k>= (int) v.size()) {k=0;}
                }
                reverse(w.begin(), w.end());

                int i = 0;
                for (int j=n;j>0;j--) {
                    pt->insert(pt->begin()+idx - 1 + (j-1)*collen, w[i]);
                    i++;
                    if (i>=(int)w.size()){i=0;}
                }
                idx++;
                collen++;
            }
        } else if (dimension == 1) {
            for (int k=0;k<(int)v.size();k++) {
                Box::pushbackT(w, v[k]);
            }
            pt->insert(pt->begin() + idx - 1, w.begin(),w.end());
            idx += (int) w.size();
        }
    }

    virtual vector<T>* getPointer(string spec) {
         return &(Array<T>::vec);
    }
        vector<T> vec;
    protected:
        T var2;
        T var3;
        T var4;
        T var5;
        int idx;
        int idy;
        int idz;
        int idn;
        int len;
        int collen;
        int rowlen;
        int dimension;
        vector<T>* pt;
    private:

};

template <> void Array<long long int>::setVarFromT(string name, string value) {
        if (debug_R) {log.logmsg4(DEBUG, MSG4, "setVarFromT", name, "v", "int");}
        if ((name == "var") || (name == "var1")) {
            var = stoll(value);
        } else if (name == "var2") {
            var2 = stoll(value);
        } else if (name == "var3") {
            var3 = stoll(value);
        } else if (name == "var4") {
            var4 = stoll(value);
        } else if (name == "var5") {
            var5 = stoll(value);
        } else if (name == "idx") {
            idx = stoll(value);
        } else if (name == "idy") {
            idy = stoll(value);
        } else if (name == "idz") {
            idz = stoll(value);
        } else if (name == "vecx") {
            vec.at(idx-1) = stoll(value);
        } else if (name == "vecn") {
            if (dimension == 1) {
                idn = idx - 1;
            } else if (dimension == 2) {
                idn = idx - 1 + (idy - 1) * collen;
            } else {
                idn = idx - 1 + (idy - 1) * collen + (idz-1)*collen * rowlen;
            }
            vec.at(idn) = stoll(value);
        } else if (name[0] == '@') {
            string x1 = name.substr(1);
            if (Box::bxb.isLong(x1)) {
                int ix = stoi(x1) - 1;
                vec.at(ix)= stoll(value);
            } else {
                // using '!' is restricted to read only
                Box::log.logmsg4(WARN, CANTRESOLVEAT, Box::myclassname, name, " ", " ");
            }
        }
};

template <> void Array<long double>::setVarFromT(string name, string value) {
        if (debug_R) {log.logmsg4(DEBUG, MSG4, "setVarFromT", name, "v", "float");}
        if ((name == "var") || (name == "var1")) {
            var = stold(value);
        } else if (name == "var2") {
            var2 = stold(value);
        } else if (name == "var3") {
            var3 = stold(value);
        } else if (name == "var4") {
            var4 = stold(value);
        } else if (name == "var5") {
            var5 = stold(value);
        } else if (name == "idx") {
            idx = stoll(value);
        } else if (name == "idy") {
            idy = stoll(value);
        } else if (name == "idz") {
            idz = stoll(value);
        } else if (name == "vecx") {
            vec.at(idx-1) = stold(value);
        } else if (name == "vecn") {
            if (dimension == 1) {
                idn = idx - 1;
            } else if (dimension == 2) {
                idn = idx - 1 + (idy - 1) * collen;
            } else {
                idn = idx - 1 + (idy - 1) * collen + (idz-1)*collen * rowlen;
            }
            vec.at(idn) = stold(value);
        } else if (name[0] == '@') {
            string x1 = name.substr(1);
            if (Box::bxb.isLong(x1)) {
                int ix = stoi(x1) - 1;
                vec.at(ix)= stold(value);
            }  else {
                // using '!' is restricted to read only
                Box::log.logmsg4(WARN, CANTRESOLVEAT, Box::myclassname, name, " ", " ");
            }
        }
};

template <> void Array<string>::setVarFromT(string name, string value) {
    if (debug_R) {log.logmsg4(DEBUG, MSG4, "setVarFromT", name, myclassname, "string");}
         if ((name == "var") || (name =="var1")) {
            var = value;
        } else if (name =="var2") {
            var2 = value;
        } else if (name == "var3") {
            var3 = value;
        } else if (name == "var4") {
            var4 = value;
        } else if (name == "var5") {
            var5 = value;
        } else if (name == "idx") {
            idx = stoll(value);
        } else if (name == "idy") {
            idy = stoll(value);
        } else if (name == "idz") {
            idz = stoll(value);
        } else if (name == "vecx") {
            vec.at(idx-1) = value;
        } else if (name == "vecn") {
            if (dimension == 1) {
                idn = idx - 1;
            } else if (dimension == 2) {
                idn = idx - 1 + (idy - 1) * collen;
            } else {
                idn = idx - 1 + (idy - 1) * collen + (idz-1)*collen * rowlen;
            }
            vec.at(idn) = value;
        } else if (name[0] == '@') {
            string x1 = name.substr(1);
            if (Box::bxb.isLong(x1)) {
                int ix = stoi(x1) - 1;
                vec.at(ix)= value;
            } else {
                // using '!' is restricted to read only
                Box::log.logmsg4(ERROR, CANTRESOLVEATRO, Box::myclassname, name, " ", " ");
            }
        }
};

template <> void Array<long long int>::setVarDefault(string name) {
    if (debug_I) {log.logmsg4(DEBUG, MSG4, "setVarDefault", name, myclassname, "int");}
    if ((name == "var") || (name =="var1")) {
        var = 0;
    } else if (name =="var2") {
        var2= 0;
    } else if (name =="var3") {
        var3 = 0;
    } else if (name =="var4") {
        var4 = 0;
    } else if (name =="var5") {
        var5 = 0;
    } else if (name == "idx") {
        idx = 0;
    } else if (name == "idy") {
        idy = 0;
    } else if (name == "idz") {
        idz = 0;
    } else if (name == "vecx") {
        vec.at(idx-1) = 0;
    } else if (name == "vecn") {
        if (dimension == 1) {
            idn = idx - 1;
        } else if (dimension == 2) {
            idn = idx - 1 + (idy - 1) * collen;
        } else {
            idn = idx - 1 + (idy - 1) * collen + (idz-1)*collen * rowlen;
        }
        vec.at(idn) = 0;
    } else if (name[0] == '@') {
        string x1 = name.substr(1);
        if (Box::bxb.isLong(x1)) {
            int ix = stoi(x1) - 1;
            vec.at(ix)= 0;
        } else {
            // using '!' is restricted to read only
            Box::log.logmsg4(ERROR, CANTRESOLVEATRO, Box::myclassname, name, " ", " ");
        }
    }
};

template <> void Array<long double>::setVarDefault(string name) {
    if (debug_I) {log.logmsg4(DEBUG, MSG4, "setVarDefault", name, myclassname, "int");}
    if ((name == "var") || (name =="var1")) {
        var = 0L;
    } else if (name =="var2") {
        var2= 0L;
    } else if (name =="var3") {
        var3 = 0L;
    } else if (name =="var4") {
        var4 = 0L;
    } else if (name =="var5") {
        var5 = 0L;
    } else if (name == "idx") {
        idx = 0;
    } else if (name == "idy") {
        idy = 0;
    } else if (name == "idz") {
        idz = 0;
    } else if (name == "vecx") {
        vec.at(idx-1) = 0L;
    } else if (name == "vecn") {
        if (dimension == 1) {
            idn = idx - 1;
        } else if (dimension == 2) {
            idn = idx - 1 + (idy - 1) * collen;
        } else {
            idn = idx - 1 + (idy - 1) * collen + (idz-1)*collen * rowlen;
        }
        vec.at(idn) = 0L;
    } else if (name[0] == '@') {
        string x1 = name.substr(1);
        if (Box::bxb.isLong(x1)) {
            int ix = stoi(x1) - 1;
            vec.at(ix)= 0L;
            vec.at(ix)= 0L;
        } else {
            // using '!' is restricted to read only
            Box::log.logmsg4(ERROR, CANTRESOLVEATRO, Box::myclassname, name, " ", " ");
        }
    }
};

template <> void Array<string>::setVarDefault(string name) {
    if (debug_I) {log.logmsg4(DEBUG, MSG4, "setVarDefault", name, myclassname, "string");}
    if ((name == "var") || (name =="var1")) {
        var = "";
    } else if (name =="var2") {
        var2 = "";
    } else if (name =="var3") {
        var3 = "";
    } else if (name =="var4") {
        var4 = "";
    } else if (name =="var5") {
        var5 = "";
    } else if (name == "idx") {
        idx = 0;
    } else if (name == "idy") {
        idy = 0;
    } else if (name == "idz") {
        idz = 0;
    } else if (name == "vecx") {
        vec.at(idx-1) = "";
    } else if (name == "vecn") {
        idn = idx - 1 + (idy - 1) * collen + (idz-1)*collen * rowlen;
        vec.at(idn) = "";
    } else if (name[0] == '@') {
        string x1 = name.substr(1);
        if (Box::bxb.isLong(x1)) {
            int ix = stoi(x1) - 1;
            vec.at(ix)= "";
        }  else {
            // using '!' is restricted to read only
            Box::log.logmsg4(ERROR, CANTRESOLVEATRO, Box::myclassname, name, " ", " ");
        }
    }
};

template <> int Array<string>::doSpecialActionT(Boxaction ba, deque<Boxqentry>& theq, Boxqentry& thebqe, map<string,string>& gvalues) {
        int result = 1;
        string action = ba.getName();
        string aug1 = ba.getAug1();
        string aug2 = ba.getAug2();
        string aug3 = ba.getAug3();
        if (debug_X) {log.logmsg4(DEBUG, MSG4, "called action", action, aug1, aug2);}
        string spec;
        string spec2;
        //string two;
        string typ = Box::btname;
        string help = "";

        if (action[0] == '!') {
            action = resolve(action.substr(1),thebqe, gvalues);
         }

        if (action == "arrange") {
            vector<string> v = ba.getArguments();
            if (debug_X) {log.logmsg4(DEBUG, MSG4, "called", action, spec, to_string(v.size()));}
            v = resolveAll(v, thebqe, gvalues);
            doArrange(Box::bxb.join(v, " "), thebqe, gvalues);
        }  else if (action == "partition") {
            spec = ba.getArgument(0);
            spec2 = resolve(ba.getArgument(1),thebqe, gvalues);
            vector<string> v = ba.getArgsTail(2);
            if (debug_X) {log.logmsg4(DEBUG, MSG4, "called", action, spec2, to_string(v.size()));}
            v = resolveAll(v, thebqe, gvalues);
            doPartition(spec, spec2, aug1, aug2, v, thebqe, gvalues);
        } else {
            Box::log.logmsg4(ERROR, UNKNOWNSPECIALACTION, action, Box::myclassname, typ, "");
        }

        return result;
    }

    template <> int Array<long long int>::doSpecialActionT(Boxaction ba, deque<Boxqentry>& theq, Boxqentry& thebqe, map<string,string>& gvalues) {
        int result = 1;
        string action = ba.getName();
        Box::log.logmsg4(ERROR, UNKNOWNSPECIALACTION, action, myclassname, "int", "");
        return result;
    }

    template <> int Array<long double>::doSpecialActionT(Boxaction ba, deque<Boxqentry>& theq, Boxqentry& thebqe, map<string,string>& gvalues) {
        int result = 1;
        string action = ba.getName();
        Box::log.logmsg4(ERROR, UNKNOWNSPECIALACTION, action, myclassname, "double", "");
        return result;
    }
#endif // ARRAY_HXX
