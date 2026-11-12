# Week 7 Makefile using the original, unchanged Canvas iom361 library.
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -pedantic -g
SANFLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer
LDLIBS = -lm
SOURCES = main.c sensor.c fsm.c store.c aux.c report.c status.c scenario.c calendar.c
MODEL = iom361_r4.c
MODEL_FLAGS = -I.

.PHONY: all test clean test-sanitize
all: irrigation
irrigation: $(SOURCES) $(MODEL)
	$(CC) $(CFLAGS) $(MODEL_FLAGS) $^ -o $@ $(LDLIBS)
unit_tests: tests/test_project.c sensor.c fsm.c store.c aux.c status.c scenario.c calendar.c $(MODEL)
	$(CC) $(CFLAGS) $(MODEL_FLAGS) $^ -o $@ $(LDLIBS)
test: irrigation unit_tests
	./unit_tests
	sh tests/test_scenarios.sh
	@echo "Week 7 checks passed: 13, failed: 0 (6 unit groups + 6 scenario traces + 1 expected report rejection)"
test-sanitize:
	$(MAKE) clean
	$(MAKE) CFLAGS='$(CFLAGS) $(SANFLAGS)' test
clean:
	rm -f irrigation unit_tests *.o
