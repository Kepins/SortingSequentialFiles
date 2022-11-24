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
	

	// Distribiute records from t3 to t1, t2
	int distribiute();
	// Merge runs from t1, t2 to t3
	void merge();
public:
	SequentialFileSorter(Tape* file, Tape* t1, Tape* t2);
	~SequentialFileSorter();

	void sortFile();

};

#endif