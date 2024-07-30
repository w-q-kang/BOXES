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

#include "OPG.h"
#include <string>
#include <vector>
#include<math.h>

using namespace std;

OPG::OPG()
{
    //ctor
}

OPG::~OPG()
{
    //dtor
}

string OPG::run(vector<string> tokens, string stype) {
        if (debug_o) {logging.logmsg4(DEBUG, MSG4, "running OPG with type", stype, to_string(tokens.size()), "tokens:");}
		string result = "";
		string zero = "0";
		vector<string> var;
		vector<string> ops;
		int t = 0;
		int tmax = tokens.size();

		tokens.push_back("'");
		var.push_back("'");
		ops.push_back("'");
		string curr = "";
		int right = 0;
		int left = 0;
		string first = "";
		string second = "";
		string third = "";
		string val = "";
		string op = "";
		string opleft = "";
		while (t <= tmax) {
            curr = tokens[t];
			right = g(curr, stype);
			if (right == 30) {
				var.push_back(curr);
				if (debug_o) {logging.logmsg4(DEBUG, MSG4, "adding", curr, "to", "var");}
				++t;
			} else {
				opleft = ops.back();
				left = f(opleft, stype);

				if ((opleft == "(") && (curr == ")")) {
					op = ops.back();
					ops.pop_back();
					++t;
					if (debug_o) {logging.logmsg4(DEBUG, MSG4, "dropping", "()", " ", " ");}
				} else if (left < right) {
					// shift
					ops.push_back(curr);
					if (debug_o) {logging.logmsg4(DEBUG, MSG4, "adding", curr, "to ops on top of", opleft);}
					++t;
				} else if ((left == 0) && (right == 0)) {
					// accept the parse
					if (debug_o) {logging.logmsg4(DEBUG, MSG4, "accepting with", join(ops, ""), " ", " ");}
					break;
				} else {
					// reduce
					op = ops.back();
					ops.pop_back();

					if ((op == "(") && (curr == ")")) {
						// do nothing
						++t;
					} else {
						second = var.back();
						var.pop_back();
                        if (debug_o) {logging.logmsg4(DEBUG, MSG4, "calculating (" + stype + ")", first, op, second);}
						if (isUnary(op)) {
							val = calculate(op, second, zero, stype);
                            if (debug_o) {logging.logmsg4(DEBUG, MSG4, "calculated unary", val + "=", op, second);}
						} else if (isTernary(op)) {
						    third = second;
                            second = var.back();
							var.pop_back();
                            first = var.back();
							var.pop_back();
							val = calculate3(op, first, second, third, stype);
                            if (debug_o) {logging.logmsg4(DEBUG, MSG4, "calculated ternary " + val + "=", first + " " + op, second, third);}
						} else {
							first = var.back();
							var.pop_back();
							val = calculate(op, first, second, stype);
							if (debug_o) {logging.logmsg4(DEBUG, MSG4, "calculated binary " + val + "=", first , op, second);}
						}

						var.push_back(val);
						if (debug_o) {logging.logmsg4(DEBUG, MSG4, "adding result ", val, "to" , join(var, ""));}
					}
				}
			}
		}

		if (ops[ops.size()-1] == "'")  {
			ops.pop_back();
		}

		if (ops.size() > 1) {
			logging.logmsg4(ERROR, UNREDUCEDOPS, join(ops, " ") ,"" ,"", "");
		} else if (var.size() > 2) {
			logging.logmsg4(ERROR, UNUSEDOPS, join(var, " ") ,"", "", "");
		} else if (var.size() < 2) {
			logging.logmsg4(ERROR, NORESULTFOR, join(tokens, " ") ,"", "", "" );
		} else {
			result = var[1];
			if (debug_o) {logging.logmsg4(DEBUG, MSG4, "OPG result ", result, "===========" , " ");}
		}
		return result;
	}

int OPG::f(string &token, string &stype) {
    if (stype == "string") {
        return fstring(token);
    } else {
    return fnum(token);
	}
}

int OPG::g(string &token, string &stype) {
    if (stype == "string") {
        return gstring(token);
    } else {
    return gnum(token);
	}
}

