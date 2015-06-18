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
OUT := out

# Documentation directory
DOC := doc

# Tests directory
TEST := test

# Compilation target (binary file)
program := tp2

# Documentation file name
docname := resize


# Compiler
CC := cc

# Libraries
CFLAGS := -I$(LIB) -g -std=gnu99 -lm -Wall -O3

default: help

help:	
	@echo -e "\nMAKE HELP\n"
	@echo -e "make all \t- build the entire project"
	@echo -e "make latex \t- build the documentation using latex"
	@echo -e "make clean \t- clean binary files"
	@echo -e "make help \t- print this help message"
	@echo -e "--\n"

all:	$(BIN)/$(program)

$(BIN)/$(program): $(SRC)/$(program).c $(OBJ)/priority.o $(OBJ)/file.o \
	$(OBJ)/ppm.o $(OBJ)/dynamic.o $(OBJ)/graph.o $(OBJ)/energy.o \
	$(OBJ)/color.o $(OBJ)/resize.o $(OBJ)/test.o 
	$(CC) -o $@ $+ $(CFLAGS)
	@echo -e "\nBinary file placed at $(BIN)/$(program)"

$(OBJ)/%.o: $(SRC)/%.c 
	$(CC) -c $^ -o $@ $(CFLAGS)

clean:
	@echo -e "\nCleaning project binary files...\n"
	@rm -vf $(BIN)/tp2 $(OBJ)/*
	@rm -vf $(OUT)/*
	@echo -e "--\n"

latex:	${DOC}/*.tex
		pdflatex -output-directory=$(DOC) $(DOC)/$(docname).tex
		@evince $(DOC)/$(docname).pdf
