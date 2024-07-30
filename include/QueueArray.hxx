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

#ifndef QUEUEARRAY_HXX
#define QUEUEARRAY_HXX
#include "Box.h"
#include "OPG.h"
#include <vector>
#include <map>
#include <deque>
#include "globals.h"
#include "Boxbasics.h"
#include "Scalar.hxx"
#include "Array.hxx"

using namespace std;

template <class T>
class QueueArray: public Array<T>
{
    public:
    QueueArray<T>():
    Array<T>::Array()
    {
    }

    void init(string pname, string pstname, string pst2name, string pbtname){
        if (debug_I) {Box::log.logmsg4(DEBUG, MSG4, "init", pname, Box::myclassname, " ");}
        Array<T>::init(pname, pstname, pst2name, pbtname);
     }

    string resolve(string text, Boxqentry& bqe, map<string,string>& gvalues) {
        int vindex;
        string x;
        if (debug_R) {Box::log.logmsg4(DEBUG, MSG1, "resolve '" + text + "' " + Box::myclassname + " int", "", "", "");}
        string s = "";
        if (text == "qlen") {
            s = this->xtos(qarray.size());
        } else if (text == "len2" ){
            s = this->xtos(vec2.size());
        } else if (text == "len3" ){
            s = this->xtos(vec3.size());
        } else if (Box::bxb.startsWith(text, "vec2@")) {
            x = text.substr(5);
            if (Box::bxb.isLong(x)) {
               vindex = stoi(x);
               s =  this->xtos(vec2.at(vindex-1));
            } else if (x[0] == '!') {
                x = x.substr(1);
                x = resolve(x,bqe, gvalues);
                if (Box::bxb.isLong(x)) {
                    vindex = stoi(x);
                    s = this->xtos(vec2.at(vindex-1));
                }
            } else {
                Box::log.logmsg4(WARN, CANTRESOLVEAT, Box::myclassname, text, " ", " ");
            }
        } else if (Box::bxb.startsWith(text, "vec3@")) {
            x = text.substr(5);
            if (Box::bxb.isLong(x)) {
               vindex = stoi(x);
               s =  this->xtos(vec3.at(vindex-1));
            } else if (x[0] == '!') {
               x = x.substr(1);
               x = resolve(x,bqe, gvalues);
               if (Box::bxb.isLong(x)) {
                    vindex = stoi(x);
                    s = this->xtos(vec3.at(vindex-1));
               }
            } else {
                Box::log.logmsg4(WARN, CANTRESOLVEAT, Box::myclassname, text, " ", " ");
            }
        } else {
            s = Array<T>::resolve(text, bqe, gvalues);
        }
        if (debug_R) {Box::log.logmsg4(DEBUG, MSG1, "resolve result '" + s + "' " + Box::myclassname + " int", "", "", "");}
        return s;
    }

    vector<string> resolveAll(vector<string> par, Boxqentry& bqe, map<string,string>& gvalues) {
        vector<string> result;
        for (int i = 0; i < (int) par.size(); ++i) {
            if (par[i] == "vec") {
               for (int j = 0; j < (int) Array<T>::vec.size(); ++j) {
                  result.push_back(this->xtos(Array<T>::vec[j]));
                }
           } else if (par[i] == "vec2") {
               for (int j = 0; j < (int) vec2.size(); ++j) {
                  result.push_back(this->xtos(vec2[j]));
                }
           } else if (par[i] == "vec3") {
               for (int j = 0; j < (int) vec3.size(); ++j) {
                  result.push_back(this->xtos(vec3[j]));
                }
           } else if (par[i] == "#*") {
                vector<string> w = bqe.getParams();
                Box::bxb.addToVector(result, w);
           } else if (par[i] == "@gvars") {
                for (map<string, string>::iterator it=gvalues.begin(); it!=gvalues.end(); ++it) {
                    result.push_back(it->first);
                }
           }
           else {
                result.push_back(resolve(par[i], bqe, gvalues));
           }
        }
        return result;

        if (debug_R) {Box::log.logmsg4(DEBUG, MSG1, "resolveAll result '" + Box::bxb.join(result,",") + "' " + Box::myclassname + " int", "", "", "");}
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

        if ( (action == "qadd") || (action == "qdel") || (action == "qget")|| (action == "qins")|| (action == "qset")
            || (action == "clear") || (action == "copy") || (action == "swap") ) {
            result = true;
        } else {
            result = Array<T>::isSpecialAction(action);

        }

        return result;
    }

    bool isSpecialActionT(string action) {
        bool result = false;

        result = Array<T>::isSpecialActionT(action);

        return result;
    }