int OPG::fnum(string &token) {
    int result = 0;
    if (token == "'") {
        result = 0;
    } else if (token=="or") {
        result = 2;
    } else if (token=="and") {
        result = 4;
    } else if (contains(token, (vector<string>) {"==", "<>"})) {
        result = 6;
    } else if (contains(token, (vector<string>){">=", "<=", ">", "<"})) {
        result = 8;
    } else if (contains(token, (vector<string>){"+", "-", "&"})) {
        result = 10;
    } else if (contains(token, (vector<string>){"*", "/", "%"})) {
        result = 12;
    } else if(token == "^") {
        result = 13;
    } else if (token == "not") {
        result = 15;
    } else if (token == "(") {
        result = -2;
    } else if (token == ")") {
        result = 18;
    } else if (isUnary(token)) {
        result = 15;
    } else {
        result = 30;
    }

	return result;
}

int OPG::fstring(string &token) {
    int result = 0;
    if (token == "'") {
        result = 0;
    } else if (token=="or") {
        result = 2;
    } else if (token=="and") {
        result = 4;
    } else if (contains(token, (vector<string>){"==", "<>", "max", "min"})) {
        result = 6;
    } else if (contains(token, (vector<string>){">=", "<=", ">", "<"})) {
        result = 8;
    } else if (contains(token, (vector<string>){"head", "headeq", "headge", "matchl", "sfind", "tail", "trim"})) {
        result = 10;
    } else if (contains(token, (vector<string>){"&"})) {
        result = 12;
    } else if (token == "not") {
        result = 14;
    } else if (token == "(") {
        result = -2;
    } else if (token == ")") {
        result = 28;
    } else if (isUnary(token)) {
        result = 14;
    }  else {
        result = 30;
    }

	return result;
}


int OPG::gnum(string &token) {
    int result = 0;
    if (token == "'") {
        result = 0;
    } else if (token=="or") {
        result = 1;
    } else if (token=="and") {
        result = 3;
    } else if (contains(token, (vector<string>){"==", "<>", "max", "min"})) {
        result = 5;
    } else if (contains(token, (vector<string>){">=", "<=", ">", "<"})) {
        result = 7;
    } else if (contains(token, (vector<string>){"+", "-", "&"})) {
        result = 9;
    } else if (contains(token, (vector<string>){"*", "/", "%"})) {
        result = 11;
    } else if(token == "^") {
        result = 14;
    } else if (token == "not") {
        result = 16;
    } else if (token == "(") {
        result = 18;
    } else if (token == ")") {
        result = -1;
    } else if (isUnary(token)) {
        result = 16;
    } else {
        result = 30;
    }

	return result;
}

int OPG::gstring(string &token) {
    int result = 0;
    if (token == "'") {
        result = 0;
    } else if (token=="or") {
        result = 1;
    } else if (token=="and") {
        result = 3;
    } else if (contains(token, (vector<string>){"==", "<>", "max", "min"})) {
        result = 5;
    } else if (contains(token, (vector<string>){">=", "<=", ">", "<"})) {
        result = 7;
    } else if (contains(token, (vector<string>){"head", "headeq", "headge", "matchl", "sfind", "tail", "trim"})) {
        result = 9;
    }else if (contains(token, (vector<string>){"&"})) {
        result = 11;
    } else if (token == "not") {
        result = 15;
    } else if (token == "(") {
        result = 29;
    } else if (token == ")") {
        result = -1;
    } else if (isUnary(token)) {
        result = 15;
    }  else {
        result = 30;
    }

	return result;
}

bool OPG::isUnary(string &token) {
    return contains(token, (vector<string>){"not", "abs", "ascii", "round", "ceil", "floor", "exp", "log", "sqrt", "log10", "sin", "sinh", "asin", "cos", "cosh", "acos", "tan", "tanh", "atan",
                        "size", "lower", "upper", "first", "last"});
}

bool OPG::isTernary(string &token) {
    return contains(token, (vector<string>){"?:"});
}

