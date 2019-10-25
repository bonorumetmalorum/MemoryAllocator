#pragma once

typedef unsigned long long Marker;

class Stack
{
public:
	explicit Stack(size_t size, void* start);
	void * alloc(size_t size); 
	Marker getMarker();
	void freeToMarker(Marker marker);
	void clear();
	~Stack();

private:
	Marker top;
	void * memory;
	size_t limit;
};

