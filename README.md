# StreamSort

A program that lexicographically sorts an arbitrary amount of lines of arbitrary length from std input.

Internally the lines are stored in blocks of memory.\
Multiple lines may be stored in a single block of memory, and when a block of memory runs out, we allocate another larger block of memory.

This improves the number of memory allocations over treating each line like a new vector of characters, 
  as each line will not need to dynamically grow independently from all others.
  Lines would normally need multiple reallocations, 
  but if we have many lines the block size will grow to the point that each line can fit in one block, 
  and eventually multiple lines will be able to fit in a single block, 
  so in the long run there is less than one allocation per line.
