# Compiler to use
CC=gcc

# Compilation flags
CFLAGS= -O2 -g -Wall -pipe

# Directory lists
SRC=src
INCLUDES=includes

# Target executable
TARGET=resolver

$(TARGET): $(SRC)/main.c $(INCLUDES)/domaininfo.h
	$(CC) $(CFLAGS) -I $(INCLUDES) -o $(TARGET) $(SRC)/main.c

clean:
	@$(RM) -rf $(TARGET)