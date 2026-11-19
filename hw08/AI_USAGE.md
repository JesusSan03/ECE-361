# AI Usage - HW8
I used ChatGPT for help with this assignment.

1. The original program used a global root, and I needed help changing it so each tree has its own root inside bst_t.

2. The old search function crashed when the tree was empty. We tested that problem first and found that it was trying to read current->data when current was NULL.

3. I needed to make sure the visit count included the last node checked, even when the number was not found. ChatGPT helped test this with 31 and 15 in the seven-number tree.

4. I got help checking that duplicate numbers still go to the right and increase the size of the tree.
