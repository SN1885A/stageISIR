all: main_dyna

list.o: list.c list.h 
	gcc -c list.c

dyna.o: dyna.c dyna.h list.h
	gcc -c dyna.c 

main_dyna.o: main_dyna.c dyna.h list.h
	gcc -c main_dyna.c

main_dyna: main_dyna.o dyna.o list.o
	gcc -o main_dyna main_dyna.o dyna.o list.o -lm

clean:
	rm -f *.o
