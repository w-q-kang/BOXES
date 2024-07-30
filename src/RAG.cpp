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

#include "RAG.h"
#include <string>
#include <vector>
#include<math.h>
#include "Boxlog.h"

using namespace std;

RAG::RAG()
{
    //ctor
    noRanges = {".,."};
}

RAG::~RAG()
{
    //dtor
}

vector<string> RAG::run(vector<string>& tokens) {
    vector<string> left;
    vector<string> right;
    vector<string> result;
    vector<string> total;
    string op = "";
    int phase = 1;
    int i = 0;
    while(i< (int) tokens.size()) {
        string token = tokens[i];
        if (isUnary(token) || isInfixRR(token)|| isInfixRV(token)) {
            op = token;
            phase = 2;
        } else if (isSep(token)) {
            bool rev = isReversed(op);
            string myop = rev ? op.substr(1) : op;
            if (rev) {
                result = doOps(myop, right, left, rev);
            } else {
                result = doOps(myop, left, right, rev);
            }
            if (token == "::") {
                addToVector(total, result);
                left.clear();
                right.clear();
                result.clear();
            } else {
                left.clear();
                addToVector(left, result);
                right.clear();
                result.clear();
            }
            phase = 1;
        } else if (phase == 1) {
            left.push_back(token);
        } else if (phase == 2) {
            right.push_back(token);
        }
        i++;
    }

    return total;
}

bool RAG::isUnary(string token) {
    return contains(token, (vector<string>) {"size", "~size"});
}

bool RAG::isInfixRR(string token) {
    return contains(token, (vector<string>){"+", "-", "&", "~+", "~-", "~&"});
}

bool RAG::isInfixRV(string token) {
    return contains(token, (vector<string>){"<", ">", "==", "<>", "<=", ">=", "~<", "~>", "~==", "~<>", "~<=", "~>="});
}

bool RAG::isSep(string token) {
    return contains(token, (vector<string>){":", "::"});
}
bool RAG::isReversed(string token) {
    return token[0] == '~';
}
vector<string> RAG::doOps(string op, vector<string>& left, vector<string>& right, bool reversed) {
    string res = "";
    bool isOk = true;
    vector<string> result;
    vector<string> r;
    if (isUnary(op) ) {
        if (reversed && (right.size() > 0)) {
            log.logmsg4(ERROR, RAGNORIGHTALLOWED, op, to_string(right.size()), "", "");
            return noRanges;
        } else if (left.size() > 0) {
            log.logmsg4(ERROR, RAGNOLEFTALLOWED, op, to_string(left.size()), "", "");
            return noRanges;
        } else {
            for (int i = 0; i < (int) right.size(); ++i) {
                tie(res,isOk) = doUnary(op, right[i]);
                if (!isOk) {break;}
                result.push_back(res);
            }
        }
    } else if (isInfixRR(op)) {
        for (int i = 0; i < (int) left.size(); ++i) {
            for (int j = 0; j < (int) right.size(); ++j) {
                tie(r,isOk) = doInfixRR(left[i], op, right[j]);
                if (!isOk) {break;}
                addToVector(result, r);
            }
            if (!isOk) {break;}
        }
    } else if (isInfixRV(op)) {
        for (int i = 0; i < (int) left.size(); ++i) {
            for (int j = 0; j < (int) right.size(); ++j) {
                tie(r,isOk) = doInfixRV(left[i], op, right[j]);
                if (!isOk) {break;}
                addToVector(result, r);
            }
            if (!isOk) {break;}
        }
    }
    if (!isOk) {result = noRanges;}
    return result;
}

