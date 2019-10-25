#pragma once

typedef unsigned long long Marker;

class Pool
{
public:
	Pool(size_t sizeOfElement, int numElements);
	void * alloc();
	void dealloc(Marker marker);
	void clear();
	~Pool();
private:
	void * memory;
	size_t sizeOfElement;
	void * head;
	size_t currentUsage = 0;
	Marker limit;
};

