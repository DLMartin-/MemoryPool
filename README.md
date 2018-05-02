# MemoryPool
A simple, generic memory pool that works without templates, based off of the implementation given in `Game Coding Complete: 4th Edition`. 

While the original implementation included the ability to grow the pool while also not invalidating any pointers, 
it returned memory at invalid locations. While this is (probably) fine on most modern CPUs, it will have a performance hit
and, if you're unlucky enough, crash your program. 

This attempts to solve that issue, at the potential cost of a few wasted bytes.
  
* Note: It does not yet offer the ability to grow the memory pool, but that's a simple thing to add if you're feeling adventurous :D

---
The way the pool works is simple:

1. First, we take the desired size of a single block, and the desired alignment.
2. Then, we check to see if the desired size is less than the size of a char*. 
   * If it is, each block size is treated as the size of a char* and has the alignment of a char*.
3. We then make sure that your desired size is a multiple of of your desired alignment, in order to properly align each block.
   * If a block is not in use, it will simply be treated as containing the pointer to the next unused block (or nullptr if no blocks remain).

---
Using the memory pool is easy: Just pass it your desired size, alignment, and the number of blocks you want it to store!
To allocate a block, just make a call to Alloc(). 
To free a block when it is no longer needed, just pass it on over to Free(). 

Please see the example below:

```c++
#include <iostream>
#include "MemoryPool.h"

typedef struct
{
  int i;
  char* c;
  double d;
} S;

int main()
{
  MemoryPool mp(sizeof(S), alignof(S), 5);

  S* s1 = new(mp.Alloc()) S;
  s1->i = 54;
  s1->d = 511;
  s1->c = static_cast<char*>("Hello World!");

  S* s2 = new(mp.Alloc()) S{10, static_cast<char*>("Jingle Bells"), 101010.0};

  std::cout << "S1->i: " << s1->i << std::endl;
  std::cout << "S1->c: " << s1->c << std::endl;
  std::cout << "S1->d: " << s1->d << std::endl;

  std::cout << "-----------------" << std::endl;

  std::cout << "S2->i: " << s2->i << std::endl;
  std::cout << "S2->c: " << s2->c << std::endl;
  std::cout << "S2->d: " << s2->d << std::endl;

  std::cout << "---------------" << std::endl;
  s1->~S();
  mp.Free(s1);

  //Request another block from mp, but do not set it's data members.
  //s3 and s1 will point to the same memory block.
  S* s3 = new(mp.Alloc()) S;

  //Expected output: Garbage data mixed with actual data
  std::cout << "S3->i: " << s3->i << std::endl;
  std::cout << "S3->c: " << s3->c << std::endl;
  std::cout << "S3->d: " << s3->d << std::endl;

  std::cout << "---------------" << std::endl;

  //Expected output: The same as s3
  std::cout << "S1->i: " << s3->i << std::endl;
  std::cout << "S1->c: " << s3->c << std::endl;
  std::cout << "S1->d: " << s3->d << std::endl;

  std::cout << "----------------" << std::endl;

  //Ensure the memory in S2 is untouched.
  std::cout << "S2->i: " << s2->i << std::endl;
  std::cout << "S2->c: " << s2->c << std::endl;
  std::cout << "S2->d: " << s2->d << std::endl;
  
  return 0;
}
```
