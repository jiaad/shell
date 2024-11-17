run:
	gcc ./src/shell.c && ./a.out
asm:
	gcc -s ./src/shell.c
obj:
	gcc -Og -c ./src/shell.c
