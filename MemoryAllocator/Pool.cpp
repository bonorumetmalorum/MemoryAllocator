#include "pch.h"
#include "Pool.h"
#include <malloc.h>

Pool::Pool(size_t sizeOfElement, int numElements)
{
	memory = malloc(sizeOfElement * numElements);
	head = memory; 
	//complete constructor, need to iterate through memory and assign free list :)
}

void * Pool::alloc()
{
	if (currentUsage + sizeOfElement > limit) {
		return nullptr;
	}
	unsigned long long * nextFree = reinterpret_cast<unsigned long long*>(head);
	void * address = head;
	head = reinterpret_cast<void*>(*nextFree);
	return address;
}

void Pool::dealloc(Marker marker)
{
	if (marker > limit) {
		throw "invalid marker";
	}
	Marker nextFree = reinterpret_cast<Marker>(head);
	head = reinterpret_cast<void*>(marker);
	Marker * newFirst = reinterpret_cast<Marker *>(head);
	*newFirst = nextFree;
}

void Pool::clear()
{
	//TODO
}

Pool::~Pool()
{
	//TODO
}
