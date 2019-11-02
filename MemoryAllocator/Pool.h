#pragma once
#include "Allocator.h"

typedef unsigned long long Marker;

class Pool : public Allocator
{
public:
	Pool(size_t sizeOfBlock, int numElements);
	void * allocate(size_t, AllocOptions = DEFAULT);
	void deallocate(Marker pos, size_t size = 0, AllocOptions = DEFAULT);
	void clear();
	void * operator[](int);
	~Pool();
private:
	size_t sizeOfBlock;
	void * head;
	Marker limit;
	int numElements;
	void * alloc();
	void dealloc(Marker index);
	bool isFreed(Marker address);
};

