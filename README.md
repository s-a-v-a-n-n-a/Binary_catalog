# Binary_catalog
**DOCUMENTATION**
=================
This is binary catalog imitating Akinator game.

**DESCRIPTION**
---------------
Work of binary catalog is based on unbalanced binary tree. It contains questions in tree nodes and objects to guess in leaves. While working program orientates toward users answers. For example, if users chooses to answer "NO" it goes to the left son of the node. 
The tree itself is stored in an array working like [My-List](https://github.com/s-a-v-a-n-n-a/My-List). Every node has access to its left and right sons and parent. That is why no recusion is needed to work with tree.
To contain strings of text the tree has special buffer. That helps to avoid extra doings with pointers on char arrays.

**PROCESS OF WORK**
-------------------
Before the work the program tells user what commands are available for her. For easier work we will also represent them here:
[h] : to remember commands
[g] : to start the game 
[d] <your input> : to input a new object. Tree will write it in special file to use after end of work
[c] <first input> <second input> : to make program compare 2 objects
[p] : to see the picture representation of the tree
[e] : to end game
  
**SUBTLETIES OF USE**
---------------------
It is considered to launch this program on Windows. 
To make sound accompaniment available it is needed to download my lecturer's program [TXLib](https://github.com/ded32/TXLib/tree/0739ed7c54cb6e0d4c8302f08a0936986cfb57b2) and pet the cat.
  
**NOTES**
---------
This program is educational. I wrote it in the first semester to learn binary trees applications.

