#pragma once
#include "Stack.h"
#include "DoubleEndedStack.h"
#include "Pool.h"
#include "SmartPointer.h"

/*

WHAT NOW?

need to add a clear method

smart pointes need to be implemented
but where do they go? they need to put into one of the allocators.

*/

class MemoryManager {
public:
	

	static MemoryManager & initStack(size_t size, int pointerLimit);
	static MemoryManager & initDoubleStack(size_t size, int pointerLimit);
	static MemoryManager & initPool(size_t size, int num_elements, int pointerLimit);

	void* allocate(size_t size, AllocOptions = DEFAULT); 
	void deallocate(Marker index, AllocOptions = DEFAULT);


private:
	MemoryManager();
	MemoryManager(Allocator * allocator, int pointerLimit);
	Allocator * allocator;
	Pool * pointerStorage;


};


