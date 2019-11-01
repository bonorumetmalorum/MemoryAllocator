#include "pch.h"
#include "Pool.h"
#include <malloc.h>

Pool::Pool(size_t sizeOfElement, int numElements)
{
	if (sizeOfElement < sizeof(Marker)) {
		memory = malloc(sizeof(Marker)*numElements);
		this->sizeOfElement = sizeof(Marker);
	}
	else {
		void * address = malloc(sizeOfElement * numElements);
		if (address == nullptr) {
			throw "error allocating memory";
		}	
		else {
			memory = address;
		}
		this->sizeOfElement = sizeOfElement;
	}
	head = memory;
	limit = (this->sizeOfElement * numElements) + reinterpret_cast<Marker>(memory);
	this->numElements = numElements;
	Marker * currentAddress = reinterpret_cast<Marker*>(head);
	Marker nextFreeMarker = reinterpret_cast<Marker>(head) + this->sizeOfElement;
	for (int i = 0; i < numElements; i++) {
		if (i == numElements-1) {
			*currentAddress = 0;
		}
		else {
			*currentAddress = nextFreeMarker;
		}
		currentAddress = (Marker *)reinterpret_cast<void *>(nextFreeMarker);
		nextFreeMarker += this->sizeOfElement;

	}
}

void * Pool::allocate(size_t size, AllocOptions)
{
	if (size > this->sizeOfElement) {
		throw "element too big for block size";
	}
	return this->alloc();
}

void Pool::deallocate(Marker pos, AllocOptions)
{
	this->dealloc(pos);
}

void * Pool::alloc()
{
	if (currentUsage + sizeOfElement > limit) {
		return nullptr;
	}
	Marker * ptr = (Marker*)head;
	Marker nextFree = *(ptr); //next free block marker stored at head
	void * address = head; // get the current address
	if (nextFree == 0) {
		head = 0;
	}
	else {
		void * newHead = reinterpret_cast<void*>(nextFree); //cat next free block marker to void *
		head = newHead; // set the new head to the next free marker (casted above)
	}
	return address; // return the address
}

void Pool::dealloc(Marker pos)
{ //need to check if the memory address is already part of the free list, if so throw an error, double free
	if (isFreed(pos)) {
		throw "invalid marker";
	}
	if (head == 0) {
		head = reinterpret_cast<void*>(pos); // set the head to the deallocated address
		*(reinterpret_cast<Marker*>(head)) = 0; //add it to the free list by setting its next to NULL, since there was no more space except this one
	}
	else {
		Marker nextFree = reinterpret_cast<Marker>(head);//get the head as a marker
		void * deallocatedAddress = reinterpret_cast<void*>(pos);
		*(reinterpret_cast<Marker*>(deallocatedAddress)) = nextFree;//assign the head as a value to this index
		head = deallocatedAddress;//assign head to this index
	}

}

bool Pool::isFreed(Marker address)
{
	bool isFree = false;
	Marker currentFree = reinterpret_cast<Marker>(head);//current free address
	while (currentFree != 0) {
		if (currentFree == address) {
			isFree = true;
			break;
		}
		void * currentRawAddress = reinterpret_cast<void *>(currentFree);//void * current address form
		Marker * nextFree = reinterpret_cast<Marker *>(currentRawAddress);//get the next free address and assign its value to the currentFree
		currentFree = *nextFree;
	}
	return isFree;
}

void Pool::clear()
{
	head = memory;
	Marker * currentAddress = reinterpret_cast<Marker*>(head);
	Marker nextFreeMarker = reinterpret_cast<Marker>(head) + sizeOfElement;
	for (int i = 0; i < this->numElements; i++) {
		if (i == numElements - 1) {
			*currentAddress = 0;
		}
		else {
			*currentAddress = nextFreeMarker;
		}
		currentAddress = (Marker *)reinterpret_cast<void *>(nextFreeMarker);
		nextFreeMarker += sizeOfElement;
	}
}

void* Pool::operator[](int index)
{
	Marker baseAddress = reinterpret_cast<Marker>(memory);
	Marker offset = baseAddress + (this->sizeOfElement * index);
	return reinterpret_cast<void*>(offset);
}

Pool::~Pool()
{
	
}
