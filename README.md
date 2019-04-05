[![LICENSE](https://img.shields.io/badge/license-Anti%20996-blue.svg)](https://github.com/996icu/996.ICU/blob/master/LICENSE)
# Hack Assembler

### A simple implementation of assembler for hack machine language in C++


compile:
```bash
g++ -g -std=c++11 -Wall -pedantic -o main.out *.cpp
```
run:
```bash
./main.out test.asm 
```
will generate test.hack (with same name)


Debug with extra info:
```bash
./main.out test.asm -v
```
All the white spaces and comments are ignored (content after // in any single line)
