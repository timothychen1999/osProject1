all:
	@echo 'Waring, this process require su'
	@sudo gcc main.c -o main.o
	@sudo chmod u+s main.o
clean:
	@rm -f *.o
