// MemoryAllocator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "MemoryManager.h"
#include <iostream>
#include "MemoryAllocator.h"

/*
	TODO:

	make allocators using placement operator in memory manager
		you will need to allocate the capacity + the size of the allocator
	allignment
		make seperate functions for allignment
*/

using namespace std;

struct Vertex {
	float x;
	float y;
	float z;
};

void testStack() {
	Stack st(100);
	int * p = (int *)st.allocate(sizeof(int));
	*p = 1;
	Marker beforeVert = st.getMarker();
	Vertex * vert = (Vertex *)st.allocate(sizeof(Vertex));
	*vert = Vertex{ 1.0, 2.0, 3.0 };
	Marker afterVert = st.getMarker();
	cout << *p << endl;
	cout << vert->x << " " << vert->y << " " << vert->z << endl;
	st.deallocate(beforeVert);
	Vertex * newVert = (Vertex*)st.allocate(sizeof(Vertex));
	*newVert = Vertex{ 1.0, 1.0, 1.0 };
	cout << "after deallocation and reallocation" << endl;
	cout << "vert: " << vert->x << " " << vert->y << " " << vert->z << " newVert: " << newVert->x << " " << newVert->y << " " << newVert->z << endl;
}

void testStackFull() {
	Stack st(3);
	char * char1 = (char*)st.allocate(sizeof(char));
	char * char2 = (char*)st.allocate(sizeof(char));
	char * char3 = (char*)st.allocate(sizeof(char));
	*char1 = 'a';
	*char2 = 'b';
	*char3 = 'c';
	//should be full at this point
	char * char4 = (char*)st.allocate(sizeof(char));
	cout << (char4 == nullptr) << endl;
}

void testDoubleEndedStack() {

	//double ended stack test
	DoubleEndedStack dst(100);

	int * inta = (int*)dst.allocate(sizeof(int), AllocOptions::BOTTOM);
	*inta = 12;
	Marker intaM = dst.getMarkerBottom();
	int * intb = (int*)dst.allocate(sizeof(int), AllocOptions::TOP);
	Marker intbM = dst.getMarkerTop();
	Vertex * verta = (Vertex*)dst.allocate(sizeof(Vertex), AllocOptions::BOTTOM);
	*verta = Vertex{ 1.0,1.0,1.0 };
	Marker vertaM = dst.getMarkerBottom();
	Vertex * vertb = (Vertex*)dst.allocate(sizeof(Vertex), AllocOptions::TOP);
	*vertb = Vertex{ 2.0,2.0,2.0 };
	Marker vertbM = dst.getMarkerTop();

	cout << "vertex a: " << verta->x << " " << verta->y << " " << verta->z << endl;
	cout << "vertex b: " << vertb->x << " " << vertb->y << " " << vertb->z << endl;

	dst.deallocate(intaM, AllocOptions::BOTTOM);
	dst.deallocate(intbM, AllocOptions::TOP);

	Vertex * vertc = (Vertex*)dst.allocate(sizeof(Vertex), AllocOptions::BOTTOM);
	*vertc = Vertex{ 5.0,15.0,15.0 };
	Marker vertcM = dst.getMarkerBottom();
	Vertex * vertd = (Vertex*)dst.allocate(sizeof(Vertex), AllocOptions::TOP);
	*vertd = Vertex{ 23.0,23.0,23.0 };
	Marker vertdM = dst.getMarkerTop();

	cout << "vertex a: " << verta->x << " " << verta->y << " " << verta->z << endl;
	cout << "vertex b: " << vertb->x << " " << vertb->y << " " << vertb->z << endl;

	cout << "vertex c: " << vertc->x << " " << vertc->y << " " << vertc->z << endl;
	cout << "vertex d: " << vertd->x << " " << vertd->y << " " << vertd->z << endl;

	dst.clear();

}

