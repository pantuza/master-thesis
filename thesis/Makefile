#
# ManSDN Graph paper Makefile 
#

TEX := pdflatex
BIB := bibtex

SRC := src
BIN := bin

DOCUMENT := thesis

all: thesis

thesis: $(SRC)/$(DOCUMENT).tex
	$(TEX) $^
	$(BIB) $(SRC)/$(DOCUMENT).aux
	$(BIB) $(SRC)/$(DOCUMENT).aux
	$(TEX) $^
	$(TEX) $^
	[ -f "$(DOCUMENT).pdf" ] && mv $(DOCUMENT).pdf $(BIN)/$(DOCUMENT).pdf

read: $(BIN)/$(DOCUMENT).pdf
	evince $^

latex: thesis read

deps: DEPENDENCIES
	sudo zypper install $(shell cat $^)

clean:
	@rm -rvf *.aux *.pdf *.dvi *.log *.bbl *.blg *.lof *.lot *.out *.toc
