CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lreadline

src = $(wildcard *.c)

.PHONY: all clean

all: watcher

watcher: 
	$(CC) $(src) $(LDFLAGS) -o watcher

clean:
	$(RM) -f watcher log.txt