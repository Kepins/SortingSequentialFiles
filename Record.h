#ifndef RECORD_H
#define RECORD_H

#include <cstdint>

// Number of int32_t to store in one record
const int NUM_ELEMENTS = 10;

class Record
{
public:
	// Array to hold elements of record
	int32_t elements[NUM_ELEMENTS];

	// Number of elements
	static const int numElements = NUM_ELEMENTS;
	// Size of record in bytes
	static const int size = sizeof(elements);
};

#endif