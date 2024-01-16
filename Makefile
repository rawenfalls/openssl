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
	@$(CC) $(FILES) $(FLAGS) $(RENAME) #&& $(START)
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
	@openssl genrsa -out key/privatekey 2048 #> /dev/null 2>&1

signature:
	@openssl dgst -sha256 -sign key/privatekey -out signature/readme.signature ts3000.ima #> /dev/null 2>&1

publickey:
	@openssl rsa -in key/privatekey -outform PEM -pubout -out key/publickey #> /dev/null 2>&1

check:
	@openssl dgst -sha256 -verify key/publickey -signature signature/readme.signature ts3000.ima #> /dev/null 2>&1

ima:
	@echo "a.ima" > a.ima
imam:
	@./merge_files.sh -q signature/readme.signature ts3000.ima	
ima2:
	@echo echo -n "" > b.ima
cp:
	@cp ../q/out/ts3000/ts3000.ima ../openssl/
dd:
	@make cp > /dev/null 2>&1 && make openssl > /dev/null 2>&1 && make imam > /dev/null 2>&1