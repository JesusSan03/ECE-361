
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra

OBJS = main.o sensor.o fsm.o store.o aux.o report.o iom361_r4.o

.PHONY: all test clean

all: irrigation

irrigation: $(OBJS)
	$(CC) $(CFLAGS) -o irrigation $(OBJS)

main.o: main.c iom361_r4.h sensor.h fsm.h store.h aux.h report.h
	$(CC) $(CFLAGS) -c main.c -o main.o

sensor.o: sensor.c sensor.h
	$(CC) $(CFLAGS) -c sensor.c -o sensor.o

fsm.o: fsm.c fsm.h
	$(CC) $(CFLAGS) -c fsm.c -o fsm.o

store.o: store.c store.h
	$(CC) $(CFLAGS) -c store.c -o store.o

aux.o: aux.c aux.h
	$(CC) $(CFLAGS) -c aux.c -o aux.o

report.o: report.c report.h
	$(CC) $(CFLAGS) -c report.c -o report.o

iom361_r4.o: iom361_r4.c iom361_r4.h
	$(CC) $(CFLAGS) -c iom361_r4.c -o iom361_r4.o

test: irrigation
	./irrigation

clean:
	rm -f irrigation $(OBJS)
