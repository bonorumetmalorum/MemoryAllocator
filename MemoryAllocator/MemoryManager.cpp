#include "pch.h"
#include "MemoryManager.h"
#include <type_traits>
#include <new>

MemoryManager::MemoryManager()
{
	this->allocator = new Stack(100);
}

MemoryManager::MemoryManager(Allocator * allocator, int pointerLimit)
{
	this->allocator = allocator;
	this->pointerStorage = new Pool(sizeof(SmartPointer<int>), pointerLimit);
	this->rcStorage = new Pool(sizeof(int), pointerLimit);
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
	void * address = this->allocator->allocate(size, options);
	return address;
}

void MemoryManager::deallocate(Marker index, AllocOptions options)
{
	this->allocator->deallocate(index, options);
}

MemoryManager::~MemoryManager()
{
	delete allocator;
}
