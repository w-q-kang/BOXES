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

#ifndef ENUMS_H
#define ENUMS_H

enum Level {DEBUG, INFO, WARN, ERROR, FATAL};

enum Lextype {NORMAL, APPEND};

enum LogMsg {
    // generic
    MSG1, MSG2, MSG3, MSG4,
    // preprocess
    UNKNOWNFILE, UNKNOWNOPTION, CURRENTVERSION,
    // parsing
    CANTBENESTED, CANTFINDPARTBEGIN, CANTFINDPARTEND, CANTFINDPARTPP,STRUCTTYPUNKNOWN,
    AUGUNKNOWN,
    // runtime
    CONDQEMPTY,UNREDUCEDOPS, UNUSEDOPS,NORESULTFOR,UNKNOWNACTION,UNKNOWNSPECIALACTION,
    HASHSTARNOTALLOWED,
    NOTADIRNODEL,DIRNOTCREATED,NOEXECONFILE,AMUSTBEGEB,UNKNOWNMATHTYPE,UNRESOLVEDMATHTYPE,
    REDEFENTRYRULE,NOTYETIMPLEMENTED,
    RAGNOLEFTALLOWED,RAGNORIGHTALLOWED, RAGOPUNKNOWN,RAGCASENOTHANDLED,
    QADELNEGINDEX,QADELUNKNOWNINDEX,CANTRESOLVEAT,CANTRESOLVEATRO,

    // debugging
    ENTRY

 };

enum Optype {OTEQUAL, OTUNEQUAL, OTLESS, OTGREATER, OTLESSEQUAL, OTGREATEREQUAL, OTCONTAINS, OTNOTCONTAINS, OTADD, OTMINUS, OTSET, OTAPPEND, OTSAME};
#endif // ENUMS_H
