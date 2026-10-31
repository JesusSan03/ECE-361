# Bonus: Table Checker
For the bonus, I made a table checker using `check.c` and `check.h`. The tests are in `tests/test_check.c`.
The checker looks through the transition table to see if there are any problems.
To run the bonus tests, type:

```bash
make bonus
```

To run all the tests, type:

```bash
make test
```

## Problems the Checker Looks For
The checker looks for four different problems.

1. Missing Entry: This happens when a spot in the table does not have an action assigned to it.

2. Invalid State: This happens when the table tries to move to a state that does not exist.

3. Unreachable State: This happens when the program cannot reach a state starting from LOCKED.

4. No Way Out: This happens when a state cannot move to a different state, no matter what event happens.

## How the Checker Works
The checker goes through the table and looks for these problems.
It also uses something called breadth-first search (BFS). This is a way to search through the states and see which ones can be reached from the starting state.
The starting state is LOCKED, and the checker follows the transitions to see where the program can go.

## Why These Problems Matter
These problems can cause the combination lock to stop working correctly.
For example, if the OPEN state has no way out, the door might stay unlocked even when someone closes it.
If GOT2 points to an invalid state, the program could try to access something that does not exist.
Another example is if GOT2 cannot be reached. This would mean the correct combination could never finish, so the lock would not open.
Missing entries can also cause problems because the program would not know what action to take.

## Three Broken Table Examples

**Example 1: Missing Action**

I changed the CLOSE event in GOT1 so its action was A_UNDEFINED.

Output:
```
undefined entry: GOT1 / CLOSE
```

This is a problem because every part of the table should have an action. Leaving one undefined means the lock might not behave correctly.

**Example 2: Invalid State**

I changed GOT2 so the RIGHT event goes to N_STATES, which is not a real state.

Output:
```
out-of-range next state: GOT2 / RIGHT -> 4
unreachable state: OPEN
```

This is a problem because the program could try to use a state that does not exist. It also means the lock cannot reach OPEN using the correct combination.

**Example 3: State Cannot Be Reached**

I changed GOT1 so the RIGHT event goes back to GOT1 instead of GOT2.

Output:
```
unreachable state: GOT2
unreachable state: OPEN
```

This is a problem because the program would never reach the last digit of the combination. Even when someone enters the correct code, the lock would not open.

## Conclusion
The table checker is useful because it can find mistakes that might be hard to notice just by looking at the code.
However, it cannot find every possible problem. For example, the table could have the wrong combination and still pass the checker.
This is why it is important to run the regular tests along with the bonus tests.