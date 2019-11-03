#include "pch.h"
#include "DoubleEndedStack.h"
#include <malloc.h>

/*
	Construct a DoubleEndedStack
	@param limit the total size of the memory managed by this DoubleEndedStack
	@throws if limit <= 0
	@throws if malloc fails to allocate memory
*/
DoubleEndedStack::DoubleEndedStack(size_t limit)
{
	if (limit <= 0) {
		throw "size must be larger than 0";
	}
	void * address = malloc(limit);
	if (address == nullptr) {
		throw "error allocating memory";
	}
	else {
		this->memory = address;
	}
	capacity = limit;
	Marker mem = reinterpret_cast<unsigned long long>(this->memory);
	topTop = mem + limit;
	bottomTop = mem;
}

/*
	Allocate the memory
	@param size size of memory to allocate
	@param options allocate to TOP or to BOTTOM stack
	@throws if invalid option is given
	@throws if top or bottom stack is out of memory
	@return void *  to the address of memory allocated
*/
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

/*
	Deallocate memory
	@param pos memory address to deallocate
	@param size size of memory to deallocate
	@options deallocate from TOP or BOTTOM
	@throws if invalid option is given
	@throws if the marker fails boundary checks
*/
void DoubleEndedStack::deallocate(Marker pos, size_t size, AllocOptions options)
{
	switch (options) {
	case DEFAULT:
		this->freeToMarkerBottom(pos);
		break;
	case TOP:
		this->freeToMarkerTop(pos, size);
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
		throw "out of memory top";
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
		throw "out of memory bottom";
	}
	else {
		void * address = (void*) bottomTop;
		bottomTop += size;
		return address;
	}
}

void DoubleEndedStack::freeToMarkerTop(Marker marker, size_t size)
{
	if (marker < topTop || marker > reinterpret_cast<Marker>(memory) + capacity) {
		throw "invalid marker";
	}
	topTop = marker + size;
}

void DoubleEndedStack::freeToMarkerBottom(Marker marker)
{
	if (marker > bottomTop || marker < reinterpret_cast<Marker>(memory)) {
		throw "invalid marker";
	}
	bottomTop = marker;
}

/*
	get the top of the top stack
	@return Marker current position of the top of the top stack (points to last allocated position)
*/
Marker DoubleEndedStack::getMarkerTop()
{
	return topTop;
}

/*
	get the top of the bottom stack
	@return Marker current position of the top of the bottom stack (points to free memory)
*/
Marker DoubleEndedStack::getMarkerBottom()
{
	return bottomTop;
}

/*
	resets both bottom and top stacks, does not delete the memory but allows for their reallocation
*/
void DoubleEndedStack::clear()
{
	bottomTop =  reinterpret_cast<Marker>(memory);
	topTop = (reinterpret_cast<Marker>(memory)) + capacity;
}
