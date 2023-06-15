# Makefile

# Compiler
CC = gcc

# Directories
SRC_DIR = .
BUILD_DIR = executable_files

# Source files
SERVER_SRC = $(SRC_DIR)/server_tcp.c $(SRC_DIR)/header.h
CLIENT_SRC = $(SRC_DIR)/client_tcp.c $(SRC_DIR)/header.h

# Object files
SERVER_OBJ = $(BUILD_DIR)/server_tcp.o
CLIENT_OBJ = $(BUILD_DIR)/client_tcp.o

# Executable files
SERVER_EXE = $(BUILD_DIR)/server
CLIENT_EXE = $(BUILD_DIR)/client

# Flags
CFLAGS = -Wall -Wextra

all: server client

server: $(SERVER_OBJ)
	$(CC) $(CFLAGS) -o $(SERVER_EXE) $(SERVER_OBJ)

client: $(CLIENT_OBJ)
	$(CC) $(CFLAGS) -o $(CLIENT_EXE) $(CLIENT_OBJ)

$(SERVER_OBJ): $(SERVER_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(CLIENT_OBJ): $(CLIENT_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all server client clean