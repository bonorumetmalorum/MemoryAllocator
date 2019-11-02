#pragma once
#include "MemoryManager.h"
#include <ostream>

template<class T>
class SmartPointer
{

public:
	SmartPointer(T * rawAddress, void * selfAddress, int* rc, AllocOptions options = DEFAULT);
	SmartPointer(const SmartPointer& other);
	SmartPointer& operator=(SmartPointer& other);
	T & operator *();
	//T * operator ->();
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

template<class T>
inline SmartPointer<T>::SmartPointer(T * rawAddress, void * selfAddress, int * rc, AllocOptions options)
{
	this->rawAddress = rawAddress;
	this->selfAddress = selfAddress;
	this->counter = rc;
	*(this->counter) = 1;
	this->options = options;
}

template<class T>
inline SmartPointer<T>::SmartPointer(const SmartPointer & other)
{
	counter = other.counter;
	rawAddress = other.rawAddress;
	selfAddress = other.selfAddress;
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
