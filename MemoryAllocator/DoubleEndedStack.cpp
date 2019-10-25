#include "pch.h"
#include "DoubleEndedStack.h"


DoubleEndedStack::DoubleEndedStack()
{
}

void * DoubleEndedStack::allocTop(size_t size)
{
	if (topTop - size < bottomTop) {
		return nullptr;
	}
	else {
		void * address = (void *)topTop;
		topTop -= size + 1;
		return address;
	}
}

void * DoubleEndedStack::allocBottom(size_t size)
{
	if (bottomTop + size > topTop) {
		return nullptr;
	}
	else {
		void * address = (void*) bottomTop;
		bottomTop += size + 1;
		return address;
	}
}

void DoubleEndedStack::freeToMarkerTop(Marker marker)
{
	if (marker < bottomTop) {
		throw "invalid marker";
	}
	topTop = marker;
}

void DoubleEndedStack::freeToMarkerBottom(Marker marker)
{
	if (marker > bottomTop) {
		throw "invalid marker";
	}
	bottomTop = marker;
}

void DoubleEndedStack::clear()
{
	bottomTop = (unsigned long) memory;
	topTop = ((unsigned long)memory) + limit;
}


DoubleEndedStack::~DoubleEndedStack()
{
	//TODO
}
