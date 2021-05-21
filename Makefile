.SILENT: clean
.PHONY: linux

NAME ?= app
CFLAGS ?= -W -Wall -lcurl -ljson-c
SRC_FILES := $(wildcard *.c)

all: linux
	./$(NAME)

linux: $(NAME).c
	clear
	gcc -o $(NAME) $(SRC_FILES) $(CFLAGS)

clean:
	rm -rf $(NAME)
# CC=gcc
# CFLAGS=-I.

# SRC_DIR=./src
# OBJ_DIR=./obj
# SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
# OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# main.exe: $(OBJ_FILES) 
# 	gcc $(LDFLAGS) -o $@ $^

# $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
# 	@mkdir -p obj
# 	gcc $(CFLAGS) -c -o $@ $<