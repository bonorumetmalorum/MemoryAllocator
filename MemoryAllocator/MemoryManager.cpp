#include "pch.h"
#include "MemoryManager.h"
#include <type_traits>

template<class Allocator>
MemoryManager::MemoryManager(size_t size)
{
	allocator = new Allocator(size);
}

MemoryManager<Pool>::MemoryManager(size_t size, int num_elements) 
{
	allocator = new Pool(size, num_elements);
}

void * MemoryManager<DoubleEndedStack>::insertTop()
{
	return nullptr;
}

void * MemoryManager<DoubleEndedStack>::insertBottom()
{
	return nullptr;
}

void MemoryManager<DoubleEndedStack>::deallocateTop()
{
}

void MemoryManager<DoubleEndedStack>::deallocateBottom()
{
}

template<class Allocator>
void * MemoryManager<Allocator>::insert()
{
	return nullptr;
}

template<class Allocator>
void MemoryManager<Allocator>::remove()
{
}

template<class Allocator>
void * MemoryManager<Allocator>::access(size_t)
{
	return nullptr;
}

template<class Allocator>
MemoryManager::~MemoryManager()
{
}
