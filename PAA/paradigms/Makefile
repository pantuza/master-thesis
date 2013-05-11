#
# Makefile for Algorithms Paradigms project
#
# author: Gustavo Pantuza
# since: 26.04.2013
#


# Latex dir
TEX_PATH := doc
# document output name
DOC_NAME := paradigms


# src dir
SRC := src
# Header files dir
LIB := lib
# Objects dir
OBJ := obj
# Binary target dir
BIN := bin


# Compilation Flags
CFLAGS := -I$(LIB) -g -std=c99
# Compiler
CC := gcc

# BINARY OUTPUT NAMES
BRUTE := tp1b
DYNAMIC := tp1pd
GREEDY := tp1g

all: $(BIN)/$(BRUTE) $(BIN)/$(DYNAMIC) $(BIN)/$(GREEDY)

$(BIN)/$(BRUTE): $(OBJ)/file.o $(OBJ)/brute.o
	$(CC) -o $(BIN)/$(BRUTE) $^ $(CFLAGS)

$(BIN)/$(DYNAMIC): $(OBJ)/file.o $(OBJ)/dynamic.o
	$(CC) -o $(BIN)/$(DYNAMIC) $^ $(CFLAGS)

$(BIN)/$(GREEDY): $(OBJ)/file.o $(OBJ)/greedy.o
	$(CC) -o $(BIN)/$(GREEDY) $^ $(CFLAGS)

$(OBJ)/%.o:	$(SRC)/%.c
	$(CC) -c $^ -o $@ $(CFLAGS)

clean:
	rm -vf ${TEX_PATH}/{*.dvi,*.aux,*.log,*.pdf}
	rm -vf $(OBJ)/*.o
	rm -vf $(BIN)/tp*
	rm -vf $(TEX_PATH)/gnuplot/{*.eps,*.pdf,*.ps,*.tex}

latex:	${TEX_PATH}/*.tex plot
	pdflatex -output-directory=$(TEX_PATH) $(TEX_PATH)/$(DOC_NAME).tex
	@evince $(TEX_PATH)/$(DOC_NAME).pdf

plot: ${TEX_PATH}/gnuplot/*.plt
	gnuplot $^
