CC = gcc
CFLAGS = -Wall

main: main.o parser.o database.o 

main.o: main.c parser.h database.h
parser.o: parser.c parser.h
database.o: database.c database.h


clean:
	rm *.o main
