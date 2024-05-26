all: exec

main.o: main.c header1.h
	gcc -c main.c -o main.o

fonct.o: fonct.c fonct.h
	gcc -c fonct.c -o fonct.o

festivalier.o: festivalier.c festivalier.h
	gcc -c festivalier.c -o festivalier.o

manager.o: manager.c manager.h
	gcc -c manager.c -o manager.o

exec: main.o fonct.o festivalier.o manager.o
	gcc main.o fonct.o festivalier.o manager.o -o exec

clean:
	rm -f *.o
	rm exec
