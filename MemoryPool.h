//
// Created by Danny on 4/28/2018.
//

#ifndef MEMORYPOOL_MEMORYPOOL_H
#define MEMORYPOOL_MEMORYPOOL_H

#include <cstdint>

class MemoryPool
{
public:
  MemoryPool(const std::uint32_t blockSize, const std::uint32_t blockAlignment, const std::uint32_t numberOfBlocks);
  ~MemoryPool();

  void *Alloc();
  void Free(void *data);
private:
  std::size_t m_memorySize;
  void *m_memory;
  unsigned char *m_currentMemoryBlock;
};


#endif //MEMORYPOOL_MEMORYPOOL_H
