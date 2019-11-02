#pragma once
#include "Allocator.h"

typedef unsigned long long Marker;

class Stack : public Allocator
{
public:
	explicit Stack(size_t size);
	void * allocate(size_t, AllocOptions = DEFAULT);
	void deallocate(Marker, size_t size = 0, AllocOptions = DEFAULT);
	Marker getTop();
	void clear();
	~Stack();

private:
	Marker top;
	void * alloc(size_t size);
	void freeToMarker(Marker marker);

};

