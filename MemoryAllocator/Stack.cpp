#include "pch.h"
#include "Stack.h"
#include <malloc.h>


Stack::Stack(size_t size)
{
	this->limit = size;
	this->memory = malloc(size);
	this->top = reinterpret_cast<uintptr_t>(this->memory);
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
	if (top + size > reinterpret_cast<Marker>(memory) + limit) {
		return nullptr;
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
	//TODO
}