    void doCopy(string left, string right) {
        vector<T> * pleft = getPointer(left);
        vector<T> * pright = getPointer(right);

         pleft->clear();

        for (int i = 0; i < (int) pright->size(); ++i) {
            pleft->push_back(pright->at(i));
        }

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

        string typ = Box::btname;
        string spec = "";
        vector<string> v;
        int lindex = 0;
        string s;
        string t;

        if (action[0] == '!') {
            action = resolve(action.substr(1),thebqe, gvalues);
        }

        if (action == "copy") {
            s = resolve(ba.getArgument(0), thebqe, gvalues);
            t = resolve(ba.getArgument(1), thebqe, gvalues);
            doCopy(s,t);
        } else if (action == "clear") {
            if (aug1 == "q") {
                qarray.clear();
            } else {
                spec = ba.getArgument(0);
                pt = getPointer(spec);
                pt->clear();
            }
        } else if (action == "swap") {
            s = resolve(ba.getArgument(0), thebqe, gvalues);
            t = resolve(ba.getArgument(1), thebqe, gvalues);
            doCopy("vhlp", s);
            doCopy(s,t);
            doCopy(t, "vhlp");
        } else if (action == "qadd") {
            s = resolve(ba.getArgument(0), thebqe, gvalues);
            pt = getPointer(s);
            qarray.push_back(*pt);
        } else if (action == "qdel") {
            t = resolve(ba.getArgument(0), thebqe, gvalues);
            if (t=="@last") {
                lindex = qarray.size() - 1;
            } else {
                lindex = stoi(t) - 1;
            }
            if (lindex >= (int) qarray.size()) {
                Box::log.logmsg4(ERROR, QADELUNKNOWNINDEX, t, "", "", "");
            } else if (lindex == 0) {
                qarray.pop_front();
            } else if (lindex > 0) {
                qarray.erase(qarray.begin() + lindex);
            } else {
                Box::log.logmsg4(ERROR, QADELNEGINDEX,t, "", "", "");
            }
        } else if (action == "qget") {
            string s = resolve(ba.getArgument(0), thebqe, gvalues);
            string t = resolve(ba.getArgument(1), thebqe, gvalues);
            pt = getPointer(s);
            if (t=="@last") {
                lindex = qarray.size() - 1;
            } else {
                lindex = stoi(t) - 1;
            }
            if (lindex == 0) {
                *pt = qarray.front();
            } else {
                *pt = *(qarray.begin() + lindex);
            }
        } else if (action == "qins") {
            string s = resolve(ba.getArgument(0), thebqe, gvalues);
            string t = resolve(ba.getArgument(1), thebqe, gvalues);
            pt = getPointer(s);
            if (t== "@new") {
                  qarray.push_back(*pt);
            } else {
                if (t=="@last") {
                    lindex = qarray.size() - 1;
                } else {
                    lindex = stoi(t) - 1;
                }
                qarray.insert(qarray.begin() + lindex, *pt);
            }
        } else if (action == "qset") {
            string s = resolve(ba.getArgument(0), thebqe, gvalues);
            string t = resolve(ba.getArgument(1), thebqe, gvalues);
            pt = getPointer(s);

            if (t=="@last") {
                lindex = qarray.size() - 1;
            } else {
                lindex = stoi(t) - 1;
            }

            qarray.at(lindex) = *pt;

        } else if ((action == "clear") && (aug1 == "queue")) {
            qarray.clear();
        } else {
            result = Array<T>::doSpecialAction(ba, theq, thebqe, gvalues);
        }

        return result;
    }

    vector<T>* getPointer(string spec) {
        if ((spec == "vec")  || (spec == "vec1")) {
            return &(Array<T>::vec);
        } else if (spec == "vec2") {
            return &vec2;
        } else if (spec == "vec3") {
            return &vec3;
        } else {
            return &vhlp;
        }
    }
    virtual int doSpecialActionT(Boxaction ba, deque<Boxqentry>& theq, Boxqentry& thebqe, map<string,string>& gvalues) {
        return 1;
    }


    protected:
        deque<vector<T>> qarray;
        vector<T> vec2;
        vector<T> vec3;
        T hlp;
        vector<T> vhlp;
        vector<T>* pt;
    private:

};

template <> void QueueArray<string>::setHlp(string value) {
    hlp = value;
}

template <> void QueueArray<long long int>::setHlp(string value) {
    hlp = stoll(value);
}

template <> void QueueArray<long double>::setHlp(string value) {
    hlp = stold(value);
}

