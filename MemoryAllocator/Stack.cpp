#include "pch.h"
#include "Stack.h"
#include <memory>


Stack::Stack(size_t size, void* start)
{
	this->limit = size;
	this->memory = start;
	this->top = reinterpret_cast<uintptr_t>(start);
}

void * Stack::alloc(size_t size)
{
	if (top + size > reinterpret_cast<Marker>(memory) + limit) {
		return nullptr;
	}
	else {
		void * address = reinterpret_cast<void*>(top);
		top += size + 1;
		return address;
	}
}

uintptr_t Stack::getMarker()
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
