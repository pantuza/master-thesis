#
# Makefile for compiling LaTeX document.
#
# @author: Gustavo Pantuza
# @since: 05.09.2013
#

docname := self-stabilized-start-stop

latex: clean *.tex
	pdflatex $(docname).tex
	bibtex $(docname) 
	pdflatex $(docname).tex
	pdflatex $(docname).tex
	@evince $(docname).pdf

clean:
	@rm -vf *.aux *.pdf *.log *.bbl *.blg
