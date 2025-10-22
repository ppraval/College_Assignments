# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pthread -g
LDFLAGS = -pthread

# Target executables
SERVER = server
CLIENT = client

# Source files
SERVER_SRC = server.c
CLIENT_SRC = client.c
COMMON_HEADER = common.h

# Object files
SERVER_OBJ = $(SERVER_SRC:.c=.o)
CLIENT_OBJ = $(CLIENT_SRC:.c=.o)

# Default target
all: $(SERVER) $(CLIENT)

# Build server
$(SERVER): $(SERVER_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "Server compiled successfully!"

# Build client
$(CLIENT): $(CLIENT_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "Client compiled successfully!"

# Compile object files
%.o: %.c $(COMMON_HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(SERVER) $(CLIENT) *.o
	@echo "Clean completed!"

# Run server
run-server: $(SERVER)
	./$(SERVER)

# Run client (can be run multiple times in different terminals)
run-client: $(CLIENT)
	./$(CLIENT)

# Help target
help:
	@echo "Available targets:"
	@echo "  make all         - Build both server and client"
	@echo "  make server      - Build only the server"
	@echo "  make client      - Build only the client"
	@echo "  make clean       - Remove all build files"
	@echo "  make run-server  - Compile and run the server"
	@echo "  make run-client  - Compile and run a client"
	@echo "  make help        - Display this help message"

.PHONY: all clean run-server run-client help
