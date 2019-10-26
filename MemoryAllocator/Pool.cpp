#include "pch.h"
#include "Pool.h"
#include <malloc.h>

Pool::Pool(size_t sizeOfElement, int numElements)
{
	memory = malloc(sizeOfElement * numElements);
	head = memory;
	limit = (sizeOfElement * numElements);
	this->sizeOfElement = sizeOfElement;
	Marker * currentAddress = reinterpret_cast<Marker*>(head);
	Marker nextFreeMarker = reinterpret_cast<Marker>(head) + sizeOfElement;
	for (int i = 1; i < numElements; i++) {
		*currentAddress = nextFreeMarker;
		currentAddress = (Marker *)reinterpret_cast<void *>(nextFreeMarker);
		nextFreeMarker += sizeOfElement;
	}
}

void * Pool::alloc()
{
	if (currentUsage + sizeOfElement > limit) {
		return nullptr;
	}
	Marker nextFree = *(reinterpret_cast<Marker*>(head)); //next free block marker stored at head
	void * newHead = reinterpret_cast<void*>(nextFree); //cat next free block marker to void *
	void * address = head; // get the current address
	head = newHead; // set the new head to the next free marker (casted above)
	return address; // return the address
}

void Pool::dealloc(int index)
{
	Marker nextFree = reinterpret_cast<Marker>(head);
	head = reinterpret_cast<void*>(marker);
	Marker * newFirst = reinterpret_cast<Marker *>(head);
	*newFirst = nextFree;
}

void Pool::clear()
{
	//TODO
}

void* Pool::operator[](int index)
{
	Marker baseAddress = reinterpret_cast<Marker>(memory);
	Marker offset = baseAddress + this->sizeOfElement * index;
	return reinterpret_cast<void*>(offset);
}

Pool::~Pool()
{
	//TODO
}
