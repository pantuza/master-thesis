#
# Makefile for Seam Carving for Content-Aware Image Resizing
#
# @author: Gustavo Pantuza
# @since: 17.05.2013
#

# Source code/headers and compilation target directories
SRC := src
LIB := lib
BIN := bin
OBJ := obj

# Documentation directory
DOC := doc

# Tests directory
TEST := test

# Compilation target (binary file)
program := tp2

# Compiler
CC := cc

# Libraries
CFLAGS := -I$(LIB)

default: help

help:	
	@echo -e "\nMAKE HELP\n"
	@echo -e "make build \t- build the entire project"
	@echo -e "make latex \t- build the documentation using latex"
	@echo -e "make clean \t- clean binary files"
	@echo -e "make help \t- print this help message"
	@echo -e "--\n"

build:	tp2
	$(CC)  $(OBJ)/$^.o -o $(BIN)/$(program) $(CFLAGS)
	@echo -e "\nBinary file placed at $(BIN)/$(program)"

tp2:	$(SRC)/$(program).c
	$(CC) -c $^ -o $(OBJ)/$(program).o $(CFLAGS)

clean:
	@echo -e "\nCleaning project binary files...\n"
	@rm -vf $(BIN)/* $(OBJ)/*
	@echo -e "--\n"
