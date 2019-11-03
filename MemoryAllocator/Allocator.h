#pragma once

/*
	common options to be used when allocating using the memory manager or allocators
*/
enum AllocOptions{
	DEFAULT,
	TOP,
	BOTTOM
};

/*
	redefintion of unsigned long long for convenience
*/
typedef unsigned long long Marker;

/*
	Interface to be used when creating new allocators
*/
class Allocator
{
public:
	/*
		allocate memory
		@param size the size of memory to allocate
		@param op the options used to make the allocation
		@return void * the address of the memory that has been allocated
	*/
	virtual void * allocate(size_t size, AllocOptions op = DEFAULT) = 0;
	/*
		deallocate memory
		@param index the memory to deallocate
		@param size the size of the memory to deallocate
		@param op the options to use when deallocating
	*/
	virtual void deallocate(Marker index, size_t size = 0, AllocOptions op = DEFAULT) = 0;
	/*
		reset the allocator
	*/
	virtual void clear() = 0;
	virtual ~Allocator() = 0;

protected:
	size_t capacity;
	size_t usage;
	void * memory;


};

