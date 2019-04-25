// HeapWindows10.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "windows.h"
using namespace std;

/*

DECLSPEC_ALLOCATOR LPVOID HeapAlloc(
  HANDLE hHeap,
  DWORD  dwFlags,
  SIZE_T dwBytes
);

*/

class Heap{
private:
	LPVOID hChunk;
	HANDLE hDefaultHeap = GetProcessHeap();
public:

	virtual void printDefaultHeap();
	HANDLE createHeap(size_t size);
	void freeHeap(HANDLE ptr);
	void surroundChunk(int count, int first, int second);
	BOOL fillAllocation(HANDLE ptr, char c, size_t size);
	virtual void pause();
	void allocatedChunks(size_t count,size_t size);
};

void Heap::allocatedChunks(size_t count,size_t size) {
	for (size_t i = 0; i < count; i++) {
		//>>> 0x7ffb0
		//524208
		HANDLE chunk = Heap::createHeap(size);
		Heap::fillAllocation(chunk, 'A', size);
	}
}
void Heap::pause() {

	cout << "hit continue" << endl;
	cin.ignore();
}
BOOL Heap::fillAllocation(HANDLE ptr, char c, size_t size) {

	if (!ptr) {
		return -1;
	}
	memset(ptr, c, size);
	cout << "fill '" << c << " ' @ :0x" << ptr << endl;
}
void Heap::surroundChunk(int count, int first, int second) {
	  
	//it will create a different size subsequences
	//Create this method to manpulate the LFH 
	//precondition if first equal to second
	//postcondition allocate a second or allocate a first 
	int temp = first;
	for (int i = 0; i < count; i++) {
			if (first == second) {
				Heap::createHeap(second);
				first = temp;
			}
			else {
				Heap::createHeap(first);
				first = second;
			}
	}
};
void Heap::printDefaultHeap() {
	//will print the base address of heap
	cout << "Heap Default @: 0x" << hDefaultHeap << endl;
}
LPVOID Heap::createHeap(size_t size) {
	//-parm size => the chunk size
	//allocate a new chunk
	//return a heap pointer location 
	 hChunk = HeapAlloc(hDefaultHeap, 0, size);
	 cout << "allocate chunk of " << hex << "0x" << size << " bytes " << " ! the heap located @ :0x" << hChunk << endl;
	 return hChunk;
}
void Heap::freeHeap(HANDLE ptr) {
	//it will freed the chunk 
	HeapFree(hDefaultHeap, 0, ptr);
	cout << "heap freed ptr :0x" << ptr << endl;
}

void playWithHeap(Heap* heap) {
	//client part 
	heap->printDefaultHeap();
	heap->pause();
	heap->surroundChunk(20, 0x58, 0x100);
	cout << "now check the last 3 chunks to see if it is a part of LFH or not " << endl;
	heap->pause();
	//it's time to apply 
	HANDLE alloc0x100 = heap->createHeap(0x100);
	HANDLE alloc0x58 = heap->createHeap(0x58);
	heap->fillAllocation(alloc0x58, 'A', 0x58);
	HANDLE alloc0x100_2 = heap->createHeap(0x100);
	heap->pause();
	heap->freeHeap(alloc0x58);
	heap->pause();
	alloc0x58 = heap->createHeap(0x58);
	//the fill out will placed the same allocate that it fill it with A's before 
	heap->fillAllocation(alloc0x58, 'B', 0x58);
	heap->pause();
}
int main()
{
	Heap* heap = new Heap();
	playWithHeap(heap);
	return 0;
}

