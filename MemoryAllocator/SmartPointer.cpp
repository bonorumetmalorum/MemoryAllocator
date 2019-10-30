#include "pch.h"
#include "SmartPointer.h"

SmartPointer::SmartPointer(void * rawAddress)
{
}

SmartPointer::SmartPointer(SmartPointer & other)
{
	this->counter = other.counter;
	this->rawAddress = other.rawAddress;
	*(this->counter)++;
}

template<typename T>
T & SmartPointer::operator*()
{
	return reinterpret_cast<T>(rawAddress);
}

SmartPointer::~SmartPointer()
{
}
