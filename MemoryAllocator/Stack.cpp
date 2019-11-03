#include "pch.h"
#include "Stack.h"
#include <malloc.h>

/*
	Construct a stack
	@param size the total size of memory managed by this stack
*/
Stack::Stack(size_t size)
{
	if (size <= 0) {
		throw "invalid size, size must be greater than 0";
	}
	this->capacity = size;
	void * address = malloc(size);
	if (address == nullptr) {
		throw "error allocating memory";
	}
	else {
		this->memory = address;
	}
	this->top = reinterpret_cast<Marker>(this->memory);
}

/*
	allocate memory
	@param size the amount of memory being allocated
	@param AllocOptions not used in this case
	@throw when out of memory
	@return void * address of memory that was allocated
*/
void * Stack::allocate(size_t size, AllocOptions options)
{
	return this->alloc(size);
}

/*
	deallcoate the memory
	@param size the amount of memory being allocated
	@param AllocOptions not used in this case
	@throw when the marker fails boundary checks
*/
void Stack::deallocate(Marker pos, size_t size, AllocOptions _)
{
	this->freeToMarker(pos);
}

void * Stack::alloc(size_t size)
{
	if (top + size > reinterpret_cast<Marker>(memory) + capacity) {
		throw "out of memory";
	}
	else {
		void * address = reinterpret_cast<void*>(top);
		top += size;
		return address;
	}
}

/*
	get the current location of the top marker
	@return Marker the current top (points to free memory)
*/
Marker Stack::getTop()
{
	return this->top;
}

void Stack::freeToMarker(Marker marker)
{
	if (marker > this->top) {
		throw "error, marker is not valid";
	}
	this->top = marker;
}

void Stack::clear()
{
	this->top = reinterpret_cast<Marker>(memory);
}

Stack::~Stack()
{

}
