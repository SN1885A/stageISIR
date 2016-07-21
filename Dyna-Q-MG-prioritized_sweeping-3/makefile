all: main_dynaMG

listMG.o: listMG.c listMG.h 
	gcc -c listMG.c

dynaMG.o: dynaMG.c dynaMG.h 
	gcc -c dynaMG.c

main_dynaMG.o: main_dynaMG.c dynaMG.h
	gcc -c main_dynaMG.c

main_dynaMG: main_dynaMG.o dynaMG.o listMG.o
	gcc -o main_dynaMG main_dynaMG.o dynaMG.o listMG.o -lm -Wall

clean:
	rm -f *.o
