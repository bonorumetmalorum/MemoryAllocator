#pragma once
#include "Allocator.h"

typedef unsigned long long Marker;

class Pool : public Allocator
{
public:
	Pool(size_t sizeOfElement, int numElements);
	void * allocate(size_t, AllocOptions = DEFAULT);
	void deallocate(Marker pos, AllocOptions = DEFAULT);
	void clear();
	void * operator[](int);
	~Pool();
private:
	void * memory;
	size_t sizeOfElement;
	void * head;
	size_t currentUsage = 0;
	Marker limit;
	int numElements;
	void * alloc();
	void dealloc(Marker index);
};