tuple<vector<string>, bool> RAG::doInfixRR(string left, string op, string right) {
    if (debug_o) {log.logmsg4(DEBUG, MSG4, "RAG infix RR", left, op, right);}
    vector<string> result;
    result.clear();
    bool ok = true;

    if ((left == ".,.") || (right == ".,.")) {
        return make_tuple(result, false);
    }

    vector<string> v = split2(left, ',', ',');
    vector<string> w = split2(right, ',', ',');
    long long int l1 = stoll(v[0]);
    long long int l2 = stoll(v[1]);
    long long int r1 = stoll(w[0]);
    long long int r2 = stoll(w[1]);

    if (op == "+") {
        if ((r1 > l2) || (l1 > r2)) {
            result.push_back(left);
            result.push_back(right);
        } else if ((l1 <= r1) && (l2 >= r1)) {
            result.push_back(to_string(l1) + "," + to_string(max(l2,r2)));
        } else if ((r1 <= l1) && (r2 >= l1)) {
            result.push_back(to_string(r1) + "," + to_string(max(l2,r2)));
        } else {
            log.logmsg4(ERROR, RAGCASENOTHANDLED, op, left, right, "");
            ok = false;
        }
    } else if (op == "-") {
        if ((r1 > l2) || (l1 > r2)) {
            result.push_back(left);
        } else if (r1 <= l1) {
            if (r2 < l2) {
                result.push_back(to_string(r2+1) + "," + to_string(l2));
            } else {
                ok = false;
            }
        } else if (r1 >= l1) {
            if (r2 >= l2) {
                result.push_back(to_string(l1) + "," + to_string(r1-1));
            } else {
                result.push_back(to_string(l1) + "," + to_string(r1-1));
                result.push_back(to_string(r2+1) + "," + to_string(l2));
            }
        } else {
            log.logmsg4(ERROR, RAGCASENOTHANDLED, op, left, right, "");
            ok = false;
        }
    }  else if (op == "&") {
        if ((r1 > l2) || (l1 > r2)) {
            ok = false;
        } else {
            result.push_back(to_string(max(l1,r1)) + "," + to_string(min(l2,r2)));
        }
    } else {
        log.logmsg4(ERROR, RAGOPUNKNOWN, op, to_string(right.size()), "", "");
        ok = false;
    }

    return make_tuple(result, ok);
}

tuple<vector<string>, bool> RAG::doInfixRV(string left, string op, string right) {
    if (debug_o) {log.logmsg4(DEBUG, MSG4, "RAG infix RV", left, op, right);}
    vector<string> result;
    result.clear();
    bool ok = true;
    vector<string> v = split2(left, ',', ',');
    long long int l1 = stoll(v[0]);
    long long int l2 = stoll(v[1]);
    long long int r = stoll(right);

    if (op == ">") {
        if (r >= l2) {
            ok = false;
        } else if (r<l1) {
            result.push_back(left);
        } else {
            result.push_back(to_string(r+1) + "," + to_string(l2));
        }
    } else if (op == "<") {
        if (r <= l1) {
            ok = false;
        } else if (r>l2) {
            result.push_back(left);
        } else {
            result.push_back(to_string(l1) + "," + to_string(r-1));
        }
    } else if (op == ">=") {
        if (r > l2) {
            ok = false;
        } else if (r<=l1) {
            result.push_back(left);
        } else {
            result.push_back(to_string(r) + "," + to_string(l2));
        }
    } else if (op == "<=") {
        if (r < l1) {
            ok = false;
        } else if (r>=l2) {
            result.push_back(left);
        } else {
            result.push_back(to_string(l1) + "," + to_string(r));
        }
    } else if (op == "==") {
        if ((r<l1) || (r>l2)) {
            ok = false;
        } else {
            result.push_back(to_string(r) + "," + to_string(r));
        }
    } else if (op == "<>") {
        if ((r<l1) || (r>l2)) {
            result.push_back(left);
        } else if (r==l1) {
            if (l2 > l1) {
                result.push_back(to_string(r+1) + "," + to_string(l2));
            } else {
                ok = false;
            }
         } else if (r==l2) {
            if (l2 > l1) {
                result.push_back(to_string(l1) + "," + to_string(r-1));
            } else {
                ok = false;
            }
         } else {
             result.push_back(to_string(l1) + "," + to_string(r-1));
             result.push_back(to_string(r+1) + "," + to_string(l2));
         }
    } else {
        log.logmsg4(ERROR, RAGOPUNKNOWN, op, to_string(right.size()), "", "");
        ok = false;
    }

    if (debug_o) {
            if (ok) {log.logmsg4(DEBUG, MSG4, "RAG result", join(result, " "), "", "true");}
            if (!ok) {log.logmsg4(DEBUG, MSG4, "RAG result", join(result, " "), "", "false");}
    }
    return make_tuple(result, ok);
}

tuple<string, bool> RAG::doUnary(string op, string right) {
    if (debug_o) {log.logmsg4(DEBUG, MSG4, "RAG unary", "", op, right);}
    string result = "";
    bool ok = true;
    vector<string> v = split2(right, ',', ',');

    if (op == "size") {
        result = to_string(stoll(v[1]) - stoll(v[0]) + 1);
    } else {
        log.logmsg4(ERROR, RAGOPUNKNOWN, op, to_string(right.size()), "", "");
        return make_tuple(".,.", false);
    }

    return make_tuple(result, ok);
}

