#pragma once
#include "Stack.h"
#include "DoubleEndedStack.h"
#include "Pool.h"

/*
	there will be at any given time on management strategy

	not making this a singleton global state object
		this would make concurrency difficult
		would add extra book keeping making this quite class quite large when it would be easier to have multiple MemoryManagers managing independently using different allocation strategies

	the goal would be to give total control to the user
	allowing them to allocate the memory for the allocators they want to use when they want

*/

template<class Allocator>
class MemoryManager
{
public:
	explicit MemoryManager(size_t size);
	void * insert();
	void remove();
	void * access(size_t);
	~MemoryManager();

private:
	Allocator allocator;
};

template<>
class MemoryManager<DoubleEndedStack> 
{
public:
	void * insertTop();
	void * insertBottom();
	void deallocateTop();
	void deallocateBottom();

private:

	DoubleEndedStack allocator;


};

template<>
class MemoryManager<Pool> 
{
public:
	explicit MemoryManager(size_t size, int num_elements);

private:
	Pool * allocator;


};

//override the new operators, arrays and all that stuff here
//create a new operator for each kind of memory allocator available
//if that specific memory allocator is not yet created to manage memory throw an error


