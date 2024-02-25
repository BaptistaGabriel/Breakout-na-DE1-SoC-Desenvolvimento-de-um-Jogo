all:

	clear
	gcc -Wall main.c -o main -lintelfpgaup -lm -std=c99
	sudo ./main