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

void * DoubleEndedStack::allocate(size_t size, AllocOptions options)
{
	switch (options) {
	case DEFAULT:
		return this->allocBottom(size);
		break;
	case TOP:
		return this->allocTop(size);
		break;
	case BOTTOM:
		return this->allocBottom(size);
		break;
	default:
		throw "invalid option";
		break;
	}
	return nullptr;
}

void DoubleEndedStack::deallocate(Marker pos, AllocOptions options)
{
	switch (options) {
	case DEFAULT:
		this->freeToMarkerBottom(pos);
		break;
	case TOP:
		this->freeToMarkerTop(pos);
		break;
	case BOTTOM:
		this->freeToMarkerBottom(pos);
		break;
	default:
		throw "invalid option";
		break;
	}
}

void * DoubleEndedStack::allocTop(size_t size)
{
	if (topTop - size < bottomTop) {
		return nullptr;
	}
	else {
		topTop -= size;
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
		bottomTop += size;
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
