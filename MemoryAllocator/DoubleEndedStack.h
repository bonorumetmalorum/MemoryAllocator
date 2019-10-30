#pragma once
#include "Allocator.h"

typedef unsigned long long Marker;

class DoubleEndedStack : public Allocator
{
public:
	explicit DoubleEndedStack(size_t limit);
	void * allocate(size_t, AllocOptions = DEFAULT);
	void deallocate(Marker pos, AllocOptions = DEFAULT);

	Marker getMarkerTop();
	Marker getMarkerBottom();
	void clear();
private:
	size_t limit;
	Marker topTop;
	Marker bottomTop;
	void * memory;
	
	void * allocTop(size_t);
	void * allocBottom(size_t);

	void freeToMarkerTop(Marker marker);
	void freeToMarkerBottom(Marker marker);
};

