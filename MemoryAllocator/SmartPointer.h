#pragma once
#include "MemoryManager.h"

template<class T>
class SmartPointer
{

public:
	SmartPointer(T * rawAddress, int* rc);
	SmartPointer(const SmartPointer& other);
	SmartPointer& operator=(SmartPointer& other);
	T & operator *();
	int getCounter();
	~SmartPointer();

private:
	T * rawAddress;
	int * counter;
};

template<class T>
inline SmartPointer<T>::SmartPointer(T * rawAddress, int * rc)
{
	this->rawAddress = rawAddress;
	this->counter = rc;
	*(this->counter) = 1;
}

template<class T>
inline SmartPointer<T>::SmartPointer(const SmartPointer & other)
{
	this->counter = other.counter;
	this->rawAddress = other.rawAddress;
	*counter += 1;
}

template<class T>
inline SmartPointer<T> & SmartPointer<T>::operator=(SmartPointer<T> & other)
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

template<class T>
inline T & SmartPointer<T>::operator*()
{
	//if the memory is already free this will throw an error as invalid marker
	return *rawAddress;
}

template<class T>
inline int SmartPointer<T>::getCounter()
{
	return *counter;
}

template<class T>
inline SmartPointer<T>::~SmartPointer()
{
	(*counter)--;
	if (*counter == 1) {
		Marker toDelete = reinterpret_cast<Marker>(rawAddress);
		Marker rcToDelete = reinterpret_cast<Marker>(counter);
		Marker smartptrToDelete = reinterpret_cast<Marker>(this);
		MemoryManager::initStack(100, 100).deallocate(toDelete);
		MemoryManager::initStack(100, 100).freeRC(rcToDelete);
		MemoryManager::initStack(100, 100).freeSmartPtr(smartptrToDelete);

	}
}
