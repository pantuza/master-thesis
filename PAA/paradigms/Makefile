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


all:
	@echo "What's up, doc!"

clean:
	rm -vf ${TEX_PATH}/{*.dvi,*.aux,*.log,*.pdf}

latex:	${TEX_PATH}/*.tex
	latex -output-directory ${TEX_PATH} ${TEX_PATH}/${DOC_NAME}.tex
	dvipdf ${TEX_PATH}/${DOC_NAME}.dvi ${TEX_PATH}/${DOC_NAME}.pdf
