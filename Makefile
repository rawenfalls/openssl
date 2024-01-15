CC=gcc
FLAGS=-lssl -lcrypto
NAME=check
RENAME=-o check
START = ./$(NAME)
FILES = main.c

TARGET := openssl

ifeq ($(findstring $(TARGET),$(MAKECMDGOALS)), $(TARGET))
	MAKEFLAGS += -B
endif

all:
	@$(CC) $(FILES) $(FLAGS) $(RENAME) && $(START)
	$(info DONE)
clean:
	@rm -rf $(NAME)
	$(info DONE)
leaks:
	@$(CC) $(FILES) $(FLAGS) $(RENAME) && valgrind $(START)
add:
	@git add . && git status

$(TARGET): privatekey signature publickey check

privatekey:
	openssl genrsa -out key/privatekey 2048

signature:
	openssl dgst -sha256 -sign key/privatekey -out signature/readme.signature for_check/readme.txt

publickey:
	openssl rsa -in key/privatekey -outform PEM -pubout -out key/publickey

check:
	openssl dgst -sha256 -verify key/publickey -signature signature/readme.signature for_check/readme.txt