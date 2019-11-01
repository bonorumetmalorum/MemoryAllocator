#include "pch.h"
#include "Stack.h"
#include <malloc.h>


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

void * Stack::allocate(size_t size, AllocOptions options)
{
	return this->alloc(size);
}

void Stack::deallocate(Marker pos, AllocOptions _)
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

Marker Stack::getMarker()
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
