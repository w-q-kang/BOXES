if not exist "bin\" mkdir bin
if not exist "bin\Debug\" mkdir bin\Debug
if not exist "obj\" mkdir obj
if not exist "obj\src\" mkdir obj\src
if not exist "obj\Debug\" mkdir obj\Debug
if not exist "obj\Debug\src\" mkdir obj\Debug\src

g++.exe -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c main.cpp -o obj\Debug\main.o
g++.exe -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c Box.cpp -o obj\Debug\src\Box.o
g++.exe -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c Boxaction.cpp -o obj\Debug\src\Boxaction.o
g++.exe -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c Boxbasics.cpp -o obj\Debug\src\Boxbasics.o
g++.exe -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c Boxer.cpp -o obj\Debug\src\Boxer.o
g++.exe -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c Boxlog.cpp -o obj\Debug\src\Boxlog.o
g++.exe -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c Boxparse.cpp -o obj\Debug\src\Boxparse.o
g++.exe -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c Boxqentry.cpp -o obj\Debug\src\Boxqentry.o
g++.exe -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c Boxqueue.cpp -o obj\Debug\src\Boxqueue.o
g++.exe -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c globals.cpp -o obj\Debug\src\globals.o
g++.exe -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c Lrule.cpp -o obj\Debug\src\Lrule.o
g++.exe -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c OPG.cpp -o obj\Debug\src\OPG.o
g++.exe -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c Partition.cpp -o obj\Debug\src\Partition.o
g++.exe -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c RAG.cpp -o obj\Debug\src\RAG.o
g++.exe  -o bin\Debug\BOXES.exe obj\Debug\main.o obj\Debug\src\Box.o obj\Debug\src\Boxaction.o obj\Debug\src\Boxbasics.o obj\Debug\src\Boxer.o obj\Debug\src\Boxlog.o obj\Debug\src\Boxparse.o obj\Debug\src\Boxqentry.o obj\Debug\src\Boxqueue.o obj\Debug\src\globals.o obj\Debug\src\Lrule.o obj\Debug\src\OPG.o obj\Debug\src\Partition.o obj\Debug\src\RAG.o   
