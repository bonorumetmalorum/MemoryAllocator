#include "pch.h"
#include "Allocator.h"
#include <memory>

Allocator::~Allocator()
{
	free(memory);
}
