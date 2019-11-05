/**
	Game Engines and Workflow CW1
	Memory Manager
	Cand No.: 201332107
*/
#include "pch.h"
#include "MemoryManager.h"
#include <iostream>
#include "MemoryAllocator.h"

using namespace std;

//-------------------------------------------------------
/*
	global function to allocate raw memory
	@param size size of memory wanting to be allocated
	@param options options to be used with the specific allocation strategy chosen to be used with the memory manager
	@return void * address of allocated memory
	@throws if the memory manager is not properly initialised or out of memory
*/
void * memAllocRaw(size_t size, AllocOptions options = DEFAULT) { return MemoryManager::getInstance().allocate(size, options); }

/*
	global function to deallocate memory
	@param address the location in memory to deallocate
	@param size the amount of memory to deallocate
	@options options to be used with the specific allocation strategy chosen to be used with the memory manager
	@throws if the memory manager is not properly initialised or the address fails boundary checks or is invalid
*/
void memFreeRaw(void * address, size_t size, AllocOptions options = DEFAULT) {
	Marker m = reinterpret_cast<Marker>(address);
	MemoryManager::getInstance().deallocate(m, size, options);
}

/*
	global function to allocate typed memory
	@param options options to be used with the specific allocation strategy chosen to be used with the memory manager
	@return SmartPointer<T> the smartpointer of the provided type to allocated memory address
	@throws if the memory manager is not properly initialised or out of memory
*/
template<template<class> class SmartPointer,class T>
inline SmartPointer<T> memAllocSmart(AllocOptions options = DEFAULT) {
	return MemoryManager::getInstance().smartAllocate<SmartPointer, T>(options);
}
//-------------------------------------------------------


struct Vertex {
	float x;
	float y;
	float z;
};

//-------------------------------------------------
/*
	Stack integration test
*/
void testStack() {
	Stack st(100);
	int * p = (int *)st.allocate(sizeof(int));
	*p = 1;
	Marker beforeVert = st.getTop();
	Vertex * vert = (Vertex *)st.allocate(sizeof(Vertex));
	*vert = Vertex{ 1.0, 2.0, 3.0 };
	Marker afterVert = st.getTop();
	cout << *p << endl;
	cout << vert->x << " " << vert->y << " " << vert->z << endl;
	st.deallocate(beforeVert);
	Vertex * newVert = (Vertex*)st.allocate(sizeof(Vertex));
	*newVert = Vertex{ 1.0, 1.0, 1.0 };
	cout << "after deallocation and reallocation" << endl;
	cout << "vert: " << vert->x << " " << vert->y << " " << vert->z << " newVert: " << newVert->x << " " << newVert->y << " " << newVert->z << endl;
}

/*
	Stack out of memory test
*/
void testStackFull() {
	Stack st(3);
	char * char1 = (char*)st.allocate(sizeof(char));
	char * char2 = (char*)st.allocate(sizeof(char));
	char * char3 = (char*)st.allocate(sizeof(char));
	*char1 = 'a';
	*char2 = 'b';
	*char3 = 'c';
	//should be full at this point
	try {
		char * char4 = (char*)st.allocate(sizeof(char));
	}
	catch(const char * e){
		cout << e << endl;
	}
}

/*
	Stack bad construction test
*/
void testBadStackConstruction() {
	try {
		Stack * st = new Stack(0);
	}
	catch (const char * e) {
		cout << e << endl;
	}
}

/*
	Stack construction test
*/
void testStackConstruction() {
	Stack * st = new Stack(100);
}

//------------------------------------------------
/*
	double ended stack integration test
*/
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
	dst.deallocate(intbM, sizeof(int), AllocOptions::TOP);

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

/*
	double ended stack top out of memory test
*/
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

/*
	double ended stack top deallocate test
*/
void testDStackTopDeallocate() {
	DoubleEndedStack * dst = new DoubleEndedStack(100);
	Marker original = dst->getMarkerTop();
	dst->allocate(sizeof(int), TOP);
	Marker mTop = dst->getMarkerTop();
	dst->deallocate(mTop, sizeof(int), TOP);
	Marker mTop2 = dst->getMarkerTop();
	cout << original << " " << mTop << " " << mTop2 << endl;
}


/*
	double ended stack bottom out of memory
*/
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

/*
	double ended stack invalid marker boundry checks test
*/
void testDStackInvalidMarker() {
	DoubleEndedStack * dst = new DoubleEndedStack(3);
	void * address1 = dst->allocate(sizeof(char), BOTTOM);
	void * address2 = dst->allocate(sizeof(char), TOP);
	dst->deallocate(reinterpret_cast<Marker>(address1), BOTTOM);
	dst->deallocate(reinterpret_cast<Marker>(address2), sizeof(char), TOP);
	try {
		dst->deallocate(reinterpret_cast<Marker>(address2), BOTTOM);
	}
	catch (const char * e) {
		cout << e << endl;
	}

	try {
		dst->deallocate(reinterpret_cast<Marker>(address1), sizeof(char), TOP);
	}
	catch (const char * e) {
		cout << e << endl;
	}
}

