g++ -ggdb -g3 -pg -Ofast -c *.cpp
g++ -ggdb -g3 -pg -Ofast -o main *.o
main.exe 1> err.log 2>&1
gprof main.exe