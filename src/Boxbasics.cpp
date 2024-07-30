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

#include "Boxbasics.h"
#include <algorithm>

using namespace std;

Boxbasics::Boxbasics()
{
    //ctor
}

Boxbasics::~Boxbasics()
{
    //dtor
}

// ========== FILE HANDLING =======================================================

vector<string> Boxbasics::readFile(string name) {
   fstream fs;
   vector<string> v;
   fs.open(name,ios::in); //open a file to perform read operation using file object
   if (fs.is_open()){   //checking whether the file is open
      string tp;
      while(getline(fs, tp)){ //read data from file object and put it into string.
         if (tp.size() != 0) {v.push_back(tp);}
      }
      fs.close(); //close the file object.
   }
   return v;
}

int Boxbasics::readFileLine(string name, fstream& fsl, string &result) {
    int retcode = 1;
    result = "";
    if (!fsl.is_open()){
        fsl.open(name,ios::in);
    }
    if (fsl.is_open()){
        if(!getline(fsl, result)){
            fsl.close();
            result = "";
            retcode = 0;
        }
   }
   return retcode;
}

bool Boxbasics::existsFile(string filename) {
    bool result = false;
    if (FILE *file = fopen(filename.c_str(), "r")) {
         fclose(file);
         result = true;
    }
    return result;
}

bool Boxbasics::writeFile(string filename, vector<string> &v, bool oneline, string sep, int linelen) {
    fstream fs;
    bool result = false;
    int n;
    try {
        fs.open(filename,ios::app);
        if (fs.is_open()){
            for (int i = 0; i < (int) v.size(); ++i) {
                if (linelen > 0) {
                    n = (i+1) % linelen;
                    if (n!=1) {fs << sep;}
                    fs << v[i];
                    if (n==0) {fs << "\n";}
                } else if (oneline) {
                    if (i>0) {fs << sep;}
                    fs << v[i];
                } else {
                    fs << v[i] << "\n";
                }
            }
            if (oneline) { fs << "\n";}
        }
        fs.close(); //close the file object.
         result = true;
    } catch (exception& e) {
        result = false;
    }

    return result;
}


string Boxbasics::getDir(string s) {
    vector<string> result = split2(s, '\\', '/');
    int len = result.size();
    if(result[len-1].size()>0) {result.pop_back();}
    return join(result, "\\");
}

bool Boxbasics::isPath(string s) {
    if (s.find(':')!=string::npos){return true;}
    if (s.find('\\')!=string::npos){return true;}
    if (s.find('/')!=string::npos){return true;}
    return false;
}

// ========== VECTOR HANDLING =======================================================

bool Boxbasics::contains(string s, vector<string> v) {
        return find(v.begin(), v.end(), s) != v.end();
}

bool Boxbasics::contains(string s, vector<long long int> v) {
        return find(v.begin(), v.end(), stoll(s)) != v.end();
}

bool Boxbasics::contains(string s, vector<long double> v) {
        return find(v.begin(), v.end(), stold(s)) != v.end();
}

vector<string> Boxbasics::split2(string name, char c1, char c2) {
    vector<string> liste;
    string word = "";
    int len = name.size();

    for (int i = 0; i < len; ++i) {
        char ch = name[i];
        if ((ch == c1) || (ch == c2)){
            if (word.size() > 0) {
                liste.push_back(word);
                word = "";
            }
        } else {
            word += ch;
        }
    }

    if (word.size() > 0) {liste.push_back(word);}

    return liste;
}

vector<string> Boxbasics::split0(string name) {
    vector<string> liste;

    int len = name.size();

    for (int i = 0; i < len; ++i) {
        char ch = name[i];
        string s = "";
        s+= ch;
        liste.push_back(s);
    }

    return liste;
}

string Boxbasics::join(vector<string> &v, string sep) {
    string result = "";
    int len = v.size();
    for (int i = 0; i < len; ++i) {
        if (i>0) {result+=sep;}
        result += v[i];
    }

    return result;
}

