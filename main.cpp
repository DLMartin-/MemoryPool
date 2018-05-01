#include <iostream>
#include "MemoryPool.h"


int main()
{
  MemoryPool mp(32, 32, 5);

  auto t1 = mp.Alloc();
  auto t2 = mp.Alloc();
  auto t3 = mp.Alloc();
  mp.Free(t1);
  auto t4 = mp.Alloc();
  auto t5 = mp.Alloc();
  auto t6 = mp.Alloc();
  auto t7 = mp.Alloc();
  auto t8 = mp.Alloc();
  mp.Free(t8);
  auto t9 = mp.Alloc();
  return 0;
}