template <> void QueueArray<string>::setHlpT(string value) {
    hlp = value;
}

template <> void QueueArray<long long int>::setHlpT(long long int value) {
    hlp = value;
}

template <> void QueueArray<long double>::setHlpT(long double value) {
    hlp = value;
}

template <> void QueueArray<string>::setVarFromT(string name, string value) {
    string x;
    int vindex;
    if (debug_R) {log.logmsg4(DEBUG, MSG4, "setVarFromT", name, "v", "int");}
    if (Box::bxb.startsWith(name, "vec2@")) {
        x = name.substr(5);
        if (Box::bxb.isLong(x)) {
            vindex = stoi(x) - 1;
            vec2.at(vindex) = value;
        }
    } else if (Box::bxb.startsWith(name, "vec3@")) {
        x = name.substr(5);
        if (Box::bxb.isLong(x)) {
            vindex = stoi(x) - 1;
            vec3.at(vindex) = value;
        }
    } else {
        Array<string>::setVarFromT(name, value);
    }
};

template <> void QueueArray<long long int>::setVarFromT(string name, string value) {
    string x;
    int vindex;
    if (debug_R) {log.logmsg4(DEBUG, MSG4, "setVarFromT", name, "v", "int");}
    if (Box::bxb.startsWith(name, "vec2@")) {
        x = name.substr(5);
        if (Box::bxb.isLong(x)) {
            vindex = stoi(x) - 1;
            vec2.at(vindex) = stoll(value);
        }
    } else if (Box::bxb.startsWith(name, "vec3@")) {
        x = name.substr(5);
        if (Box::bxb.isLong(x)) {
            vindex = stoi(x) - 1;
            vec3.at(vindex) = stoll(value);
        }
    } else {
        Array<long long int>::setVarFromT(name, value);
    }
};


template <> void QueueArray<long double>::setVarFromT(string name, string value) {
    string x;
    int vindex;
    if (debug_R) {log.logmsg4(DEBUG, MSG4, "setVarFromT", name, "v", "float");}
    if (Box::bxb.startsWith(name, "vec2@")) {
        x = name.substr(5);
        if (Box::bxb.isLong(x)) {
            vindex = stoi(x) - 1;
            vec2.at(vindex) = stold(value);
        }
    } else if (Box::bxb.startsWith(name, "vec3@")) {
        x = name.substr(5);
        if (Box::bxb.isLong(x)) {
            vindex = stoi(x) - 1;
            vec3.at(vindex) = stold(value);
        }
    } else {
        Array<long double>::setVarFromT(name, value);
    }
};

template <> void QueueArray<string>::setVarDefault(string name) {
        if (debug_I) {log.logmsg4(DEBUG, MSG4, "setVarDefault", name, myclassname, "string");}
        Array<string>::setVarDefault(name);
    }

template <> void QueueArray<long long int>::setVarDefault(string name) {
        if (debug_I) {log.logmsg4(DEBUG, MSG4, "setVarDefault", name, myclassname, "int");}
        Array<long long int>::setVarDefault(name);
    }

template <> void QueueArray<long double>::setVarDefault(string name) {
        if (debug_I) {log.logmsg4(DEBUG, MSG4, "setVarDefault", name, myclassname, "float");}
        Array<long double>::setVarDefault(name);
    }

 template <> int QueueArray<string>::doSpecialActionT(Boxaction ba, deque<Boxqentry>& theq, Boxqentry& thebqe, map<string,string>& gvalues) {
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

        if (action == "???") {
            v = ba.getArguments();
            v = resolveAll(v, thebqe, gvalues);
        }  else {
           Array<string>::doSpecialActionT(ba, theq, thebqe, gvalues);
        }

        return result;
    }


template <> int QueueArray<long long int>::doSpecialActionT(Boxaction ba, deque<Boxqentry>& theq, Boxqentry& thebqe, map<string,string>& gvalues) {
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

        if (action == "???") {
            v = ba.getArguments();
            v = resolveAll(v, thebqe, gvalues);
        }  else {
           Array<long long int>::doSpecialActionT(ba, theq, thebqe, gvalues);
        }

        return result;
    }

template <> int QueueArray<long double>::doSpecialActionT(Boxaction ba, deque<Boxqentry>& theq, Boxqentry& thebqe, map<string,string>& gvalues) {
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

        if (action == "???") {
            v = ba.getArguments();
            v = resolveAll(v, thebqe, gvalues);
        }  else {
           Array<long double>::doSpecialActionT(ba, theq, thebqe, gvalues);
        }

        return result;
    }



#endif // QUEUEARRAY_HXX
