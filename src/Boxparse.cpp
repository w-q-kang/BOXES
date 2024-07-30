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

#include "globals.h"
#include "Boxparse.h"
#include <algorithm>
#include "Scalar.hxx"
#include "Triple.hxx"
#include "Quintuple.hxx"
#include "Septuple.hxx"
#include "Decuple.hxx"
#include "Array.hxx"
#include "MachineField.hxx"
#include "MachineNet.hxx"
#include "QueueArray.hxx"


using namespace std;

Boxparse::Boxparse()
{
    //ctor
}

Boxparse::~Boxparse()
{
    //dtor
}
void Boxparse::setSymbols(vector<string>& psymbols) {
    symbols = psymbols;
}

bool Boxparse::doParse() {
    bool result = true;
    string info = "start";
    int from = 0;
    int ipointer = 0;
    int to = symbols.size() - 1;
    int nextp = 0;
    while ((from <= to) && result) {
        Partition p = findNextPart(from, to, {"box"}, {"end"}, false, false);
        info = symbols[p.from + 1];
        Box* newbox = new Box();
        if (!p.hasErrors) {
            string name = symbols[p.from + 1];
            string structure = symbols[p.from + 2];
            string typ = symbols[p.from + 3];
            string subtyp = "";
            nextp = p.from + 4;
            if (structure == "scalar") {
                if (typ == "string") {
                    newbox = new Scalar<string>();
                    newbox->init(name,structure, "", typ);
                } else if (typ == "int") {
                    newbox = new Scalar<long long int>();
                    newbox->init(name,structure, "", typ);
                } else if (typ == "float") {
                    newbox = new Scalar<long double>();
                    newbox->init(name,structure, "", typ);
                }
                if (debug_P) {log.logmsg4(DEBUG, MSG4, "added", structure, typ, "");}
            } else if (structure == "triple") {
                if (typ == "string") {
                    newbox = new Triple<string>();
                    newbox->init(name,structure, "", typ);
                } else if (typ == "int") {
                    newbox = new Triple<long long int>();
                    newbox->init(name,structure, "", typ);
                } else if (typ == "float") {
                    newbox = new Triple<long double>();
                    newbox->init(name,structure, "", typ);
                }
                if (debug_P) {log.logmsg4(DEBUG, MSG4, "added", structure, typ, "");}
            } else if (structure == "quintuple") {
                if (typ == "string") {
                    newbox = new Quintuple<string>();
                    newbox->init(name,structure, "", typ);
                } else if (typ == "int") {
                    newbox = new Quintuple<long long int>();
                    newbox->init(name,structure, "", typ);
                } else if (typ == "float") {
                    newbox = new Quintuple<long double>();
                    newbox->init(name,structure, "", typ);
                }
                if (debug_P) {log.logmsg4(DEBUG, MSG4, "added", structure, typ, "");}
            } else if (structure == "septuple") {
                if (typ == "string") {
                    newbox = new Septuple<string>();
                    newbox->init(name,structure, "", typ);
                } else if (typ == "int") {
                    newbox = new Septuple<long long int>();
                    newbox->init(name,structure, "", typ);
                } else if (typ == "float") {
                    newbox = new Septuple<long double>();
                    newbox->init(name,structure, "", typ);
                }
                if (debug_P) {log.logmsg4(DEBUG, MSG4, "added", structure, typ, "");}
            }  else if (structure == "decuple") {
                if (typ == "string") {
                    newbox = new Decuple<string>();
                    newbox->init(name,structure, "", typ);
                } else if (typ == "int") {
                    newbox = new Decuple<long long int>();
                    newbox->init(name,structure, "", typ);
                } else if (typ == "float") {
                    newbox = new Decuple<long double>();
                    newbox->init(name,structure, "", typ);
                }
                if (debug_P) {log.logmsg4(DEBUG, MSG4, "added", structure, typ, "");}
            } else if (structure == "array") {
                if (typ == "string") {
                    newbox = new Array<string>();
                    newbox->init(name,structure, "", typ);
                } else if (typ == "int") {
                    newbox = new Array<long long int>();
                    newbox->init(name,structure, "", typ);
                } else if (typ == "float") {
                    newbox = new Array<long double>();
                    newbox->init(name,structure, "", typ);
                }
                if (debug_P) {log.logmsg4(DEBUG, MSG4, "added", structure, typ, "");}
            } else if (structure == "machine") {
                subtyp = symbols[p.from + 4];
                nextp = p.from + 5;
                if (typ == "field") {
                    if (subtyp == "string") {
                        newbox = new MachineField<string>();
                        newbox->init(name,structure, typ, subtyp);
                    } else if (subtyp == "int") {
                        newbox = new MachineField<long long int>();
                        newbox->init(name,structure, typ, subtyp);
                    }
                    if (debug_P) {log.logmsg4(DEBUG, MSG4, "added", structure + typ, subtyp, "");}
                } else if (typ == "net") {
                    if (subtyp == "string") {
                        newbox = new MachineNet<string>();
                        newbox->init(name,structure, typ, subtyp);
                    } else if (subtyp == "int") {
                        newbox = new MachineNet<long long int>();
                        newbox->init(name,structure, typ, subtyp);
                    }
                    if (debug_P) {log.logmsg4(DEBUG, MSG4, "added", structure + typ, subtyp, "");}
                }
            } else if (structure == "queue") {
                subtyp = symbols[p.from + 4];
                nextp = p.from + 5;
                if (typ == "array") {
                    if (subtyp == "string") {
                        newbox = new QueueArray<string>();
                        newbox->init(name,structure, typ, subtyp);
                    } else if (subtyp == "int") {
                        newbox = new QueueArray<long long int>();
                        newbox->init(name,structure, typ, subtyp);
                    } else if (subtyp == "float") {
                        newbox = new QueueArray<long double>();
                        newbox->init(name,structure, typ, subtyp);
                    }
                    if (debug_P) {log.logmsg4(DEBUG, MSG4, "added", structure + typ, subtyp, "");}
                }
            } else {
                p.msg = STRUCTTYPUNKNOWN;
                p.hasErrors = true;
                log.logmsg4(ERROR, p.msg, structure, typ, subtyp, " ");
                result = false;
                break;
            }

            ipointer++;
            result = doParseResponses(newbox, ipointer, nextp, p.to - 1);
            pointers[ipointer] = newbox;

            if (debug_P) {log.logmsg4(DEBUG, MSG4, "added " + name + " as pointer", to_string(ipointer), to_string(p.from), to_string(p.to));}
        } else {
            string help = "";
            if (p.from >= 0) {help += symbols[p.from];}
            if (p.to >= 0) {help += ".." + symbols[p.to];}
            log.logmsg4(ERROR, p.msg, help, "in box", info, "");
            result = false;
            break;
        }
        from = p.to + 1;
    }

    if (debug_C) {
        log.logmsg4(DEBUG, MSG4, "chaining:", "", "", "");
        for (int j = 0; j < (int) chain.size(); ++j) {
            log.logmsg4(DEBUG, MSG4, "chain", chain.at(j), "", "");
        }
    }
    return result;
}

