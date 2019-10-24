#include "pch.h"
#include "Stack.h"
#include <memory>


Stack::Stack(size_t size, void* start)
{
	this->limit = size;
	this->memory = start;
	this->top = (unsigned long)start;
}

void * Stack::alloc(size_t size)
{
	if (top + size > (unsigned long)memory + limit) {
		return nullptr;
	}
	else {
		void * address = (void *)top;
		top += size + 1;
		return address;
	}
}

Marker Stack::getMarker()
{
	return Marker();
}

void Stack::freeToMarker(Marker marker)
{
}

void Stack::clear()
{
}

Stack::~Stack()
{
}
