
rm -rf build/
mkdir build/
gcc -O0 -Isrc src/linux.cpp -o game -lx11 -lGL -lGLU -lGLEW -lm -std=c++11