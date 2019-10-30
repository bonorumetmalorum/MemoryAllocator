#include "pch.h"
#include "MemoryManager.h"
#include <type_traits>

MemoryManager::MemoryManager()
{
	this->allocator = new Stack(100);
}

MemoryManager::MemoryManager(Allocator * allocator, int pointerLimit)
{
	this->allocator = allocator;
}

MemoryManager & MemoryManager::initStack(size_t size, int pointerLimit)
{
	static MemoryManager m(new Stack(size), pointerLimit);
	return m;
}

MemoryManager & MemoryManager::initDoubleStack(size_t size, int pointerLimit)
{
	static MemoryManager m(new DoubleEndedStack(size), pointerLimit);
	return m;
}

MemoryManager & MemoryManager::initPool(size_t size, int num_elements, int pointerLimit)
{
	static MemoryManager m(new Pool(size, num_elements), pointerLimit);
	return m;
}

void * MemoryManager::allocate(size_t size, AllocOptions options)
{
	return this->allocator->allocate(size, options);
}

void MemoryManager::deallocate(Marker index, AllocOptions options)
{
	this->allocator->deallocate(index, options);
}
