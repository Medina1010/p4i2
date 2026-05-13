all: pdf
data:
	make -C analysis
pdf: data
	tectonic f4i2.tex -o ../vault/
