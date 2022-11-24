#include "SequentialFileSorter.h"

#include <iostream>

int SequentialFileSorter::distribiute()
{
	// One record to read to
	Record record;
	// True if there is more than one run in t3
	int not_sorted=0;

	// If there are no records
	if (t3->nextRecord(&record)) {
		// Return that t3 is sorted
		return 1;
	}
	// Track key of prev record
	int32_t prev_key = record.key;

	// Tape to write records to
	Tape* curr_tape = t1;

	// Write first record to t1
	curr_tape->nextRecord(&record);

	while (!t3->nextRecord(&record)) {
		if (prev_key > record.key) {
			// Swap tapes
			curr_tape = (curr_tape == t1 ? t2 : t1);
			// Indicate that there is more than one run on t3
			not_sorted = 1;
		}
		prev_key = record.key;
		curr_tape->nextRecord(&record);
	}
	t1->endWrite();
	t2->endWrite();

	// Return 
	return !not_sorted;
}

void SequentialFileSorter::merge()
{

}

SequentialFileSorter::SequentialFileSorter(Tape* file, Tape* t1, Tape* t2)
	:
	t1(t1),
	t2(t2),
	t3(file)
{
}

SequentialFileSorter::~SequentialFileSorter()
{
}

void SequentialFileSorter::sortFile()
{
	int end = distribiute();
	std::cout << "t1: \n";
	t1->debugPrint(std::cout);
	std::cout << "t2: \n";
	t2->debugPrint(std::cout);
	std::cout << "t3: \n";
	t3->debugPrint(std::cout);
	while(!end){
		merge();
		end = distribiute();
		t1->debugPrint(std::cout);
		t2->debugPrint(std::cout);
		t3->debugPrint(std::cout);
	}
	

}
