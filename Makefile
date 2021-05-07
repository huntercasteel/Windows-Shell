# Makefile

default: MyShell

MyShell: main.o lexer.o alias.o
	gcc main.o lexer.o alias.o -o MyShell
	rm *.o

main.o: main.c
	gcc -c main.c

lexer.o: lexer.c lexer.h
	gcc -c lexer.c

alias.o: alias.c alias.h
	gcc -c alias.c