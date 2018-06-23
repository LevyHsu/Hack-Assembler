# Hack Assembler

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
