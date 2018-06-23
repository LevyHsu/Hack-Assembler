# Hack Assembler

## A simple implementation of assembler for hack machine language in C++
### Link: http://www.nand2tetris.org/06.php

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
