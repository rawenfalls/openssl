CC=gcc
FLAGS=-lssl -lcrypto
RENAME=-o chek

all:
	@$(CC) main.c $(FLAGS) $(RENAME) && ./chek 
	$(info DONE)
clean:
	@rm -rf chek
	$(info DONE)
leaks:
	@$(CC) main.c $(FLAGS) $(RENAME) && valgrind ./chek