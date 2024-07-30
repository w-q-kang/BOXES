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

#ifndef NETRULE_H
#define NETRULE_H
#include <string>
#include "Enums.h"

using namespace std;

template <class T>
class Netrule
{
    public:
        Netrule() {
        };

        Netrule(string pcurrentstate, string pnewstate) {
            currentstate = pcurrentstate;
            newstate = pnewstate;
            noOutput = false;
            isElse = false;
        };
        virtual ~Netrule(){
            // dtor
        };

        void setOprel(string poprel) {
           if (poprel == "==") {
                oprel = OTEQUAL;
            } else if (poprel == "!=") {
                oprel = OTUNEQUAL;
            } else if (poprel == "!=") {
                oprel = OTUNEQUAL;
            } else if (poprel == ">") {
                oprel = OTGREATER;
            } else if (poprel == ">=") {
                oprel = OTGREATEREQUAL;
            } else if (poprel == "<") {
                oprel = OTLESS;
            } else if (poprel == "<=") {
                oprel = OTLESSEQUAL;
            } else if (poprel == "=~") {
                oprel = OTCONTAINS;
            } else if (poprel == "!~") {
                oprel = OTNOTCONTAINS;
            }
        }

        void setOp(string pop) {
            if (pop == "=") {
                op = OTSET;
            } else if (pop == "+") {
                op = OTADD;
            } else if (pop == "-") {
                op = OTMINUS;
            } else if (pop == "&") {
                op = OTAPPEND;
            }
        }

        virtual string key(string sep) {
            return currentstate;
        }

        virtual void setRule(string pinput, string poprel, string poldvalue, string pop, string pnewvalue, string poutput) {

        }

        virtual void setElserule(string pop, string pnewvalue, string poutput) {
        }

        virtual bool isvalid(string pcurrentstate, string pinput) {
            return false;
        }
		string currentstate;
		string input;
		Optype oprel;
		T relvalue; // fixed value for comparison
		string newstate;
		Optype op;
		T newvalue;
		string output;
		bool noOutput;
        bool isElse;
    protected:

    private:

};


template <> void Netrule<string>::setRule(string pinput, string poprel, string prelvalue, string pop, string pnewvalue, string poutput) {
    input = pinput;
    setOprel(poprel);
    setOp(pop);

    relvalue = prelvalue;
    newvalue = stoll(pnewvalue);
    if (poutput.size() > 0) {
        output = poutput;
    } else {
        noOutput = true;
    }
 }

template <> void Netrule<long long int>::setRule(string pinput, string poprel, string prelvalue, string pop, string pnewvalue, string poutput) {
    input = pinput;
    setOprel(poprel);
    setOp(pop);
    relvalue = stoll(prelvalue);
    newvalue = stoll(pnewvalue);
    if (poutput.size() > 0) {
        output = poutput;
    } else {
        noOutput = true;
    }
}

template <> void Netrule<long double>::setRule(string pinput, string poprel, string prelvalue, string pop, string pnewvalue, string poutput) {
    input = pinput;
    setOprel(poprel);
    setOp(pop);
    relvalue = stold(prelvalue);
    newvalue = stold(pnewvalue);
    if (poutput.size() > 0) {
        output = poutput;
    } else {
        noOutput = true;
    }
}

template <> void Netrule<string>::setElserule(string pop, string pnewvalue, string poutput) {
    isElse = true;
    setOp(pop);
    newvalue = stoll(pnewvalue);

    if (poutput.size() > 0) {
        output = poutput;
    } else {
        noOutput = true;
    }
 }

template <> void Netrule<long long int>::setElserule(string pop, string pnewvalue, string poutput) {
    isElse = true;
    setOp(pop);
    newvalue = stoll(pnewvalue);

    if (poutput.size() > 0) {
        output = poutput;
    } else {
        noOutput = true;
    }
}

template <> void Netrule<long double>::setElserule(string pop, string pnewvalue, string poutput) {
    isElse = true;
    setOp(pop);
    newvalue = stold(pnewvalue);

    if (poutput.size() > 0) {
        output = poutput;
    } else {
        noOutput = true;
    }
}

template <> bool Netrule<string>::isvalid(string pcurrentstate, string pinput) {
        if (pcurrentstate != currentstate) { return false;}
        if (isElse) {return true;}

        if (oprel == OTEQUAL) {
            return pinput == relvalue;
        } else if (oprel == OTUNEQUAL) {
            return pinput != relvalue;
        } else if (oprel == OTGREATER) {
            return pinput > relvalue;
        }else if (oprel == OTGREATEREQUAL) {
            return pinput >= relvalue;
        }else if (oprel == OTLESS) {
            return pinput < relvalue;
        }else if (oprel == OTLESSEQUAL) {
            return pinput <= relvalue;
        }else if (oprel == OTCONTAINS) {
            return pinput.find(relvalue) != std::string::npos;
        } else if (oprel == OTNOTCONTAINS) {
            return pinput.find(relvalue) == std::string::npos;
        };
        return false;
}

template <> bool Netrule<long long int>::isvalid(string pcurrentstate, string pinput) {
        if (pcurrentstate != currentstate) { return false;}
        if (isElse) {return true;}

        if (oprel == OTEQUAL) {
            return stoll(pinput) == relvalue;
        } else if (oprel == OTUNEQUAL) {
            return stoll(pinput) != relvalue;
        } else if (oprel == OTGREATER) {
            return stoll(pinput) > relvalue;
        } else if (oprel == OTGREATEREQUAL) {
            return stoll(pinput) >= relvalue;
        } else if (oprel == OTLESS) {
            return stoll(pinput) < relvalue;
        } else if (oprel == OTLESSEQUAL) {
            return stoll(pinput) <= relvalue;
        };
        return false;
}

template <> bool Netrule<long double>::isvalid(string pcurrentstate, string pinput) {
        if (pcurrentstate != currentstate) { return false;}
        if (isElse) {return true;}

        if (oprel == OTEQUAL) {
            return stold(pinput) == relvalue;
        } else if (oprel == OTUNEQUAL) {
            return stold(pinput) != relvalue;
        } else if (oprel == OTGREATER) {
            return stold(pinput) > relvalue;
        } else if (oprel == OTGREATEREQUAL) {
            return stold(pinput) >= relvalue;
        } else if (oprel == OTLESS) {
            return stold(pinput) < relvalue;
        } else if (oprel == OTLESSEQUAL) {
            return stold(pinput) <= relvalue;
        };
        return false;
}
#endif // NETRULE_H
