#pragma once

//manage all memory blocks
//memory blocks are stacks, double ended stack and pools
//each memory instance will control one block of heap allocated memory using one of the provided allocators


class Memory
{
public:
	Memory(size_t size);
	~Memory();

private:
	void * stack;
	void * dst;
	void * pool;
};

//override the new operators, arrays and all that stuff here
//create a new operator for each kind of memory allocator available
//if that specific memory allocator is not yet created to manage memory throw an error