bool Boxparse::doParseResponses(Box* box, int ip, int pfrom, int pto) {
    bool result = true;
    int from = pfrom;
    int to = pto;
    string info = "<unknown tag>";
    vector<Boxaction> a;
    string help;
    while (from <= to) {
        Partition p = findNextPart(from, to, {"on"}, {"off"}, false, false);
        if ((int) symbols.size() > p.from + 1) {info = symbols[p.from + 1];}
        if (!p.hasErrors) {
            string name = symbols[p.from + 1];
            a.clear();
            chainlinks = "";
            a = splitResponse(p.from + 2, p.to);
            box->setResponse(name, a);
            if (chainlinks.size() > 0) {chain.push_back(name + chainlinks);}
            addQref(name, ip);
            if (debug_P) {log.logmsg4(DEBUG, MSG4, "response (" + to_string(ip) + ")", name, to_string(p.from), to_string(p.to));}
        } else {
            help = "";
            if (p.from >= 0) {help += symbols[p.from];}
            if (p.to >= 0) {help += ".." + symbols[p.to];}
            log.logmsg4(ERROR, p.msg, help, info, p.content(false), " ");
            result = false;
            break;
        }
        from = p.to + 1;
    }
    return result;
}
vector<Boxaction> Boxparse::splitResponse(int from, int to) {
    vector<Boxaction> a;
    vector<string> w;
    vector<string> v;
    int oldfrom = from;
    string info = "<unknown action>";
    while (from <= to) {
        Partition p = checkNextAction(from, to,  {"|", "off"}, false);

        if ((int) symbols.size() > p.from) {info = symbols[p.from];}
        if (!p.hasErrors) {

            string name = symbols[p.from];

            v.clear();
            if (b.findInString('.', name)) {
                v= b.split2(name, '.', '.');
                name = v[0];
             }

           // check action
           if  (!b.contains(name, actionwords) && ((name.length() == 1) || (b.first(name)!= '!'))) {
               log.logmsg4(ERROR, UNKNOWNACTION, name, " ", " ", " ");
               p.hasErrors = true;
           }

           if (v.size() > 1 ) {
                for (int j = 1; j < (int) v.size(); ++j) {
                    string test = v[j];
                    if (!b.contains(test, augmentations)) {
                        if ((!b.isLong(test)) && (!b.startsWith(test, "!"))) {
                            log.logmsg4(WARN, AUGUNKNOWN, v[j], name, " ", " ");
                            p.hasErrors = true;
                        }
                    }
                }
            }

             //if (b.contains(name, {"send", "do"})) {
             if ((name =="send") or (name =="do")) {
                chainlinks+= " - " + symbols[p.from + 1];
             }
            w.clear();

            for (int i = p.from + 1; i < p.to; ++i) {
                w.push_back(symbols[i]);
            }

            Boxaction ba = Boxaction(name);
            ba.setArguments(w);
            if (v.size() > 1) {ba.setAug(v[1]);}
            if (v.size() > 2) {ba.setAug(v[2]);}
            if (v.size() > 3) {ba.setAug(v[3]);}
            a.push_back(ba);

            if (debug_P) {log.logmsg4(DEBUG, MSG4, "action/augs", name, "", to_string(v.size()));}
        } else {
             log.logmsg4(ERROR, p.msg, "action", info, symbols[oldfrom-1] + "-" + symbols[to], " ");
             p.hasErrors = true;
            break;
        }
        from = p.to + 1;
    }
    return a;
}
Partition Boxparse::findNextPart(int from, int to, vector<string> starttags, vector<string> endtags, bool canNest, bool special) {
    // special is true for actions
    // - recognize !var as a valid action
    // - recognize action.opt as valid
    Partition p;

    p.hasErrors = false;
    p.hasPrependedSymbols = false;
    int i = from;
    int level = 0;

    string text;
    string text1;
    vector<string> v;
    while (i <= to) {
        text = symbols[i];
        text1 = text;
        if (debug_P) {log.logmsg4(DEBUG, MSG4, "findNextPart", to_string(starttags.size()), to_string(endtags.size()), text);}
        v.clear();
        if (special && (text.length() > 2)) {
            v = b.split2(text, '.', '.');
            text1 = v[0];
        }
        auto pos = find(starttags.begin(), starttags.end(), text1);
        if  ((pos != starttags.end()) || (special && ((symbols[i]).length() > 1) && (b.first(symbols[i])== '!') )) {
            if (debug_P) {log.logmsg4(DEBUG, MSG4, "findNextPart1", "success", text, to_string(i));}
            if (v.size() > 1 ) {
                for (int j = 1; j < (int) v.size(); ++j) {
                    string test = v[j];
                    if (!b.contains(test, augmentations)) {
                        if ((!b.isLong(test)) && (!b.startsWith(test, "!"))) {
                            log.logmsg4(WARN, AUGUNKNOWN, v[j], v[0], " ", " ");
                        }
                    }
                }
            }
            level++;
            if (level > 1) {
                if (!canNest) {
                    p.hasErrors = true;
                    p.msg = CANTBENESTED;
                    break;
                }
            } else {
                p.from = i;
                if (p.from > from) {
                    p.hasPrependedSymbols = true;
                    if (debug_P) {log.logmsg4(DEBUG, MSG4, "findNextPart1", "success - with", to_string(p.from - from), "prepending symbols");}
                }
            }
        }
        pos = find(endtags.begin(), endtags.end(), text);
        if  (pos != endtags.end()) {
            level--;
            if (debug_P) {log.logmsg4(DEBUG, MSG4, "findNextPart2", "success", text, to_string(i) + "/" +to_string(level));}
            if (level == 0) {
                p.to = i;
                if (p.to < to) {
                    p.hasFollowingSymbols = true;
                }
                break;
            }
        }

        i++;
    }

    if (!p.hasErrors) {
        if (p.from < 0) {
            p.hasErrors = true;
            p.msg = CANTFINDPARTBEGIN;
        } else if (p.to < 0) {
            p.hasErrors = true;
            if (p.hasPrependedSymbols) {
                p.msg = CANTFINDPARTPP;
            } else {
                p.msg = CANTFINDPARTEND;
            }
         }
    }

    return p;
}


