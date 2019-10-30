// MemoryAllocator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Stack.h"
#include "DoubleEndedStack.h"
#include "Pool.h"
#include <iostream>

using namespace std;

struct Vertex {
	float x;
	float y;
	float z;
};

int main()
{
	//stack tests
	Stack st(100);
	int * p = (int *)st.alloc(sizeof(int));
	*p = 1;
	Marker beforeVert = st.getMarker();
	Vertex * vert = (Vertex *)st.alloc(sizeof(Vertex));
	*vert = Vertex{ 1.0, 2.0, 3.0 };
	Marker afterVert = st.getMarker();
	cout << *p << endl;
	cout << vert->x << " " << vert->y << " " << vert->z << endl;
	st.freeToMarker(beforeVert);
	Vertex * newVert = (Vertex*)st.alloc(sizeof(Vertex));
	*newVert = Vertex{ 1.0, 1.0, 1.0 };
	cout << "after deallocation and reallocation" << endl;
	cout << "vert: " << vert->x << " " << vert->y << " " << vert->z << " newVert: " << newVert->x << " " << newVert->y << " " << newVert->z << endl;

	//double ended stack test
	DoubleEndedStack dst(100);
	
	int * inta = (int*)dst.allocBottom(sizeof(int));
	*inta = 12;
	Marker intaM = dst.getMarkerBottom();
	int * intb = (int*)dst.allocTop(sizeof(int));
	Marker intbM = dst.getMarkerTop();
	Vertex * verta = (Vertex*)dst.allocBottom(sizeof(Vertex));
	*verta = Vertex{ 1.0,1.0,1.0 };
	Marker vertaM = dst.getMarkerBottom();
	Vertex * vertb = (Vertex*)dst.allocTop(sizeof(Vertex));
	*vertb = Vertex{ 2.0,2.0,2.0 };
	Marker vertbM = dst.getMarkerTop();
	
	cout << "vertex a: " << verta->x << " " << verta->y << " " << verta->z << endl;
	cout << "vertex b: " << vertb->x << " " << vertb->y << " " << vertb->z << endl;

	dst.freeToMarkerBottom(intaM);
	dst.freeToMarkerTop(intbM);
	
	Vertex * vertc = (Vertex*)dst.allocBottom(sizeof(Vertex));
	*vertc = Vertex{ 5.0,15.0,15.0 };
	Marker vertcM = dst.getMarkerBottom();
	Vertex * vertd = (Vertex*)dst.allocTop(sizeof(Vertex));
	*vertd = Vertex{ 23.0,23.0,23.0 };
	Marker vertdM = dst.getMarkerTop();

	cout << "vertex a: " << verta->x << " " << verta->y << " " << verta->z << endl;
	cout << "vertex b: " << vertb->x << " " << vertb->y << " " << vertb->z << endl;

	cout << "vertex c: " << vertc->x << " " << vertc->y << " " << vertc->z << endl;
	cout << "vertex d: " << vertd->x << " " << vertd->y << " " << vertd->z << endl;

	dst.clear();

	//pool allocator test

	Pool pool(sizeof(Vertex), 3);

	for (int i = 0; i < 3; i++) {
		Vertex* vert = (Vertex*)pool.alloc();
		vert->x = i; vert->y = i; vert->z = i;
	}
	Vertex * vertTest;
	for (int j = 0; j < 3; j++) {
		vertTest = (Vertex *)pool[j];
		cout << vertTest->x << " " << vertTest->y << " " << vertTest->z << endl;
	}
	
	pool.dealloc(0);

	Vertex * newVertTest = (Vertex*)pool.alloc();
	*newVertTest = Vertex{ 5.0, 5.0, 5.0 };

	Vertex * checkNewVert = (Vertex*)pool[0];
	cout << "after deallocation and re allocation: " << checkNewVert->x << " " << checkNewVert->y << " " << checkNewVert->z << endl;

	pool.dealloc(2);

	Vertex * newVert2 = (Vertex*)pool.alloc();
	*newVert2 = Vertex{ 5.0, 5.0, 5.0 };

	Vertex * checkNewVert2 = (Vertex*)pool[2];
	cout << "after deallocation and re allocation: " << checkNewVert2->x << " " << checkNewVert2->y << " " << checkNewVert2->z << endl;

	//deallocate everything
	for (int i = 0; i < 3; i++) {
		pool.dealloc(i);
	}

	//allocate everything again
	for (int i = 0; i < 3; i++) {
		Vertex * x = (Vertex *)pool.alloc();
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

	//-----memory allocator container tests-----



	return 0;
}
