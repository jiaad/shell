compile:
	gcc -g -Wall -Werror -Wextra -fanalyzer -D_POSIX_C_SOURCE=200809L -std=c11 -pedantic-errors src/readline.c src/shell.c lib/src/DA.c src/lexer.c  src/token.c lib/src/error.c  lib/src/fork.c src/commands.c src/builtins/cd.c lib/src/signal.c  src/parser.c src/piping.c -o bin/readline
run:
	./bin/readline

asm:
	gcc -s ./src/shell.c
obj:
	gcc -Og -c ./src/shell.c
