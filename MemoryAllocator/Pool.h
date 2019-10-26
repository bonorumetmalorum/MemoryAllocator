#pragma once

typedef unsigned long long Marker;

class Pool
{
public:
	Pool(size_t sizeOfElement, int numElements);
	void * alloc();
	void dealloc(int index);
	void clear();
	void * operator[](int);
	~Pool();
private:
	void * memory;
	size_t sizeOfElement;
	void * head;
	size_t currentUsage = 0;
	Marker limit;
};

