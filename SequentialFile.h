#ifndef SEQUENTIAL_FILE_H
#define SEQUENTIAL_FILE_H

#include <string>
#include <fstream>

#include "Record.h"

class SequentialFile
{
private:
	// Path to file
	const std::string filePath;
	// File stream
	std::fstream fileStream;

	// Modes that file can be in
	enum FileMode { writing, reading };

	// Mode that file is in
	FileMode mode;

	// DiskPage size in bytes
	const int diskPageSize;
	// Number of records per disk page
	const int recordsPerPage;

	// DiskPage buffer for reading and writing
	char* const diskPageBuffer;

	// Set mode of a file
	void setMode(FileMode mode);

	// Current record to read/write
	int currRecord;

	// Number of records in a file
	int numRecords;

	// True if there were no writes after flushPageBuffer call
	bool justFlushed;

	// Write record on a proper offset on diskPageBuffer
	void writeRecord(Record* record, int recordOnPage);
	
	// Write diskPageBuffer to a file
	void flushPageBuffer();

	// Read to diskPageBuffer from a file
	void loadPageBuffer();

	// Read record from a proper offset in diskPageBuffer
	void readRecord(Record* record, int recordOnPage);

public:
	// Constructor
	SequentialFile(const std::string& filePath, int diskPageSize);

	// Destructor
	~SequentialFile();

	// Next record is written or read depending on file's mode
	int nextRecord(Record* record);

	// Must be called after ending a sequence of writes so a not fully used page will be written
	void endWrite();

	// Next record will write to begginging of a file
	void resetToWrite();

	// Write out all file content without incrementing accesses counters
	void debugPrint(std::ostream& os);
};

#endif