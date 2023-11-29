all:hw4

run:main.c
	make hw4
	./hw4

hw4:main.c
	gcc main.c -o  hw4 -lm

doc: main.tex
	pdflatex -shell-escape main.tex -o main.pdf

clean:
	rm *.pdf
	rm *.dot
	rm hw4
