# ----------------------------------------------------------------------------
# Makefile for CSCE 3530 – Programming Assignment 3
# ----------------------------------------------------------------------------

# The compiler to use
CC = gcc

# Compiler flags:
#  -Wall    => enable most compiler warnings
#  -Wextra  => enable even more warnings
#  -g       => include debug information
CFLAGS = -Wall -Wextra -g

# The default target will build the 'wbserver' executable.
all: wbserver

# Build the wbserver executable from wbserver.c
wbserver: wbserver.c
	$(CC) $(CFLAGS) -o wbserver wbserver.c

# 'make clean' will remove the executable
clean:
	rm -f wbserver
