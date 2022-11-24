#ifndef SEQUENTIAL_FILE_SORTER_H
#define SEQUENTIAL_FILE_SORTER_H

#include "SequentialFile.h"

class SequentialFileSorter
{
private:
	// Tape is an allias for SequentialFile
	typedef SequentialFile Tape;

	/*
		t1, t2  - tape for distribution
		t3 - tape on which merge happens
	*/
	Tape *t1, *t2, *t3;
	
	// Number of phases to sort a file
	int phases = 0;

	// Distribiute records from t3 to t1, t2
	// Returns 0 if sorting has not ended yet
	int distribiute();
	// Merge runs from t1, t2 to t3
	void merge();
public:
	// Object that sorts file using t1, t2
	SequentialFileSorter(Tape* file, Tape* t1, Tape* t2);

	// Destructor
	~SequentialFileSorter();

	// Sort file passed as 'file' in constructor using tapes 't1', 't2'
	void sortFile();

	// Returns number of phases to sort a file
	int getPhases() { return phases; }
};

#endif