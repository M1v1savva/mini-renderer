g++ -w -Ofast -c *.cpp
g++ -w -Ofast -o main *.o
main.exe 1> err.log 2>&1
img.bmp