#pragma once
#include "MemoryManager.h"

template<class T>
class SmartPointer
{

public:
	SmartPointer(T * rawAddress, int* rc, AllocOptions options = DEFAULT);
	SmartPointer(const SmartPointer& other);
	SmartPointer& operator=(SmartPointer& other);
	T & operator *();
	//T * operator ->();
	//implement other operators [], ++, --
	int getCounter();
	~SmartPointer();

private:
	T * rawAddress;
	int * counter;
	AllocOptions options;
};

template<class T>
inline SmartPointer<T>::SmartPointer(T * rawAddress, int * rc, AllocOptions options)
{
	this->rawAddress = rawAddress;
	this->counter = rc;
	*(this->counter) = 1;
	this->options = options;
}

template<class T>
inline SmartPointer<T>::SmartPointer(const SmartPointer & other)
{
	counter = other.counter;
	rawAddress = other.rawAddress;
	*counter += 1;
	options = other.options;
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
		Marker toDelete = reinterpret_cast<Marker>(rawAddress);
		Marker rcToDelete = reinterpret_cast<Marker>(counter);
		Marker smartptrToDelete = reinterpret_cast<Marker>(this);
		MemoryManager::initStack(100, 100).deallocate(toDelete, this->options);
		MemoryManager::initStack(100, 100).freeRC(rcToDelete);
		MemoryManager::initStack(100, 100).freeSmartPtr(smartptrToDelete);
	}

	return *this;
}

template<class T>
inline T & SmartPointer<T>::operator*()
{
	//if the memory is already free this will throw an error as invalid marker
	return *rawAddress;
}

//template<class T>
//inline T * SmartPointer<T>::operator->()
//{
//	return &rawAddress;
//}

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
		if (this->options == TOP) {
			MemoryManager::initStack(100, 100).deallocate(toDelete, sizeof(T), this->options);
		}
		else {
			MemoryManager::initStack(100, 100).deallocate(toDelete, this->options);
		}
		MemoryManager::initStack(100, 100).freeRC(rcToDelete);
		MemoryManager::initStack(100, 100).freeSmartPtr(smartptrToDelete);

	}
}
