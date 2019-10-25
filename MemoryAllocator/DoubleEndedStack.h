#pragma once

typedef unsigned long Marker;

class DoubleEndedStack
{
public:
	DoubleEndedStack();
	void* allocTop(size_t size);
	void* allocBottom(size_t size);
	void freeToMarkerTop(Marker marker);
	void freeToMarkerBottom(Marker marker);
	void clear();
	~DoubleEndedStack();
private:
	size_t limit;
	Marker topTop;
	Marker bottomTop;
	void * memory;
};

