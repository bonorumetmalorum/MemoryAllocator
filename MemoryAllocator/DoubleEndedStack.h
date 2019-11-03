#pragma once
#include "Allocator.h"

/*
	Double ended stack allocator
	two stacks, top and bottom, located at either end of the chunk of memory, that grow inwards
*/
class DoubleEndedStack : public Allocator
{
public:
	explicit DoubleEndedStack(size_t limit);
	void * allocate(size_t, AllocOptions = DEFAULT);
	void deallocate(Marker pos, size_t size, AllocOptions = DEFAULT);

	Marker getMarkerTop();
	Marker getMarkerBottom();
	void clear();
private:
	Marker topTop;
	Marker bottomTop;
	
	void * allocTop(size_t);
	void * allocBottom(size_t);

	void freeToMarkerTop(Marker marker, size_t size);
	void freeToMarkerBottom(Marker marker);
};

