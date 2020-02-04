g++ -ggdb -g3 -pg -O0 -c *.cpp
g++ -ggdb -g3 -pg -O0 -o main *.o
main.exe 1> err.log 2>&1
output.tga
gprof main.exe