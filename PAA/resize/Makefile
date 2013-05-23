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
CFLAGS := -I$(LIB) -g -std=gnu99 -lm -Wall

default: help

help:	
	@echo "\nMAKE HELP\n"
	@echo "make build \t- build the entire project"
	@echo "make latex \t- build the documentation using latex"
	@echo "make clean \t- clean binary files"
	@echo "make help \t- print this help message"
	@echo "--\n"

build:	$(BIN)/$(program)

$(BIN)/$(program): $(SRC)/$(program).c $(OBJ)/file.o $(OBJ)/ppm.o $(OBJ)/dynamic.o $(OBJ)/graph.o $(OBJ)/energy.o
	$(CC) -o $@ $+ $(CFLAGS)
	@echo -e "\nBinary file placed at $(BIN)/$(program)"

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) -c $^ -o $@ $(CFLAGS)

clean:
	@echo -e "\nCleaning project binary files...\n"
	@rm -vf $(BIN)/* $(OBJ)/*
	@echo -e "--\n"

latex:	${DOC}/*.tex plot
		pdflatex -output-directory=$(TEX_PATH) $(TEX_PATH)/$(DOC_NAME).tex
		@evince $(TEX_PATH)/$(DOC_NAME).pdf
