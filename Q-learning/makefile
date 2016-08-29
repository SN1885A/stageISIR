all: main_qL

list.o: list.c list.h
	gcc -g -c list.c
	
qL.o: qL.c list.c qL.h 
	gcc -c qL.c list.c

main_qL.o: main_qL.c qL.h list.h
	gcc -c main_qL.c

main_qL: main_qL.o qL.o list.o
	gcc -o main_qL main_qL.o qL.o list.o -lm

clean:
	rm -f *.o
