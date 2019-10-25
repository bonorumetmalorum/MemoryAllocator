#pragma once

class Pool
{
public:
	Pool(size_t sizeOfElement, int numElements);
	void * alloc();
	void dealloc();
	void clear();
	~Pool();
private:
	void * memory;
	size_t sizeOfElement;
	void * head;
	size_t currentUsage = 0;
	unsigned long long limit;
};

