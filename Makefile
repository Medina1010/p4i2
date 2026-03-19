all: pdf
data:
	make -C analysis
pdf: data
	tectonic main.tex
