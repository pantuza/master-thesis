#
# My Master's thesis Makefile 
#

TEX := colorpdflatex
BIB := bibtex

FLAGS := -halt-on-error -file-line-error

SRC := src
BIN := bin

DOCUMENT := presentation

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

deps: DEPENDENCIES colorlatex.sh
	sudo zypper install $(shell cat $<)
	bash $(word 2, $^)

clean:
	@rm -rvf *.aux *.pdf *.dvi *.log *.bbl *.blg *.lof *.lot *.out \
		*.toc *.nav *.snm $(BIN)/*.pdf
