// Programmer: Max Goshay
// Programmer's ID: 252053
#define _CRT_SECURE_NO_WARNINGS
#ifndef DynamicArray_h 
#define DynamicArray_h
#include <iostream>
#include <string>
#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <fstream>
using namespace std;

#ifndef NO_MEM_DEBUG
#include <chrono>

static size_t g_num_allocations = 0;
static size_t g_num_frees = 0;

static size_t g_allocated_mem = 0;
static size_t g_freed_mem = 0;

void* operator new (size_t size) throw(std::bad_alloc) {
	g_allocated_mem += size;
	++g_num_allocations;
	size_t* mem = (size_t*)std::malloc(size + sizeof(size_t));
	if (!mem) {
		throw std::bad_alloc();
	}
	mem[0] = size;
	return (void*)(&mem[1]);
}
void operator delete (void* mem) throw() {
	auto ptr = &((size_t*)mem)[-1];
	g_freed_mem += ptr[0];
	++g_num_frees;
	std::free(ptr);
}
struct PerfLogger {
	std::chrono::high_resolution_clock::time_point t0;

	PerfLogger() : t0(std::chrono::high_resolution_clock::now()) {}
	~PerfLogger() {
		using namespace std::chrono;
		auto t1 = high_resolution_clock::now();
		std::cout << "\nUsed  memory: " << ((double)g_allocated_mem) * 1e-6 << " MB (" << g_num_allocations << " allocations)\n";
		std::cout << "Freed memory: " << ((double)g_freed_mem) * 1e-6 << " MB (" << g_num_frees << " deallocations)\n";
		std::cout << "Ran in " << duration_cast<duration<double>>(t1 - t0).count() * 1e3 << " ms\n";
	}
} g_perf_logger;

#endif // NO_MEM_DEBUG



template <typename V>
class DynamicArray
{
	int cap;
	V dummy;
public:
	V* values;
	DynamicArray(int = 2); // default constructor
	DynamicArray(const DynamicArray<V>&); // copy c
	~DynamicArray() { delete[] values; } //destructor
	DynamicArray<V>& operator=(const DynamicArray<V>&);
	int capacity() const { return cap; }
	void capacity(int); // setter cap
	V operator [] (int) const; //getter
	V& operator [] (int); //setter
};
//constructor
template <typename V>
DynamicArray<V>::DynamicArray(int cap)
{
	this->cap = cap;
	values = new V[cap];
	for (int i = 0; i < cap; i++) {
		values[i] = V();
	}
}
//copy constructor
template <typename V>
DynamicArray<V>::DynamicArray(const DynamicArray<V> & original) {
	cap = original.cap;
	values = new V[cap][1];
	dummy = V();
	for (int i = 0; i < cap; i++)
		values[i] = original.values[i];
	dummy = original.dummy;
}
//operator =
template <typename V>
DynamicArray<V>& DynamicArray<V>::operator=(const DynamicArray<V>&
	original) {
	if (this != &original)
	{
		delete[] values;
		cap = original.cap;
		values = new V[cap];
		for (int i = 0; i < cap; i++)
			values[i] = original.values[i];
		dummy = original.dummy;
	}
	return *this;
}
//set capacity
template <typename V>
void DynamicArray<V>::capacity(int cap) {
	V* temp = new V[cap];
	int limit = min(cap, this->cap);
	for (int i = 0; i < limit; i++)
		temp[i] = values[i];
	for (int i = limit; i < cap; i++)
		temp[i] = V();
	delete[] values;
	values = temp;
	this->cap = cap;
}
//getter []
template <typename V>
V DynamicArray<V>::operator[ ](int index) const
{
	if (index < 0) return dummy;
	if (index >= cap) return dummy;
	return values[index];
}
//setter []
template <typename V>
V& DynamicArray<V>::operator[ ](int index)
{
	if (index < 0) return dummy;
	if (index >= cap) capacity(index * 2);
	return values[index];
}
#endif