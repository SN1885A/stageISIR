all: main_qL

qL.o: qL.c qL.h 
	gcc -c qL.c

main_qL.o: main_qL.c qL.h
	gcc -c main_qL.c

main_qL: main_qL.o qL.o
	gcc -o main_qL main_qL.o qL.o -lm

clean:
	rm -f *.o
