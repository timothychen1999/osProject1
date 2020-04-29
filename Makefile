all:
	sudo gcc main.c -o main.o
	chmod u+s main.o
clean:
	rm -f *.o
