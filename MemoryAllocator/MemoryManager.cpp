#include "pch.h"
#include "MemoryManager.h"
#include <type_traits>
#include <new>

MemoryManager::MemoryManager()
{
	allocator = nullptr;
	pointerStorage = nullptr;
	rcStorage = nullptr;
}

MemoryManager::MemoryManager(Allocator * allocator, int pointerLimit)
{
	this->allocator = allocator;
	this->pointerStorage = new Pool(sizeof(SmartPointer<int>), pointerLimit);
	this->rcStorage = new Pool(sizeof(int), pointerLimit);
}

MemoryManager & MemoryManager::getInstance()
{
	static MemoryManager m;
	return m;
}

//method to set allocator and pool/rc siz


//MemoryManager & MemoryManager::initStack(size_t size, int pointerLimit)
//{
//	static MemoryManager m(new Stack(size), pointerLimit);
//	return m;
//}
//
//MemoryManager & MemoryManager::initDoubleStack(size_t size, int pointerLimit)
//{
//	static MemoryManager m(new DoubleEndedStack(size), pointerLimit);
//	return m;
//}
//
//MemoryManager & MemoryManager::initPool(size_t size, int num_elements, int pointerLimit)
//{
//	static MemoryManager m(new Pool(size, num_elements), pointerLimit);
//	return m;
//}

void MemoryManager::init(Allocator * allocator, int pointerLimit)
{
	if (pointerLimit <= 0) {
		throw "invalid pointer limit value, must be greater than 0";
	}
	else if (allocator == nullptr) {
		throw "invalide allocator provided";
	}
	MemoryManager::getInstance().allocator = allocator;
	MemoryManager::getInstance().pointerStorage = new Pool(sizeof(SmartPointer<int>), pointerLimit);
	MemoryManager::getInstance().rcStorage = new Pool(sizeof(SmartPointer<int>), pointerLimit);
}

void * MemoryManager::allocate(size_t size, AllocOptions options)
{
	if (allocator == nullptr) {
		throw "memory manager is not initialised";
	}
	void * address = this->allocator->allocate(size, options);
	return address;
}

void MemoryManager::deallocate(Marker index, size_t size, AllocOptions options)
{
	if (allocator == nullptr) {
		throw "memory manager is not initialised";
	}
	this->allocator->deallocate(index, size, options);
}

void MemoryManager::freeSmartPtr(Marker toFree)
{
	if (pointerStorage == nullptr) {
		throw "memory manager is not initialised";
	}
	pointerStorage->deallocate(toFree);
}

void MemoryManager::freeRC(Marker toFree)
{
	if (rcStorage == nullptr) {
		throw "memory manager is not initialised";
	}
	rcStorage->deallocate(toFree);
}

MemoryManager::~MemoryManager()
{
	delete allocator;
}
