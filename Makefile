.SILENT: clean
.PHONY: linux

NAME ?= app
CFLAGS ?= -W -Wall -lcurl -ljson-c -g
SRC_FILES := $(wildcard *.c)

all: linux
	./$(NAME)

linux: $(NAME).c
	clear
	gcc -o $(NAME) $(SRC_FILES) $(CFLAGS)

clean:
	rm -rf $(NAME)