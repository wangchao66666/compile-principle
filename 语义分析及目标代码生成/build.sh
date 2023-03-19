#! /bin/bash
g++ -c PLLexiacl.cpp -o PLLexiacl.o -O2
g++ -c PLPLO.cpp -o PLPLO.o -O2
g++ -c main.cpp  -o main.o -O2

g++ main.o -o Compiler -lm

g++ -c interpreter.cpp -o interpreter.o -O2
g++ interpreter.o -o Interpreter -lm