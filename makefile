all:hw3

run:main.c
	make hw3
	./hw3

hw3:main.c
	gcc main.c -o  hw3 -lm

doc: main.tex
	pdflatex -shell-escape main.tex -o main.pdf

clean:
	rm hw3