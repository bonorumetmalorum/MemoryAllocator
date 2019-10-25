#pragma once

typedef unsigned long Marker;

class DoubleEndedStack
{
public:
	DoubleEndedStack();
	void* allocTop(size_t size);
	void* allocBottom(size_t size);
	void deallocTop(Marker marker);
	void deallocBottom(Marker marker);
	void clear();
	~DoubleEndedStack();
};

