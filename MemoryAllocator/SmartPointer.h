#pragma once
#include "MemoryManager.h"
#include <ostream>

/*
	SmartPointer to control reference counting and automatic deallocation
	SmartPointer will not work correctly without a correctly configured MemoryManager
*/
template<class T>
class SmartPointer
{

public:
	SmartPointer(T * rawAddress, void * selfAddress, int* rc, AllocOptions options = DEFAULT);
	SmartPointer(const SmartPointer& other);
	SmartPointer& operator=(SmartPointer& other);
	T & operator *();
	//implement other operators [], ++, --
	friend std::ostream& operator <<(std::ostream& os, const SmartPointer& ptr) {
		os << ptr.rawAddress;
		return os;
	};
	int getCounter();
	~SmartPointer();


private:
	T * rawAddress;
	void * selfAddress;
	int * counter;
	AllocOptions options;
};

/*
	construct a SmartPointer
	@param rawAddress the address of allocated data
	@param selfAddress the address at which this smart pointer was allocated
	@param rc the address at which this SmartPointers reference count is located
	@param	options the options used when the allocation was initially made
*/
template<class T>
inline SmartPointer<T>::SmartPointer(T * rawAddress, void * selfAddress, int * rc, AllocOptions options)
{
	this->rawAddress = rawAddress;
	this->selfAddress = selfAddress;
	this->counter = rc;
	*(this->counter) = 1;
	this->options = options;
}

/*
	Shallow copy constructor
*/
template<class T>
inline SmartPointer<T>::SmartPointer(const SmartPointer & other)
{
	counter = other.counter;
	rawAddress = other.rawAddress;
	selfAddress = other.selfAddress;
	*counter += 1;
	options = other.options;
}

/*
	assignment operator
	destroy the left hand size SmarPointer and create a reference of the Right hand size SmartPointer
*/
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

/*
	dereference opertor
	@return T typed reference to the underlying data
*/
template<class T>
inline T & SmartPointer<T>::operator*()
{
	return *rawAddress;
}

/*
	get the reference count
	@return int the current number of references
*/
template<class T>
inline int SmartPointer<T>::getCounter()
{
	return *counter;
}

/*
	destroy the reference and if reference count is 1 destroy the data along with this pointer
	@throws if memory manager is not initialised correctly
*/
template<class T>
inline SmartPointer<T>::~SmartPointer()
{
	(*counter)--;
	if (*counter == 1) {
		Marker toDelete = reinterpret_cast<Marker>(rawAddress);
		Marker rcToDelete = reinterpret_cast<Marker>(counter);
		Marker smartptrToDelete = reinterpret_cast<Marker>(selfAddress);
		if (this->options == TOP) {
			size_t size = sizeof(T);
			MemoryManager::getInstance().deallocate(toDelete, size, this->options);
		}
		else {
			MemoryManager::getInstance().deallocate(toDelete, this->options);
		}
		MemoryManager::getInstance().freeRC(rcToDelete);
		MemoryManager::getInstance().freeSmartPtr(smartptrToDelete);

	}
}
