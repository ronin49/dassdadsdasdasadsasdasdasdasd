PREFIX?=/usr/X11R6
CFLAGS?=-Os -pedantic -Wall

all:
	$(CC) $(CFLAGS) -I$(PREFIX)/include -g lol.c tinywm.c -L$(PREFIX)/lib -lX11 -o wm

clean:
	rm -f tinywm

