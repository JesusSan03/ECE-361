# Homework 5: Combination Lock

This homework uses a finite state machine to make a combination lock. The combination is 3-1-4. Every time the correct number is entered, the lock moves to the next state. If the wrong number is entered, the lock beeps and starts over.

## How to Build and Test

First, open the hw05 folder in VS Code and open the terminal.

Use these commands:

```bash
make
./lock < data/codes.txt
make test
make bonus
make clean
```

- `make` builds the program.
- `./lock < data/codes.txt` runs the program using the example inputs.
- `make test` runs the tests.
- `make bonus` runs the bonus tests.
- `make clean` removes the files created when building the program.

The program reads one character at a time. Numbers are entered into the keypad, c means the door is closing, and t means the timer ran out. Any other characters are ignored.

The program does not use a real five-second timer. Instead, entering t represents the timer running out.

## The Four States

There are four states in the combination lock.

- LOCKED: The door is locked and waiting for the number 3.
- GOT1: The first number was correct and it is waiting for 1.
- GOT2: The first two numbers were correct and it is waiting for 4.
- OPEN: The correct combination was entered and the door is unlocked.

## Transition Table

This table shows what happens depending on the current state and the event. Each entry shows the next state and the action.

| State | RIGHT | WRONG | CLOSE | TIMEOUT |
|---|---|---|---|---|
| LOCKED | GOT1 / NONE | LOCKED / BEEP | LOCKED / NONE | LOCKED / NONE |
| GOT1 | GOT2 / NONE | LOCKED / BEEP | GOT1 / NONE | LOCKED / NONE |
| GOT2 | OPEN / UNLOCK | LOCKED / BEEP | GOT2 / NONE | LOCKED / NONE |
| OPEN | OPEN / NONE | OPEN / NONE | LOCKED / LOCK | LOCKED / LOCK |

When the correct number is entered, the lock moves forward. If the wrong number is entered, it beeps and goes back to LOCKED. If the timer runs out while entering the code, it starts over without beeping.

When the door is open, entering more numbers does nothing. Closing the door or having the timer run out locks it again.

## State Diagram

```text
LOCKED
   |
   | RIGHT (3)
   v
 GOT1
   |
   | RIGHT (1)
   v
 GOT2
   |
   | RIGHT (4) / UNLOCK
   v
 OPEN
   |
   | CLOSE or TIMEOUT / LOCK
   v
LOCKED
```

Other possible transitions:

- LOCKED: Wrong numbers cause a beep. Closing or timeout does nothing.
- GOT1: Wrong numbers or timeout go back to LOCKED.
- GOT2: Wrong numbers or timeout go back to LOCKED.
- GOT1 and GOT2: Closing the door does nothing.
- OPEN: Numbers are ignored. Closing the door or timeout goes back to LOCKED.

## Tests

The tests are in `tests/test_lock.c`.

They check all 16 entries in the transition table. They also check correct numbers, wrong numbers, closing the door, timer events, and other characters.

Some example inputs are:

- 314: Opens the lock.
- 3314: Does not open because the second 3 is wrong.
- 31t4: Does not open because the timer resets the code.
- 314c: Opens the lock and then closes it.

To run the tests, type:

```bash
make test
```

Each test prints PASS or FAIL, and the program shows the final results.

## Bonus Checker

The bonus files are `check.c` and `check.h`.

The checker looks for problems in the transition table, like missing entries, invalid states, and states that cannot be reached.

The bonus tests use broken versions of the table to see if the checker can find the problems.

More information about the bonus is in `BONUS.md`.