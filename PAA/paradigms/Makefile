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
CFLAGS := -I$(LIB) -g
# Compiler
CC := gcc

# BINARY OUTPUT NAMES
BRUTE := tp1b
DYNAMIC := tp1pd
GREEDY := tp1g


all: brute dynamic greedy

brute:	$(OBJ)/brute.o $(OBJ)/file.o
	$(CC) -o $(BIN)/$(BRUTE) $^ $(CFLAGS)

dynamic:	$(OBJ)/dynamic.o $(OBJ)/file.o
	$(CC) -o $(BIN)/$(DYNAMIC) $^ $(CFLAGS)

greedy:	$(OBJ)/greedy.o $(OBJ)/file.o
	$(CC) -o $(BIN)/$(GREEDY) $^ $(CFLAGS)

$(OBJ)/%.o:	$(SRC)/%.c
	$(CC) -c $^ -o $@ $(CFLAGS)

clean:
	rm -vf ${TEX_PATH}/{*.dvi,*.aux,*.log,*.pdf}
	rm -vf $(OBJ)/*.o
	rm -vf $(BIN)/{$(BRUTE),$(DYNAMIC),$(GREEDY)}

latex:	${TEX_PATH}/*.tex
	latex -output-directory ${TEX_PATH} ${TEX_PATH}/${DOC_NAME}.tex
	dvipdf ${TEX_PATH}/${DOC_NAME}.dvi ${TEX_PATH}/${DOC_NAME}.pdf
