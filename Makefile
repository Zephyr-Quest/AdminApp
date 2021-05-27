.SILENT: clean
.PHONY: linux run

NAME = app
OUTPUT_DIR = ./build
SRC_DIR = ./src

CFLAGS = -W -Wall -lcurl -ljson-c

SRC_FILES := $(wildcard $(SRC_DIR)/*.c)

all: linux run

linux: $(SRC_FILES)
	clear
	gcc -o $(OUTPUT_DIR)/$(NAME) $(SRC_FILES) $(CFLAGS)

run:
	$(OUTPUT_DIR)/$(NAME)
	
clean:
	rm -rf $(NAME)