void Boxbasics::addToVector(vector<string> &v, vector<string> &w) {
     for (int i = 0; i < (int) w.size(); ++i) {
        v.push_back(w[i]);
    }
}

void Boxbasics::addToVector(vector<long long int> &v, vector<string> &w) {
     for (int i = 0; i < (int) w.size(); ++i) {
        v.push_back(stoll(w[i]));
    }
}

void Boxbasics::addToVector(vector<long double> &v, vector<string> &w) {
     for (int i = 0; i < (int) w.size(); ++i) {
        v.push_back(stold(w[i]));
    }
}
// ========== STRING HANDLING =======================================================

bool Boxbasics::findInString(char c, string s) {
    return s.find(c) != string::npos;
}

char Boxbasics::first(string name) {
    if (name.length()==0) {
        return ' ';
    }
    return name[0];
}

bool Boxbasics::startsWith(string text, string s) {
    int len = s.length();
    if ((int) text.length() >= len) {
        if (text.substr(0, len) == s) {
            return true;
        }
    }

    return false;
}

char Boxbasics::last(string name) {
    int len = name.size();
    return name[len - 1];
}

string Boxbasics::format(string text) {
        string r = text;
        string result = "";
        int len = r.size();
        int i = 0;
        bool escaped = false;
        if ((r[0]== '"') && (r[len-1]== '"')) {
            r = r.substr(1,len-2);
        }
        while (i<(int)r.size()) {
            char c = r[i];
            if (escaped) {
                escaped = false;
                if (c == 'q') {
                    result.push_back('\"');
                } else if (c=='n') {
                    result.push_back('\n');
                } else if (c=='t') {
                    result.push_back('\t');
                } else {
                    result.push_back(c);
                }
            } else if (c != '\\') {
              result.push_back(c);
            } else {
                escaped = true;
            }
          ++i;
        }
        return result;
}

// ==================== test functions ====================

bool Boxbasics::isEmpty(string text) {
    return (text == "") || (text.size() == 0);
}

bool Boxbasics::isEmptyOrZero(string text) {
    return (text == "") || (text.size() == 0)|| (text == "0");
}

bool Boxbasics::testLong(string text) {
    bool result = true;
    try {
        for (int i = 0; i < (int) text.length(); ++i) {
            char c = text[i];
            if ((int(c) < 48 ) || (int(c) > 57)) {
                if ((i > 0) || ((c != '+') && (c != '-'))) {
                    result = false;
                    break;
                }
            }
        }
    } catch (exception& e) {
        result = false;
    }

    return result;
}

bool Boxbasics::testNum(string text) {
    bool result = true;
    int dots = 0;
    int imax = text.length() - 1;
    try {
        for (int i = 0; i <= imax; ++i) {
            char c = text[i];
            if ( c== '.') {dots++;}
            if ((int(c) < 48 ) || (int(c) > 57)) {
                if (c == '.') {
                    if ((i==imax) || (dots>1)) {
                        result = false;
                        break;
                    }
                } else if ((i > 0) || ((c != '+') && (c != '-')))  {
                    result = false;
                    break;
                }
            }
        }
    } catch (exception& e) {
        result = false;
    }

    return result;
}


bool Boxbasics::isLong(string text) {
    bool result = testLong(text);
    try {
        if (result) {
            int i __attribute__((unused)) = stoll(text);
        }
    } catch (exception& e) {
        result = false;
    }

    return result;
}

bool Boxbasics::isNumber(string text) {
    bool result = isLong(text);
    if(result) {return true;}
    result = testNum(text);

    try {
        if (result) {
            double d __attribute__((unused)) = stod(text);
        }
    } catch (exception& e) {
        result = false;
    }

    return result;
}

string Boxbasics::cstos(const char* a) {
    string s(a);
    return s;
 }
