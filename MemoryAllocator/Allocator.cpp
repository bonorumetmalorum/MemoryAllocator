#include "pch.h"
#include "Allocator.h"

Allocator::~Allocator()
{
	delete(memory);
}
