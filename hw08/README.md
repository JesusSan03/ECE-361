# HW8 - Binary Search Tree
For this homework, I changed the BinarySearchTree.c example into a tree that other C programs can use. Smaller numbers go left, and equal or bigger numbers go right. The tree does not use a global root anymore.

## How to run
In the hw08 folder, type:

```
make
make test
make memcheck
make bonus
```

## What I made
- `bst.h` and `bst.c` have the tree functions.
- `treestats.c` reads numbers and prints the tree size and height.
- `tests/test_bst.c` tests the tree and the old crash.
- `tests/test_remove.c` tests the bonus.
- `BONUS.md` explains how removing a number works.

## The crash
I tested searching an empty tree, and the original search code crashed. This was the first line of the memory checker report:

```
bst.c:105:19: runtime error: member access within null pointer of type 'const struct node'
```

The old program never noticed the problem because it added seven numbers before searching. The search tried to use `current->data` when `current` was NULL, so I fixed it by checking for NULL first.

## Tests
The original seven-number tree has size 7 and height 3. Searching for 31 takes 3 visits, and searching for 15 takes 3 visits but does not find it. I also tested duplicates, an empty tree, and numbers inserted in order. All tests passed.

## Insertion order
I ran:

```
seq 1 1023 | ./treestats
seq 1 1023 | sort -R | ./treestats
```

Sorted output:

```
n 1023, height 1023
```

Shuffled output:

```
n 1023, height 24
```

Putting numbers in order made the tree very tall, while mixing them made a much shorter tree in this run. A taller tree can take more visits to search because there are more nodes along the path.

The shuffled height might be different if I run it again.

## Bonus
I added `bst_remove()` to remove one number. The bonus tests passed, and the tree drawings are in BONUS.md.