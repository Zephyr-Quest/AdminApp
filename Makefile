.SILENT: clean
.PHONY: linux run

NAME = app
OUTPUT_DIR = ./build
SRC_DIR = ./src

CC = gcc
CFLAGS = -W -Wall -lcurl -ljson-c

SRC_FILES := $(wildcard $(SRC_DIR)/*.c)

all: linux run

linux: $(SRC_FILES)
	clear
	$(CC) -o $(OUTPUT_DIR)/$(NAME) $(SRC_FILES) $(CFLAGS)

run:
	$(OUTPUT_DIR)/$(NAME)

clean:
	rm -r $(OUTPUT_DIR)
