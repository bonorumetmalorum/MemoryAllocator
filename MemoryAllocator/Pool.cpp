#include "pch.h"
#include "Pool.h"
#include <malloc.h>

Pool::Pool(size_t sizeOfElement, int numElements)
{
	memory = malloc(sizeOfElement * numElements);
	head = memory;
	limit = (sizeOfElement * numElements) + reinterpret_cast<Marker>(memory);
	this->sizeOfElement = sizeOfElement;
	Marker * currentAddress = reinterpret_cast<Marker*>(head);
	Marker nextFreeMarker = reinterpret_cast<Marker>(head) + sizeOfElement;
	for (int i = 1; i < numElements; i++) {
		if (nextFreeMarker >= limit) {
			*currentAddress = NULL;
		}
		else {
			*currentAddress = nextFreeMarker;
		}
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
	if (head == NULL) {
		Marker mem_address = (index * sizeOfElement) + reinterpret_cast<Marker>(memory); //get the memory address by applying the offset
		head = reinterpret_cast<void*>(mem_address); // set the head to the deallocated address
		*(reinterpret_cast<Marker*>(head)) = NULL; //add it to the free list by setting its next to NULL, since there was no more space except this one
	}
	else {
		Marker mem_address = (index * sizeOfElement) + reinterpret_cast<Marker>(memory); //get the memory address by applying the offset
		Marker nextFree = reinterpret_cast<Marker>(head);//get the head as a marker
		void * deallocatedAddress = reinterpret_cast<void*>(mem_address);
		*(reinterpret_cast<Marker*>(deallocatedAddress)) = nextFree;//assign the head as a value to this index
		head = deallocatedAddress;//assign head to this index
	}

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
