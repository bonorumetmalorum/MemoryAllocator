#pragma once

enum AllocOptions{
	DEFAULT,
	TOP,
	BOTTOM
};

typedef unsigned long long Marker;

class Allocator
{
public:
	virtual void * allocate(size_t size, AllocOptions op = DEFAULT) = 0;
	virtual void deallocate(Marker index, AllocOptions = DEFAULT) = 0;
	~Allocator();

protected:
	size_t capacity;
	size_t usage;
	void * memory;


};