bool OPG::isFloatOnly(string &token) {
	return contains(token, (vector<string>){"^", "round", "ceil", "floor", "exp", "log", "sqrt", "log10", "sin", "sinh", "asin", "cos", "cosh", "acos", "tan", "tanh", "atan"});
}

bool OPG::isStringOnly(string &token) {
	return contains(token, (vector<string>){"ascii", "first", "head", "headeq", "headge", "last", "lower", "matchl", "size", "tail", "trim", "upper"});
}

bool OPG::isNotString(string &token) {
	return contains(token, (vector<string>){"abs", "+", "-", "*", "/", "%"});
}

string OPG::calculate(string &op, string &first, string &second, string &stype) {
    string result = "";

    if (stype == "string") {
        result = calculateString(op, first, second);
    } else if (stype == "int") {
        result = calculateLong(op, first, second);
    } else if (stype == "float") {
        result = calculateDouble(op, first, second);
    } else if (stype == "mixed") {
        if (isStringOnly(op)) {
            result = calculateString(op, first, second);
        } else if (isFloatOnly(op)) {
            result = calculateDouble(op, first, second);
        } else if (isLong(first) && isLong(second)) {
            result = calculateLong(op, first, second);
        } else if (isNumber(first) && isNumber(second)) {
            result = calculateDouble(op, first, second);
        } else if (!isNotString(op)) {
            result = calculateString(op, first, second);
        } else {
            logging.logmsg4(ERROR, UNRESOLVEDMATHTYPE, stype, first, op, second);
            result = "0";
        }
    } else {
        logging.logmsg4(WARN, UNKNOWNMATHTYPE, stype, "", "", "");
        result = "0";
    }

    return result;
}

string OPG::calculateLong(string &op, string &first, string &second) {
    string result = "0";
    long long int lfirst = stoll(first);
    long long int lsecond = stoll(second);

    if (op == "or") {
        result = (lfirst != 0) || (lsecond != 0) ? "1" : "0";
    } else if (op == "and") {
        result = (lfirst != 0) && (lsecond != 0) ? "1" :  "0";
    } else if (op == "==") {
        result = (lfirst == lsecond) ? "1" :  "0";
    } else if (op == "<>") {
        result = (lfirst == lsecond) ?  "0" : "1";
    } else if (op == ">=") {
        result = (lfirst >= lsecond) ? "1":  "0";
    } else if (op == "<=") {
        result = (lfirst <= lsecond) ? "1" :  "0";
    } else if (op == ">") {
        result = (lfirst > lsecond) ? "1" :  "0";
    } else if (op == "<") {
        result = (lfirst < lsecond) ? "1":  "0";
    } else if (op == "max") {
        result = (lfirst >= lsecond) ? first : second;
    } else if (op == "min") {
        result = (lfirst <= lsecond) ? first : second;
    } else if (op == "+") {
        result = to_string(lfirst + lsecond);
    } else if (op == "-") {
        result = to_string(lfirst - lsecond);
    } else if (op == "*") {
        result = to_string(lfirst * lsecond);
    } else if (op == "/") {
        result = to_string(lfirst / lsecond);
    } else if (op == "%") {
        result = to_string(lfirst % lsecond);
    } else if (op == "not") {
        result = (lfirst == 0) ? "1" :  "0";
    } else if (op == "abs") {
        result = to_string(abs(lfirst));
    } else {
        result = "0";
    }

    return result;
}


