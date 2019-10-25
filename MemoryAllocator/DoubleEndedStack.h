#pragma once

typedef unsigned long long Marker;

class DoubleEndedStack
{
public:
	explicit DoubleEndedStack(size_t limit, void * memory);
	void* allocTop(size_t size);
	void* allocBottom(size_t size);
	void freeToMarkerTop(Marker marker);
	void freeToMarkerBottom(Marker marker);
	Marker getMarkerTop();
	Marker getMarkerBottom();
	void clear();
private:
	size_t limit;
	Marker topTop;
	Marker bottomTop;
	void * memory;
};

