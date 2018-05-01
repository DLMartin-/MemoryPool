//
// Created by Danny on 4/28/2018.
//

#include "MemoryPool.h"
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <cassert>
#include <iostream>

static const std::uint32_t cMinimumAlignment = sizeof(unsigned char*);

//Round up the size of the struct passed in to be a multiple of the desired alignment.
static const std::uint32_t CalculateStructSize(const std::uint32_t size, const std::uint32_t alignment)
{
  return ((size + alignment - 1) / alignment) * alignment;
}

MemoryPool::MemoryPool(const std::uint32_t blockSize,
                       const std::uint32_t blockAlignment,
                       const std::uint32_t numberOfBlocks) : m_memory(nullptr),
                                                             m_currentMemoryBlock(nullptr),
                                                             m_memorySize(0)
{
  assert(numberOfBlocks > 0);
  const std::size_t actualStructAlignment = (blockAlignment > cMinimumAlignment) ? blockAlignment : cMinimumAlignment;
  const std::size_t actualStructSize = CalculateStructSize(blockSize, actualStructAlignment);

  m_memorySize = actualStructSize * numberOfBlocks + actualStructAlignment;
  m_memory = std::malloc(m_memorySize);
  m_currentMemoryBlock = static_cast<unsigned char*>(std::align(actualStructAlignment,
                                                                actualStructSize,
                                                                m_memory,
                                                                m_memorySize));

  //Loop thru our malloc'd memory and initialize each block to point to the next valid block
  void** ptr = nullptr;
  for (std::size_t i = 0; i <= numberOfBlocks - 2; ++i)
  {
    ptr = reinterpret_cast<void**>(m_currentMemoryBlock + i * actualStructSize);
    *ptr = (m_currentMemoryBlock + ((i + 1) * actualStructSize));
  }
  ptr = reinterpret_cast<void**>(m_currentMemoryBlock + (numberOfBlocks - 1) * actualStructSize);
  *ptr = nullptr;
}

MemoryPool::~MemoryPool()
{
    std::free(m_memory);
}

void* MemoryPool::Alloc()
{
  if (m_currentMemoryBlock == nullptr)
  {
    return nullptr;
  }
  auto ptr = reinterpret_cast<void*>(m_currentMemoryBlock);
  auto nextPtr = reinterpret_cast<void**>(m_currentMemoryBlock);
  m_currentMemoryBlock = reinterpret_cast<unsigned char *>(*nextPtr);
  return ptr;
}

void MemoryPool::Free(void* data)
{
  if (data >= m_memory && data < reinterpret_cast<unsigned char*>(m_memory) + m_memorySize)
  {
    *reinterpret_cast<void**>(data) = m_currentMemoryBlock;
    m_currentMemoryBlock = reinterpret_cast<unsigned char*>(data);
  }
}


