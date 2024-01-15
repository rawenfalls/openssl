CC=gcc
FLAGS=-lssl -lcrypto
NAME=check
RENAME=-o check
START = ./$(NAME)
FILES = main.c

all:
	@$(CC) $(FILES) $(FLAGS) $(RENAME) && $(START)
	$(info DONE)
clean:
	@rm -rf $(NAME)
	$(info DONE)
leaks:
	@$(CC) $(FILES) $(FLAGS) $(RENAME) && valgrind $(START)
add:
	git add . && git status