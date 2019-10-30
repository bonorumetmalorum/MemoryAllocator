#pragma once
#include "Allocator.h"

typedef unsigned long long Marker;

class Stack : public Allocator
{
public:
	explicit Stack(size_t size);
	void * allocate(size_t, AllocOptions = DEFAULT);
	void deallocate(Marker, AllocOptions = DEFAULT);
	Marker getMarker();
	void clear();
	~Stack();

private:
	Marker top;
	void * memory;
	size_t limit;

	void * alloc(size_t size);
	void freeToMarker(Marker marker);

};

