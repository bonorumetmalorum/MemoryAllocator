#include "pch.h"
#include "SmartPointer.h"

SmartPointer::SmartPointer(void * rawAddress)
{
}

SmartPointer::SmartPointer(const SmartPointer & other)
{
	this->counter = other.counter;
	this->rawAddress = other.rawAddress;
	*(this->counter)++;
}

SmartPointer & SmartPointer::operator=(SmartPointer & other)
{
	void * prevAddress = this->rawAddress;
	int * prevCounter = this->counter;

	this->rawAddress = other.rawAddress;
	this->counter = other.counter;

	*(counter)++;
	--(*prevCounter);

	if (*prevCounter == 0) {
		//delete the old data here
		//need to setup the global memory manager to do this
	}

	return *this;
}

template<typename T>
T & SmartPointer::operator*()
{
	return reinterpret_cast<T>(rawAddress);
}

SmartPointer::~SmartPointer()
{
	*counter--;
	if (*counter == 0) {
		//delete the data
		//setup the global memory manager to do this
	}
}