string OPG::calculateDouble(string &op, string &first, string &second) {
    string result = "0";
    long double dfirst = stold(first);
    long double dsecond = stold(second);

    if (op == "or") {
        result = (dfirst != 0) || (dsecond != 0) ? "1" : "0";
    } else if (op == "and") {
        result = (dfirst != 0) && (dsecond != 0) ? "1": "0";
    } else if (op == "==") {
        result = (dfirst == dsecond) ? "1" : "0";
    } else if (op == "<>") {
        result = (dfirst == dsecond) ? "0" : "1";
    } else if (op == ">=") {
        result = (dfirst >= dsecond) ? "1": "0";
    } else if (op == "<=") {
        result = (dfirst <= dsecond) ? "1" : "0";
    } else if (op == ">") {
        result = (dfirst > dsecond) ? "1": "0";
    } else if (op == "<") {
        result = (dfirst < dsecond) ? "1": "0";
    } else if (op == "max") {
        result = (dfirst >= dsecond) ? first : second;
    } else if (op == "min") {
        result = (dfirst <= dsecond) ? first : second;
    } else if (op == "+") {
        result = to_string(dfirst + dsecond);
    } else if (op == "-") {
        result = to_string(dfirst - dsecond);
    } else if (op == "*") {
        result = to_string(dfirst * dsecond);
    } else if (op == "/") {
        result = to_string(dfirst / dsecond);
    } else if (op == "%") {
        result = to_string((int) round(dfirst) % (int) round(dsecond));
    } else if (op == "not") {
        result = (dfirst == 0) ? "1" : "0";
    } else if (op == "abs") {
        result = to_string(abs(dfirst));
    } else if (op == "^") {
		result = to_string(pow(dfirst, dsecond));
	} else if (op == "ceil") {
		result = to_string(ceil(dfirst));
	} else if (op == "floor") {
		result = to_string(floor(dfirst));
	} else if (op == "round") {
		result = to_string(round(dfirst));
	} else if (op == "sin") {
		result = to_string(sin(dfirst));
	} else if (op == "cos") {
		result = to_string(cos(dfirst));
	} else if (op == "tan") {
		result = to_string(tan(dfirst));
	} else if (op == "sinh") {
		result = to_string(sinh(dfirst));
	} else if (op == "cosh") {
		result = to_string(cosh(dfirst));
	} else if (op == "tanh") {
		result = to_string(tanh(dfirst));
	} else if (op == "asin") {
		result = to_string(asin(dfirst));
	} else if (op == "acos") {
		result = to_string(acos(dfirst));
	} else if (op == "atan") {
		result = to_string(atan(dfirst));
	} else if (op == "exp") {
		result = to_string(exp(dfirst));
	} else if (op == "log") {
		result = to_string(log(dfirst));
	} else if (op == "log10") {
		result = to_string(log10(dfirst));
	} else if (op == "sqrt") {
		result = to_string(sqrt(dfirst));
	} else {
        result = "0";
	}
    return result;
}

string OPG::calculateString(string &op, string &first, string &second) {
    string result = "";

    if (op == "or") {
        result = !isEmptyOrZero(first) || !isEmptyOrZero(second) ? "1" : "0";
    } else if (op == "and") {
        result = !isEmptyOrZero(first) && !isEmptyOrZero(second) ? "1" : "0";
    } else if (op == "==") {
        result = (first == second) ? "1" : "0";
    } else if (op == "<>") {
        result = (first == second) ? "0" : "1";
    } else if (op == ">=") {
        result = (first >= second) ? "1" : "0";
    } else if (op == "<=") {
        result = (first <= second) ? "1" : "0";
    } else if (op == ">") {
        result = (first > second) ? "1" : "0";
    } else if (op == "<") {
        result = (first < second) ? "1" : "0";
    } else if (op == "max") {
        result = (first >= second) ? first : second;
    } else if (op == "min") {
        result = (first <= second) ? first : second;
    } else if (op == "&") {
        result = first + second;
    } else if (op == "not") {
        result = isEmptyOrZero(first) ? "1" : "0";
    } else if (op == "size") {
		result = to_string(first.size());
	} else if (op == "ascii") {
	    char c = first[0];
		result = to_string(int(c));
	} else if (op == "lower") {
	    for (int i = 0; i < (int) first.length(); i++) {
            result.push_back(tolower(first[i]));
	    }
	} else if (op == "upper") {
	    for (int i = 0; i < (int) first.length(); i++) {
            result.push_back(toupper(first[i]));
	    }
	} else if (op == "first") {
		result.push_back(first.front());
	} else if (op == "last") {
		result.push_back(first.back());
	} else if (op == "head") {
        result = first.substr(0, stoi(second));
    } else if (op == "tail") {
        result = first.substr(first.size() - stoi(second));
    } else if (op == "sfind") {
        auto pos = first.find(second);
        if (pos != string::npos) {
            result = to_string(pos + 1);
        } else {
            result = "0";
        }
    } else if (op == "trim") {
        result = first;
        while ((result.length() > 0 ) && (result.at(0) == second.at(0))) {
            result.erase(0, 1);
	    }

	    while ((result.length() > 0 ) && (result.back() == second.at(0))) {
            result.pop_back();
        }
    } else if (op == "matchl") {
        if (debug_o) {logging.logmsg4(DEBUG, MSG4, "calling match begin", first, second, " ");}
        result = (doMatchLeft(first, second)) ? "1" : "0";
    } else if (op == "headeq") {
        if (debug_o) {logging.logmsg4(DEBUG, MSG4, "calling headeq", first, second, " ");}
        result = (doHeadEq(first, second)) ? "1" : "0";
    } else if (op == "headge") {
        if (debug_o) {logging.logmsg4(DEBUG, MSG4, "calling headge", first, second, " ");}
        result = (doHeadGe(first, second)) ? "1" : "0";
    } else {
        result = first + op + second;
    }

    return result;
}

