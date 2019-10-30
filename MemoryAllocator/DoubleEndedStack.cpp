#include "pch.h"
#include "DoubleEndedStack.h"
#include <malloc.h>

DoubleEndedStack::DoubleEndedStack(size_t limit)
{
	this->memory = malloc(limit);
	Marker mem = reinterpret_cast<unsigned long long>(this->memory);
	topTop = mem + limit;
	bottomTop = mem;
}

void * DoubleEndedStack::allocTop(size_t size)
{
	if (topTop - size < bottomTop) {
		return nullptr;
	}
	else {
		topTop -= size + 1;
		void * address = (void *)topTop;
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

Marker DoubleEndedStack::getMarkerTop()
{
	return topTop;
}

Marker DoubleEndedStack::getMarkerBottom()
{
	return bottomTop;
}

void DoubleEndedStack::clear()
{
	bottomTop =  reinterpret_cast<Marker>(memory);
	topTop = (reinterpret_cast<Marker>(memory)) + limit;
}
