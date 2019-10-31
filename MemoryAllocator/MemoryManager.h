#pragma once
#include "Stack.h"
#include "DoubleEndedStack.h"
#include "Pool.h"
#include "SmartPointer.h"

/*

WHAT NOW?

need to add a clear method

*/

class MemoryManager {
public:
	

	static MemoryManager & initStack(size_t size, int pointerLimit);
	static MemoryManager & initDoubleStack(size_t size, int pointerLimit);
	static MemoryManager & initPool(size_t size, int num_elements, int pointerLimit);

	void * allocate(size_t size, AllocOptions = DEFAULT); 
	void deallocate(Marker index, AllocOptions = DEFAULT);

	template<template<class> class SmartPointer, typename T>
	SmartPointer<T> smartAllocate(size_t, AllocOptions = DEFAULT);

	~MemoryManager();


private:
	MemoryManager();
	MemoryManager(Allocator * allocator, int pointerLimit);
	Allocator * allocator;
	Pool * pointerStorage;
	Pool * rcStorage;
};

template<template<class> class SmartPointer, typename T>
SmartPointer<T> MemoryManager::smartAllocate(size_t size, AllocOptions options)
{
	T * address = (T*)this->allocate(size, options);
	void* ptr = this->pointerStorage->allocate(size);
	int* rc = (int*)this->rcStorage->allocate(size);
	SmartPointer<T> * pointer = new(ptr) SmartPointer<T>(address, rc);
	return *pointer;
}
