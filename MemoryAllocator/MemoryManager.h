#pragma once
#include "Stack.h"
#include "DoubleEndedStack.h"
#include "Pool.h"
#include "SmartPointer.h"

/*
	MemoryManager wrapper class to use an allocator to allocate and deallocate memory
	MemoryManager must be initialised first otherwise SmartPointer will not work correctly
*/
class MemoryManager {
	
	template<class T> friend class SmartPointer;

public:
	
	static MemoryManager & getInstance();
	void init(Allocator * allocator, int pointerLimit);
	void * allocate(size_t size, AllocOptions = DEFAULT); 
	void deallocate(Marker index, size_t size = 0, AllocOptions = DEFAULT);
	void clear();

	template<template<class> class SmartPointer, typename T>
	SmartPointer<T> smartAllocate(AllocOptions = DEFAULT);


	~MemoryManager();


private:
	MemoryManager();
	MemoryManager(Allocator * allocator, int pointerLimit);
	Allocator * allocator;
	Pool * pointerStorage;
	Pool * rcStorage;
	bool isInit = false;
	void freeSmartPtr(Marker toFree);
	void freeRC(Marker toFree);
};

/*
	Allocates the memory for type T and returns a SmartPointer for access and destruction of the allocated data

	@Param options the desired allocation options
	@return a SmartPointer with type T
*/
template<template<class> class SmartPointer, typename T>
SmartPointer<T> MemoryManager::smartAllocate(AllocOptions options)
{
	size_t size = sizeof(T);
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


