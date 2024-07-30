if [ ! -d "bin/Debug" ]; then mkdir -p "bin/Debug"; fi
if [ ! -d "obj/src" ]; then mkdir -p "obj/src"; fi
if [ ! -d "obj/Debug/src" ]; then mkdir -p "obj/Debug/src"; fi
clang++ -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c main.cpp -o obj/Debug/main.o
clang++ -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c Box.cpp -o obj/Debug/src/Box.o
clang++ -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c Boxaction.cpp -o obj/Debug/src/Boxaction.o
clang++ -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c Boxbasics.cpp -o obj/Debug/src/Boxbasics.o
clang++ -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c Boxer.cpp -o obj/Debug/src/Boxer.o
clang++ -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c Boxlog.cpp -o obj/Debug/src/Boxlog.o
clang++ -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c Boxparse.cpp -o obj/Debug/src/Boxparse.o
clang++ -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c Boxqentry.cpp -o obj/Debug/src/Boxqentry.o
clang++ -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c Boxqueue.cpp -o obj/Debug/src/Boxqueue.o
clang++ -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c globals.cpp -o obj/Debug/src/globals.o
clang++ -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c Lrule.cpp -o obj/Debug/src/Lrule.o
clang++ -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c OPG.cpp -o obj/Debug/src/OPG.o
clang++ -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c Partition.cpp -o obj/Debug/src/Partition.o
clang++ -Wall -fexceptions -g -std=c++17 -Og -Iinclude -c RAG.cpp -o obj/Debug/src/RAG.o
clang++  -o bin/Debug/boxes obj/Debug/main.o obj/Debug/src/Box.o obj/Debug/src/Boxaction.o obj/Debug/src/Boxbasics.o obj/Debug/src/Boxer.o obj/Debug/src/Boxlog.o obj/Debug/src/Boxparse.o obj/Debug/src/Boxqentry.o obj/Debug/src/Boxqueue.o obj/Debug/src/globals.o obj/Debug/src/Lrule.o obj/Debug/src/OPG.o obj/Debug/src/Partition.o obj/Debug/src/RAG.o   
