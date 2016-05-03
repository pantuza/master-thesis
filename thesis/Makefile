#
# My Master's thesis Makefile 
#

TEX := colorpdflatex
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

deps: DEPENDENCIES colorlatex.sh
	sudo zypper install $(shell cat $<)
	bash $(word 2, $^)

push:
	git checkout thesis
	git pull origin thesis
	git push origin thesis

merge: push
	git checkout master
	git pull --no-edit origin master
	git merge thesis --no-edit
	git push origin master
	git checkout thesis

clean:
	@rm -rvf *.aux *.pdf *.dvi *.log *.bbl *.blg *.lof *.lot *.out \
		*.toc $(BIN)/*.pdf
