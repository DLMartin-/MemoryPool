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