string OPG::calculateLong3(string &op, string &first, string &second, string &third) {
    string result = "";
    long long int dfirst = stoll(first);
    long long int dsecond = stoll(second);
    long long int dthird = stoll(third);

    if (op == "?:") {
        if (dfirst != 0) {
            result = to_string(dsecond);
        } else {
            result = to_string(dthird);
        }
    } else {
        result = first + op + second + third;
    }

    return result;
};

string OPG::calculateDouble3(string &op, string &first, string &second, string &third){
    string result = "";
    long double dfirst = stold(first);
    long double dsecond = stold(second);
    long double dthird = stold(third);

    if (op == "?:") {
        if (dfirst != 0L) {
            result = to_string(dsecond);
        } else {
            result = to_string(dthird);
        }
    } else {
        result = first + op + second + third;
    }

    return result;
};

string OPG::calculateString3(string &op, string &first, string &second, string &third){
    string result = "";

    if (op == "?:") {
        if (first != "0") {
            result = second;
        } else {
            result = third;
        }
    } else {
        result = first + op + second + third;
    }

    return result;
};

string OPG::calculate3(string &op, string &first, string &second, string &third, string &stype){
    string result = "";

    if ((isLong(first) && isLong(second)&& isLong(third)) || (stype == "long")) {
        result = calculateLong3(op, first, second, third);
    } else if ((isNumber(first) && isNumber(second)&& isNumber(third)) || (stype == "float")) {
        result = calculateDouble3(op, first, second, third);
    } else {
        result = calculateString3(op, first, second, third);
    }

    return result;
};


// result: number of chars from beginning that match
int OPG::doMatchFront(string a, string b) {
    int l = min(a.length(), b.length());
    int i = 0;
    int counter = 0;
    while (i<l) {
        if ((a.at(i) == b.at(i)) || (a.at(i) == '?') || (b.at(i) == '?')) {
            counter++;
        } else {
            break;
        }
    }

    return counter;
}

// true if b matches the beginning of a
bool OPG::doMatchLeft(string a, string b) {
    int l = b.size();
    if ((int) a.size() < l) {
       //logging.logmsg4(WARN, AMUSTBEGEB, a, b,"", "");
       return false;
    }
    int i = 0;
    while (i<l) {
        if ((a.at(i) != b.at(i)) && (a.at(i) != '?') && (b.at(i) != '?')) {
            return false;
        }
        i++;
    }

    return true;
}

bool OPG::doHeadEq(string a, string b) {
    int l = min(a.size(),b.size());

    int i = 0;
    while (i<l) {
        if ((a.at(i) != b.at(i))) {
            return false;
        }
        i++;
    }

    return true;
}

bool OPG::doHeadGe(string a, string b) {
    int l = min(a.size(),b.size());

    int i = 0;
    while (i<l) {
        if ((a.at(i) < b.at(i))) {
            return false;
        }
        i++;
    }

    return true;
}
