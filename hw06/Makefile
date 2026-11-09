CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
LDLIBS = -lm
MODULES = sensor.o panel.o scenario.o iom361_r4.o
all: replay
replay: main.o $(MODULES)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)
test_hw06: tests/test_hw06.o $(MODULES)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)
main.o: main.c sensor.h panel.h scenario.h iom361_r4.h
sensor.o: sensor.c sensor.h iom361_r4.h
panel.o: panel.c panel.h iom361_r4.h
scenario.o: scenario.c scenario.h
iom361_r4.o: iom361_r4.c iom361_r4.h
tests/test_hw06.o: tests/test_hw06.c sensor.h panel.h scenario.h iom361_r4.h
	$(CC) $(CFLAGS) -I. -c $< -o $@
test: replay test_hw06
	./test_hw06
	sh ./regress.sh
clean:
	rm -f replay test_hw06 main.o $(MODULES) tests/test_hw06.o
.PHONY: all test clean
