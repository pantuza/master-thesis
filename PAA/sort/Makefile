#
# Makefile for the exercices written in C language
#

SRC := src
BIN := bin

# list C files and get its name without extension
OBJECTS = $(basename $(wildcard ${SRC}/*.c))

# default rule to compile C files
.c:
	cc -g -o ${BIN}/$(basename $(notdir $<)) $< 

all: ${OBJECTS}
	
clean: 
	@rm -fv ${BIN}/*


