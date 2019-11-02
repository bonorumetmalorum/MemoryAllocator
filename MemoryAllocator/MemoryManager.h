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
	
	template<class T> friend class SmartPointer;

public:
	
	static MemoryManager & getInstance();
	void init(Allocator * allocator, int pointerLimit);
	void * allocate(size_t size, AllocOptions = DEFAULT); 
	void deallocate(Marker index, size_t size = 0, AllocOptions = DEFAULT);

	template<template<class> class SmartPointer, typename T>
	SmartPointer<T> smartAllocate(size_t, AllocOptions = DEFAULT);


	~MemoryManager();


private:
	MemoryManager();
	MemoryManager(Allocator * allocator, int pointerLimit);
	Allocator * allocator;
	Pool * pointerStorage;
	Pool * rcStorage;

	void freeSmartPtr(Marker toFree);
	void freeRC(Marker toFree);
};

template<template<class> class SmartPointer, typename T>
SmartPointer<T> MemoryManager::smartAllocate(size_t size, AllocOptions options)
{
	T * address = (T*)this->allocate(size, options);
	try {
		void* ptr = this->pointerStorage->allocate(size);
		int* rc = (int*)this->rcStorage->allocate(size);
		SmartPointer<T> * pointer = new(ptr) SmartPointer<T>(address, ptr, rc, options);
		return *pointer;
	}
	catch (const char * e) {
		throw "out of pointer / rc storage";
	}
}


