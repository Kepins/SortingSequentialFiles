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
	// Start writing to the beggining of the t3
	t3->resetToWrite();
	// Two records to read to
	Record t1_record, t2_record;
	// t1 must contain at least one record
	t1->nextRecord(&t1_record);
	// t2 must contain at least one record
	t2->nextRecord(&t2_record);

	// Bool value to store return value from tx->nextRecord
	int empty;
	while (true) {
		if (t1_record.key < t2_record.key) {
			// Write record from t1 to t3
			t3->nextRecord(&t1_record);
			empty = t1->nextRecord(&t1_record);
			if (empty) {
				t3->nextRecord(&t2_record);
				// Write all other records from t2 to t3
				while (!t2->nextRecord(&t2_record)) {
					t3->nextRecord(&t2_record);
				}
				break;
			}
		}
		else {
			// Write record from t2 to t3
			t3->nextRecord(&t2_record);
			empty = t2->nextRecord(&t2_record);
			if (empty) {
				// Write existing record to t3
				t3->nextRecord(&t1_record);
				// Write all other records from t1 to t3
				while (!t1->nextRecord(&t1_record)) {
					t3->nextRecord(&t1_record);
				}
				break;
			}
		}
	}
	t1->resetToWrite();
	t2->resetToWrite();
	t3->endWrite();
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
	/*std::cout << "Distribiution\n\n";
	std::cout << "t1: \n";
	t1->debugPrint(std::cout);
	std::cout << "t2: \n";
	t2->debugPrint(std::cout);
	std::cout << "t3: \n";
	t3->debugPrint(std::cout);*/
	while(!end){
		merge();
		/*std::cout << "Merging\n\n";
		std::cout << "t1: \n";
		t1->debugPrint(std::cout);
		std::cout << "t2: \n";
		t2->debugPrint(std::cout);
		std::cout << "t3: \n\n";
		t3->debugPrint(std::cout);*/
		end = distribiute();
		/*std::cout << "Distribiution\n";
		std::cout << "t1: \n";
		t1->debugPrint(std::cout);
		std::cout << "t2: \n";
		t2->debugPrint(std::cout);
		std::cout << "t3: \n";
		t3->debugPrint(std::cout);*/
	}
	

}
