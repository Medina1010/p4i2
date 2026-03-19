all: data1.tex data2.tex
data1.tex: data1.tsv main
	./main data1.tsv data1.tex
data2.tex: data2.tsv main
	./main data2.tsv data2.tex
main: main.c
	cc main.c -o main