/*
	double ended stack construction error test
*/
void testDStackConstructionError() {
	try {
		DoubleEndedStack * dst = new DoubleEndedStack(0);
	}
	catch (const char * e) {
		cout << e << endl;
	}
}

void testDoubleEndedStackMeetInTheMiddle() {
	DoubleEndedStack dst(sizeof(Vertex)*2);
	Vertex * v1 = (Vertex*)dst.allocate(sizeof(Vertex), BOTTOM);
	Vertex * v2 = (Vertex*)dst.allocate(sizeof(Vertex), TOP);
	v2->x = 5.0;
	v2->y = 5.0;
	v2->z = 5.0;

	v1->x = 6.0;
	v1->y = 6.0;
	v1->z = 6.0;

	cout << v1->x << " " << v1->y << " " << v1->z << endl;
	cout << v2->x << " " << v2->y << " " << v2->z << endl;

	cout << v1 << endl;
	cout << v2 << endl;
}

//------------------------------------------------

/*
	pool integration test
*/
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

/*
	pool out of memory test
*/
void testPoolOutOfMemory() {
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

/*
	pool invalid marker boundary check test
*/
void testPoolInvalidMarker() {
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

/*
	pool block size smaller than Marker test
*/
void testPoolSmallerThanMarker() {
	Pool * pool = new Pool(sizeof(unsigned int), 3);
	int * rc1 = (int*)pool->allocate(sizeof(int));
	int * rc2 = (int*)pool->allocate(sizeof(int));
}

/*
	pool allocation request larger than block test
*/
void testPoolSizeLargerThanBlock() {
	Pool * p = new Pool(sizeof(char), 10);
	try {
		p->allocate(sizeof(Vertex));
	}
	catch (const char * e) {
		cout << e << endl;
	}
}

/*
	pool boundary check test
*/
void testPoolOutOfBounds() {
	Pool * p = new Pool(sizeof(int), 10);
	try {
		p->deallocate(0, 10);
	}
	catch (const char * e) {
		cout << e << endl;
	}
}           

/*
	pool consturction test
*/
void testPoolConstruction() {
	try {
		Pool * p = new Pool(sizeof(char), 10000000000000000000); //unable to make this throw an error but it should work
	}
	catch (const char * e) {
		cout << e << endl;
	}
}

/*
	pool address is inbetween blocks test
*/
void testPoolAddressNotOnBlockBoundary() {
	Pool * p = new Pool(sizeof(Vertex), 100);
	char * verttoinc = (char*)p->allocate(sizeof(Vertex));
	verttoinc++;
	try {
		p->deallocate(reinterpret_cast<Marker>(verttoinc));
	}
	catch (const char * e) {
		cout << e << endl;
	}
	verttoinc--;
	p->deallocate(reinterpret_cast<Marker>(verttoinc));
}

//-------------------------------------------------

/*
	smart pointer integration test
*/
void testSmartPointer() {
	MemoryManager::getInstance().init(new Stack(100), 100);
	SmartPointer<int> pointer = MemoryManager::getInstance().smartAllocate<SmartPointer, int>();
	SmartPointer<int>pointer2 = pointer;
	cout << pointer.getCounter() << endl;
}

/*
	smart pointer access data test
*/
void testSmartPointerAccess() {
	MemoryManager::getInstance().init(new Pool(sizeof(Vertex), 100), 100);
	SmartPointer<Vertex> pointer = MemoryManager::getInstance().smartAllocate<SmartPointer, Vertex>();
	SmartPointer<Vertex> pointer2 = pointer;
	*pointer2 = Vertex{2.0, 2.0, 2.0};
	cout << (*pointer2).x << (*pointer).y << (*pointer).z << endl;
}

/*
	smart pointer destruction test
*/
void testSmartPointerDelete() {
	MemoryManager::getInstance().init(new Stack(100), 100);
	{
		SmartPointer<int> p = MemoryManager::getInstance().smartAllocate<SmartPointer, int>();
	}
}

/*
	smart pointer double ended stack deallocation test
*/
void testSmartPointerDStackTopDeallocate(){
	MemoryManager::getInstance().init(new DoubleEndedStack(100), 100);
	{
		SmartPointer<int> p = MemoryManager::getInstance().smartAllocate<SmartPointer, int>(TOP);
		cout << p << endl;
	}
	SmartPointer<int> e = MemoryManager::getInstance().smartAllocate<SmartPointer, int>(TOP);
	cout << e << endl;
}

//-------------------------------------------------

/*
	memory manager stack integration test
*/
void testMemoryManagerStack() {
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

/*
	memory manager double ended stack integration test
*/
void testMemoryManagerDStack() {
	MemoryManager::getInstance().init(new DoubleEndedStack(100), 100);
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

/*
	memory manager pool integration test
*/
void testMemoryManagerPool() {
	MemoryManager::getInstance().init(new Pool(sizeof(Vertex), 100), 100);
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

/*
	memory manager smart allocation test
*/
void testMemoryManagerSmartAllocate() {
	MemoryManager::getInstance().init(new Stack(100), 100);
	SmartPointer<Vertex> s = MemoryManager::getInstance().smartAllocate<SmartPointer, Vertex>();
	(*s).x = 1.0;
	(*s).y = 2.0;
	(*s).z = 3.0;
	cout << (*s).x << (*s).y << (*s).z << endl;

	cout << sizeof(SmartPointer<Vertex>) << " " << sizeof(SmartPointer<int>) << endl;

	SmartPointer<Vertex> t = MemoryManager::getInstance().smartAllocate<SmartPointer, Vertex>();
	(*t).x = 6.0;
	(*t).y = 6.0;
	(*t).z = 6.0;
	cout << (*t).x << (*t).y << (*t).z << endl;

}

/*
	memory manager reference count and smart pointer out of memory test
*/
void testMemoryManagerSmartPointerRcOutOfSpace() {
	MemoryManager::getInstance().init(new Stack(100), 1);
	SmartPointer<char> a = MemoryManager::getInstance().smartAllocate<SmartPointer, char>();
	try {
		SmartPointer<char> b = MemoryManager::getInstance().smartAllocate<SmartPointer, char>();
	}
	catch(const char * e){
		cout << e << endl;
	}
}

/*
	memory manager out of memory test
*/
void testMemoryManagerOutOfMemory() {
	MemoryManager::getInstance().init(new Pool(sizeof(int), 2), 100);
	MemoryManager::getInstance().allocate(sizeof(int));
	MemoryManager::getInstance().allocate(sizeof(int));
	try {
		MemoryManager::getInstance().allocate(sizeof(int));
	}
	catch (const char * e) {
		cout << e << endl;
	}
		 
}

/*
	memory manager bad initialisation test
*/
void testMemoryManagerNoInit() {
	try {
		MemoryManager::getInstance().smartAllocate<SmartPointer, int>();
	}
	catch (const char * e) {
		cout << e << endl;
	}

}

void testGlobalAlloc() {
	MemoryManager::getInstance().init(new Stack(100), 100);
	Vertex * address = (Vertex*)memAllocRaw(sizeof(Vertex));
	*address = Vertex{ 1.0,1.0,1.0 };
}

void testGlobalFree() {
	MemoryManager::getInstance().init(new Stack(100), 100);
	Vertex * address = (Vertex*)memAllocRaw(sizeof(Vertex));
	*address = Vertex{ 1.0,1.0,1.0 };
	memFreeRaw(address, sizeof(Vertex));
}

void testGlobalSmartAlloc() {
	MemoryManager::getInstance().init(new Stack(100), 100);
	SmartPointer<Vertex> address = memAllocSmart<SmartPointer, Vertex>();
	*address = Vertex{ 1.0,1.0,1.0 };
}


int main()
{
	//testStack();

	//testStackFull();
	
	//testBadStackConstruction();

	//testStackConstruction();

	//--------------------------

	//testDoubleEndedStack();

	//testDStackFullTop();

	//testDStackFullBottom();

	//testDStackTopDeallocate();

	//testDStackInvalidMarker();

	//testDStackConstructionError();

	//testDoubleEndedStackMeetInTheMiddle();

	//-------------------------------

	//testPool();

	//testPoolConstruction();

	//testPoolInvalidMarker();

	//testPoolAddressNotOnBlockBoundary();

	//testPoolOutOfMemory();

	//testPoolSizeLargerThanBlock();

	//testPoolSmallerThanMarker();

	//---------------------------------

	//testSmartPointer();
	
	//testSmartPointerAccess();

	//testSmartPointerDelete();

	//testSmartPointerDStackTopDeallocate();

	//-----------------------------------

	//testMemoryManagerStack();

	//testMemoryManagerDStack();

	//testMemoryManagerPool();

	//testMemoryManagerSmartAllocate();

	//testMemoryManagerSmartPointerRcOutOfSpace();

	//testMemoryManagerNoInit();

	//testMemoryManagerOutOfMemory();

	//testGlobalAlloc();

	//testGlobalFree();

	//testGlobalSmartAlloc();

	return 0;
}
