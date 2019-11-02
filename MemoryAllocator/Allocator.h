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
	virtual void deallocate(Marker index, size_t size = 0, AllocOptions = DEFAULT) = 0;
	virtual void clear() = 0;
	virtual ~Allocator() = 0;

protected:
	size_t capacity;
	size_t usage;
	void * memory;


};