void testPool() {
	//pool allocator test

	Pool pool(sizeof(Vertex), 3);

	for (int i = 0; i < 3; i++) {
		Vertex* vert = (Vertex*)pool.allocate(sizeof(Vertex));
		vert->x = i; vert->y = i; vert->z = i;
	}
	Vertex * vertTest;
	for (int j = 0; j < 3; j++) {
		vertTest = (Vertex *)pool[j];
		cout << vertTest->x << " " << vertTest->y << " " << vertTest->z << endl;
	}

	void * marker0 = pool[0];
	pool.deallocate(reinterpret_cast<Marker>(marker0));

	Vertex * newVertTest = (Vertex*)pool.allocate(sizeof(Vertex));
	*newVertTest = Vertex{ 5.0, 5.0, 5.0 };

	Vertex * checkNewVert = (Vertex*)pool[0];
	cout << "after deallocation and re allocation: " << checkNewVert->x << " " << checkNewVert->y << " " << checkNewVert->z << endl;


	void * marker2 = pool[2];
	pool.deallocate(reinterpret_cast<Marker>(marker2));

	Vertex * newVert2 = (Vertex*)pool.allocate(sizeof(Vertex));
	*newVert2 = Vertex{ 5.0, 5.0, 5.0 };

	Vertex * checkNewVert2 = (Vertex*)pool[2];
	cout << "after deallocation and re allocation: " << checkNewVert2->x << " " << checkNewVert2->y << " " << checkNewVert2->z << endl;

	//deallocate everything
	for (int i = 0; i < 3; i++) {
		void* pos = pool[i];
		pool.deallocate(reinterpret_cast<Marker>(pos));
	}

	//allocate everything again
	for (int i = 0; i < 3; i++) {
		Vertex * x = (Vertex *)pool.allocate(sizeof(Vertex));
		x->x = i;
		x->y = i;
		x->z = i;
	}

	//print everything again
	cout << "after clearing pool manually" << endl;
	for (int i = 0; i < 3; i++) {
		Vertex * v = (Vertex *)pool[i];
		cout << v->x << " " << v->y << " " << v->z << endl;
	}
}

void smartPointerTest() {
	int * ptr = new int;
	int * rc = new int;
	SmartPointer<int> * pointer = new SmartPointer<int>(ptr, rc);
	SmartPointer<int>pointer2 = *pointer;
	cout << pointer->getCounter() << endl;
}


void smartPointerAccessTest() {
	int * p = new int;
	int * rc = new int;
	SmartPointer<int> pointer = *(new SmartPointer<int>(p, rc));
	SmartPointer<int> pointer2 = pointer;
	*pointer2 = 2;
	cout << *pointer2 << endl;
}

void memoryAllocatorTest() {
	MemoryManager manager = MemoryManager::initStack(100, 100); //lazy initialised singleton
	Vertex * vert = (Vertex*)manager.allocate(sizeof(Vertex));
	vert->x = 100;
	vert->y = 200;
	vert->z = 300;

	cout << vert->x << " " << vert->y << " " << vert->z << endl;
	
	manager.deallocate(reinterpret_cast<Marker>(vert));

	Vertex* vert2 = (Vertex*)manager.allocate(sizeof(Vertex));
	
	vert2->x = 200;
	vert2->y = 300;
	vert2->z = 400;

	cout << vert->x << " " << vert->y << " " << vert->z << endl;

	cout << vert2->x << " " << vert2->y << " " << vert2->z << endl;
}

void memoryManagerSmartAllocateTest() {
	SmartPointer<Vertex> s = MemoryManager::initStack(100, 100).smartAllocate<SmartPointer, Vertex>(sizeof(Vertex)); //need to make a better method for this, very clunky to keep calling over and over to get the instance
	(*s).x = 1.0;
	(*s).y = 2.0;
	(*s).z = 3.0;
	cout << (*s).x << (*s).y << (*s).z << endl;

	cout << sizeof(SmartPointer<Vertex>) << " " << sizeof(SmartPointer<int>) << endl;

	SmartPointer<Vertex> t = MemoryManager::initStack(100, 100).smartAllocate<SmartPointer, Vertex>(sizeof(Vertex)); //need to make a better method for this, very clunky to keep calling over and over to get the instance
	(*t).x = 6.0;
	(*t).y = 6.0;
	(*t).z = 6.0;
	cout << (*t).x << (*t).y << (*t).z << endl;


}

void testPoolInt() {
	Pool * pool = new Pool(sizeof(unsigned int), 3);
	int * rc1 = (int*)pool->allocate(sizeof(int));
	int * rc2 = (int*)pool->allocate(sizeof(int));
}

int main()
{
	//testStack();

	//testStackFull();

	//testDoubleEndedStack();

	//testPool();

	testPoolInt();

	//smartPointerTest();

	//memoryManagerSmartAllocateTest();

	//memoryAllocatorTest();



	return 0;
}
