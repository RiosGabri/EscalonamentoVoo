CC = gcc
CFLAGS = -Wall

scheduler: main.c
	$(CC) $(CFLAGS) main.c -o scheduler

clean:
	rm -f scheduler *.out