Partition Boxparse::checkNextAction(int from,int to, vector<string> endtags, bool canNest) {
    Partition p;
    p.hasErrors = false;
    p.hasPrependedSymbols = false;
    p.from = from;

    int i = from + 1;
    string text;
    if (debug_P) {log.logmsg4(DEBUG, MSG4, "BeginPartAction", "success", symbols[p.from], to_string(p.from));}
    while (i <= to) {
        text = symbols[i];
        auto pos = find(endtags.begin(), endtags.end(), text);
        if  (pos != endtags.end()) {
            if (debug_P) {log.logmsg4(DEBUG, MSG4, "findEndPartAction", "success", text, to_string(i));}
            p.to = i;
            if (p.to < to) {
                p.hasFollowingSymbols = true;
            }
            break;
         }

        i++;
    }

    if (!p.hasErrors) {
        if (p.from < 0) {
            p.hasErrors = true;
            p.msg = CANTFINDPARTBEGIN;
        } else if (p.to < 0) {
            p.hasErrors = true;
            if (p.hasPrependedSymbols) {
                p.msg = CANTFINDPARTPP;
            } else {
                p.msg = CANTFINDPARTEND;
            }
         }
    }

    return p;
}
void Boxparse::addQref(string name, int index) {
    vector<int> refs;
    refs.clear();
    if (qref.find(name) != qref.end()) {
        refs = qref[name];
        if (debug_P) {log.logmsg4(DEBUG, MSG4, "qref found", name, to_string(refs.size()), "");}
    }
    refs.push_back(index);
    qref[name] = refs;
    if (debug_P) {log.logmsg4(DEBUG, MSG4, "qref added", name, to_string(refs.size()), "");}
}

vector<int> Boxparse::getQref(string name) {
    if (debug_P) {log.logmsg4(DEBUG, MSG4, "boxparse", "getQref", name, "");}
    vector<int> result;
    if (qref.find(name) != qref.end()) {
        result = qref[name];
    }

    return result;
};

Box* Boxparse::getPointer(int idx) {
    try {
        return pointers[idx];
    } catch (exception& e) {
        //Boxbasics b;
        log.logmsg4(FATAL, MSG4, b.cstos(e.what()), "when getting Boxparse pointer",  to_string(idx), " ");
        return new Box();
    }
}
