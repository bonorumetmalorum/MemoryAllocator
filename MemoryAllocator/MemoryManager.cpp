#include "pch.h"
#include "MemoryManager.h"
#include <type_traits>
#include <new>

MemoryManager::MemoryManager()
{
	allocator = nullptr;
	pointerStorage = nullptr;
	rcStorage = nullptr;
	isInit = false;
}

MemoryManager::MemoryManager(Allocator * allocator, int pointerLimit)
{
	this->allocator = allocator;
	this->pointerStorage = new Pool(sizeof(SmartPointer<int>), pointerLimit);
	this->rcStorage = new Pool(sizeof(int), pointerLimit);
}

/*
	get the current instance of the memory manager
	@return MemoryManager & instance of the memory manager, may be unintialised
*/
MemoryManager & MemoryManager::getInstance()
{
	static MemoryManager m;
	return m;
}

/*
	initialise the memory manager
	@param allocator the allocator to use
	@param pointerLimit the upper limit on live smart pointers
	@throw if the memory manager is already initialised
	@throw if the allocator is not correctly constructed
	@throw if the pointerLimit <= 0
*/
void MemoryManager::init(Allocator * allocator, int pointerLimit)
{
	if (isInit) {
		throw "already initialised";
	}
	if (pointerLimit <= 0) {
		throw "invalid pointer limit value, must be greater than 0";
	}
	else if (allocator == nullptr) {
		throw "invalide allocator provided";
	}
	MemoryManager::getInstance().allocator = allocator;
	MemoryManager::getInstance().pointerStorage = new Pool(sizeof(SmartPointer<int>), pointerLimit);
	MemoryManager::getInstance().rcStorage = new Pool(sizeof(SmartPointer<int>), pointerLimit);
	isInit = true;
}

/*
	Allocate the given size of memory with the given allocation options
	@param size the size of the block to be allocated
	@param options the specific allocation options
*/
void * MemoryManager::allocate(size_t size, AllocOptions options)
{
	if (allocator == nullptr) {
		throw "memory manager is not initialised";
	}
	void * address = this->allocator->allocate(size, options);
	return address;
}

/*
	Deallocate the given Marker
	@param index the marker to deallocate
	@param size the amount of space the marker is using
	@param options the specific deallocation options
*/
void MemoryManager::deallocate(Marker index, size_t size, AllocOptions options)
{
	if (allocator == nullptr) {
		throw "memory manager is not initialised";
	}
	this->allocator->deallocate(index, size, options);
}

/*
	clear the memory, will not reset pointers and their reference counts
*/
void MemoryManager::clear()
{
	allocator->clear();
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
