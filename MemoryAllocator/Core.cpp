#include "pch.h"
#include "Core.h"


Core::Core()
{
}


Core::~Core()
{
}

MemoryManager & Core::getMemoryManager()
{
	return m;
}
