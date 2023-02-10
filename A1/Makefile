.PHONY: all clean

CC=gcc
CFLAGS=

all: minishell

minishell: minishell.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf *.o minishell
