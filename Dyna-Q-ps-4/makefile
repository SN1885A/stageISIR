all: main_dynaMGA

listMGA.o: listMGA.c listMGA.h 
	gcc -c listMGA.c

dynaMGA.o: dynaMGA.c dynaMGA.h 
	gcc -c dynaMGA.c

main_dynaMGA.o: main_dynaMGA.c dynaMGA.h
	gcc -c main_dynaMGA.c

main_dynaMGA: main_dynaMGA.o dynaMGA.o listMGA.o
	gcc -o main_dynaMGA main_dynaMGA.o dynaMGA.o listMGA.o -lm -Wall

clean:
	rm -f *.o
