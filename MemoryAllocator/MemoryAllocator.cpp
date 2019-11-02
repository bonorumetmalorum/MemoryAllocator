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

//-------------------------------------------------
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

void testStackConstruction() {
	Stack * st = new Stack(0);
}

//------------------------------------------------
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

void testDStackFullTop() {
	DoubleEndedStack * dst = new DoubleEndedStack(3);
	dst->allocate(sizeof(char), BOTTOM);
	dst->allocate(sizeof(char), TOP);
	dst->allocate(sizeof(char), TOP);
	try {
		dst->allocate(sizeof(char), TOP);
	}
	catch (const char* e) {
		cout << e << endl;
	}
}

void testDStackTopDeallocate() {
	DoubleEndedStack * dst = new DoubleEndedStack(100);
	Marker original = dst->getMarkerTop();
	dst->allocate(sizeof(int), TOP);
	Marker mTop = dst->getMarkerTop();
	dst->deallocate(mTop, sizeof(int), TOP);
	Marker mTop2 = dst->getMarkerTop();
	cout << original << " " << mTop << " " << mTop2 << endl;
}

void testDStackFullBottom() {
	DoubleEndedStack * dst = new DoubleEndedStack(3);
	dst->allocate(sizeof(char), TOP);
	dst->allocate(sizeof(char), BOTTOM);
	dst->allocate(sizeof(char), BOTTOM);
	try {
		dst->allocate(sizeof(char), BOTTOM);
	}
	catch (const char* e) {
		cout << e << endl;
	}
}

void testDStackInvalidMarker() {
	DoubleEndedStack * dst = new DoubleEndedStack(3);
	void * address1 = dst->allocate(sizeof(char), BOTTOM);
	void * address2 = dst->allocate(sizeof(char), BOTTOM);
	dst->deallocate(reinterpret_cast<Marker>(address2), BOTTOM);
	try {
		dst->deallocate(reinterpret_cast<Marker>(address2), BOTTOM);
	}
	catch (const char * e) {
		cout << e << endl;
	}

	try {
		dst->deallocate(reinterpret_cast<Marker>(address1), TOP);
	}
	catch (const char * e) {
		cout << e << endl;
	}
}

void testDStackConstructionError() {
	try {
		DoubleEndedStack * dst = new DoubleEndedStack(0);
	}
	catch (const char * e) {
		cout << e << endl;
	}
}
//------------------------------------------------

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

void poolOutOfMemoryTest() {
	Pool * p = new Pool(sizeof(int), 3);
	p->allocate(sizeof(int));
	p->allocate(sizeof(int));
	p->allocate(sizeof(int));
	try {
		p->allocate(sizeof(int));
	}
	catch (const char * e) {
		cout << e << endl;
	}

}

void poolInvalidMarkerTest() {
	Pool * p = new Pool(sizeof(int), 3);
	void * address1 = p->allocate(sizeof(int));
	void * address2 = p->allocate(sizeof(int));
	void * address3 = p->allocate(sizeof(int));
	p->deallocate(reinterpret_cast<Marker>(address1));
	try {
		p->deallocate(reinterpret_cast<Marker>(address1));
	}
	catch(const char * e){
		cout << e << endl;
	}
}

void testPoolSmallerThanMarker() {
	Pool * pool = new Pool(sizeof(unsigned int), 3);
	int * rc1 = (int*)pool->allocate(sizeof(int));
	int * rc2 = (int*)pool->allocate(sizeof(int));
}

void testPoolSizeLargerThanBlock() {
	Pool * p = new Pool(sizeof(char), 10);
	try {
		p->allocate(sizeof(int));
	}
	catch (const char * e) {
		cout << e << endl;
	}
}

void poolOutOfBoundsTest() {
	Pool * p = new Pool(sizeof(int), 10);
	
}           

void poolConstructionTest() {
	try {
		Pool * p = new Pool(sizeof(char), 10000000000000000000);
	}
	catch (const char * e) {
		cout << e << endl;
	}
}

