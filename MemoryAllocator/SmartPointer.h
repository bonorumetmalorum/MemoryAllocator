#pragma once
#include "MemoryManager.h"


class SmartPointer
{
public:
	SmartPointer(void * rawAddress);
	SmartPointer(const SmartPointer& other);
	SmartPointer& operator=(SmartPointer& other);
	template<typename T>
	T & operator *();
	~SmartPointer();
private:
	void * rawAddress;
	int * counter;
};

