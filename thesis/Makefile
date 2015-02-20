#
# My Master's thesis Makefile 
#

TEX := pdflatex
BIB := bibtex

FLAGS := -halt-on-error -file-line-error

SRC := src
BIN := bin

DOCUMENT := thesis

all: thesis

thesis: $(SRC)/$(DOCUMENT).tex
	$(TEX) $^ $(FLAGS)
	$(BIB) $(DOCUMENT).aux
	$(BIB) $(DOCUMENT).aux
	$(TEX) $^ $(FLAGS)
	$(TEX) $^ $(FLAGS)
	[ -f "$(DOCUMENT).pdf" ] && mv $(DOCUMENT).pdf $(BIN)/$(DOCUMENT).pdf

read: $(BIN)/$(DOCUMENT).pdf
	evince $^

latex: thesis read

deps: DEPENDENCIES
	sudo zypper install $(shell cat $^)

clean:
	@rm -rvf *.aux *.pdf *.dvi *.log *.bbl *.blg *.lof *.lot *.out \
		*.toc $(BIN)/*