void poolAddressNotOnBlockBoundaryTest() {
	//TODO
	//the pool allocator must only accept markers that are a multiple of the size of the block
}

//-------------------------------------------------
void smartPointerTest() {
	int * ptr = new int;
	int * rc = new int;
	SmartPointer<int> * pointer = new SmartPointer<int>(ptr, rc, DEFAULT);
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

void smartPointerDeleteTest() {
	MemoryManager::getInstance().init(new Stack(100), 100);
	SmartPointer<int> p = MemoryManager::getInstance().smartAllocate<SmartPointer, int>(sizeof(int));
}

void smartPointerDStackTopDeallocate(){
	MemoryManager::getInstance().init(new DoubleEndedStack(100), 100);
	{
		SmartPointer<int> p = MemoryManager::getInstance().smartAllocate<SmartPointer, int>(sizeof(int), TOP);
		cout << p << endl;
	}
	SmartPointer<int> e = MemoryManager::getInstance().smartAllocate<SmartPointer, int>(sizeof(int), TOP);
	cout << e << endl;
}

//-------------------------------------------------
void memoryManagerTest() {
	MemoryManager::getInstance().init(new Stack(100), 100);
	Vertex * vert = (Vertex*)MemoryManager::getInstance().allocate(sizeof(Vertex));
	vert->x = 100;
	vert->y = 200;
	vert->z = 300;

	cout << vert->x << " " << vert->y << " " << vert->z << endl;
	
	MemoryManager::getInstance().deallocate(reinterpret_cast<Marker>(vert));

	Vertex* vert2 = (Vertex*)MemoryManager::getInstance().allocate(sizeof(Vertex));
	
	vert2->x = 200;
	vert2->y = 300;
	vert2->z = 400;

	cout << vert->x << " " << vert->y << " " << vert->z << endl;

	cout << vert2->x << " " << vert2->y << " " << vert2->z << endl;
}

void memoryManagerSmartAllocateTest() {
	MemoryManager::getInstance().init(new Stack(100), 100);
	SmartPointer<Vertex> s = MemoryManager::getInstance().smartAllocate<SmartPointer, Vertex>(sizeof(Vertex)); //need to make a better method for this, very clunky to keep calling over and over to get the instance
	(*s).x = 1.0;
	(*s).y = 2.0;
	(*s).z = 3.0;
	cout << (*s).x << (*s).y << (*s).z << endl;

	cout << sizeof(SmartPointer<Vertex>) << " " << sizeof(SmartPointer<int>) << endl;

	SmartPointer<Vertex> t = MemoryManager::getInstance().smartAllocate<SmartPointer, Vertex>(sizeof(Vertex));
	(*t).x = 6.0;
	(*t).y = 6.0;
	(*t).z = 6.0;
	cout << (*t).x << (*t).y << (*t).z << endl;

}

void memoryManagerSmartPointerRcOutOfSpaceTest() {
	MemoryManager::getInstance().init(new Stack(100), 100);
	SmartPointer<char> a = MemoryManager::getInstance().smartAllocate<SmartPointer, char>(sizeof(char));
	try {
		SmartPointer<char> b = MemoryManager::getInstance().smartAllocate<SmartPointer, char>(sizeof(char));
	}
	catch(const char * e){
		cout << e << endl;
	}
}

void smartPointerDoubleFreeTest() {
	MemoryManager::getInstance().init(new Pool(sizeof(int), 100), 100);
	SmartPointer<int> p = MemoryManager::getInstance().smartAllocate<SmartPointer, int>(sizeof(int));
	//TODO
}

//provide new global new and delete functions to work with memory manager

int main()
{
	//testStack();

	//testStackFull();

	//testDoubleEndedStack();
	
	//testDStackTopDeallocate();

	//testPool();

	//testPoolInt();

	//poolInvalidMarkerTest();

	//smartPointerTest();

	//smartPointerDeleteTest();
	
	//smartPointerDoubleFreeTest();

	smartPointerDStackTopDeallocate();

	//memoryManagerSmartAllocateTest();

	//memoryAllocatorTest();

	return 0;
}
