#pragma once
#include "Stack.h"
#include "DoubleEndedStack.h"
#include "Pool.h"

/*
	TODO: unify the allocators under a common interface
	provide enum to allow for optional allocation parameters (e.g. double ended stack etc)

	make memory manager use one single allocation strategy
*/

class MemoryManager {
public:
	MemoryManager();

	void * allocate(size_t size); //create a new block and add it to the end of the list
	void deallocate(int index);


private:
	size_t capacity; //max capacity of the allocate heap
	size_t currentUsage; //how much of the heap is being used
	int numBlocks;
	void * memory; //start addres of heap


};


