#include "SequentialFile.h"

SequentialFile::SequentialFile(const std::string& filePath, int diskPageSize)
	: 
	// Initialize filePath
	filePath(filePath),
	// Initialize disk page size
	diskPageSize(diskPageSize),
	// Initialize number of records per disk page
	recordsPerPage(diskPageSize / Record::size),
	// Create a disk page buffer for reading/writing
	diskPageBuffer(new char[diskPageSize])
{
	// Create empty file
	std::fstream temp = std::fstream(filePath, std::ios::out);
	temp.close();

	// Create a file stream for reading and writing
	fileStream = std::fstream(filePath, std::ios::binary | std::ios::out | std::ios::in);

	// Set mode to writing
	setMode(writing);
}

SequentialFile::~SequentialFile() 
{
	// Close file stream
	fileStream.close();

	// Free buffer memory
	delete[] diskPageBuffer;
}

void SequentialFile::setMode(FileMode mode) {
	// If new mode is writing
	if (mode == writing) {
		// Set number of records to 0 because we will be filling this file
		numRecords = 0;
		// Set a flush flag
		justFlushed = true;
	}
	// Else - new mode is reading
	else {
	}
	// Set mode to the one passed by parameter
	this->mode = mode;
	// Set current record number to read/write to beggining
	currRecord = 0;
	// Reset file stream
	fileStream.seekp(0);
}

void SequentialFile::writeRecord(Record* record, int recordOnPage)
{
	// Copy record's elements to a diskPageBuffer
	memcpy(
		diskPageBuffer + recordOnPage * Record::size,
		&record->key,
		Record::size
	);
	// Reset a flush flag
	justFlushed = false;
}

void SequentialFile::flushPageBuffer()
{
	// Write the page out
	fileStream.write(diskPageBuffer, diskPageSize);
	// Set a flag that there was a flush
	justFlushed = true;
}


void SequentialFile::loadPageBuffer() {
	// Read one page to diskPageBuffer
	fileStream.read(diskPageBuffer, diskPageSize);
}

void SequentialFile::readRecord(Record* record, int recordOnPage)
{
	// Copy diskPageBuffer's data to record
	memcpy(
		&record->key,
		diskPageBuffer + recordOnPage * Record::size,
		Record::size
	);
}

int SequentialFile::nextRecord(Record* record)
{
	// Which record on page it is
	int recordOnPage = currRecord % recordsPerPage;

	// If file is in writing mode
	if (mode == writing) {
		// Write record to disk page buffer
		writeRecord(record, recordOnPage);
		// Increment number of records
		numRecords++;
		// If page buffer is full
		if (recordOnPage == (recordsPerPage - 1)) {
			// Write page to memory
			flushPageBuffer();
		}
	}
	// Else - file is in reading mode
	else {
		// If there are no records to read
		if (currRecord >= numRecords) {
			// Return failure status
			return 1;
		}
		// If the page should be changed
		if (recordOnPage == 0) {
			// Load new page in the buffer
			loadPageBuffer();
		}
		// Read a record
		readRecord(record, recordOnPage);
	}

	// Increment current record to read/write position
	currRecord++;

	return 0;
}

void SequentialFile::endWrite()
{
	// If mode is writing and diskPageBuffer is not saved
	if (mode==writing && !justFlushed) {
		flushPageBuffer();
	}
	setMode(reading);
}

void SequentialFile::resetToWrite() {
	// Set mode to writing
	setMode(writing);
}

void SequentialFile::debugPrint(std::ostream& os) {
	std::streampos pre = fileStream.tellp();
	
	Record record;
	for (int i = 0; i < numRecords; i++) {
		int pageNumber = i / recordsPerPage;
		int recordOnPage = i % recordsPerPage;
		fileStream.seekp(pageNumber * diskPageSize + recordOnPage * Record::size);
		fileStream.read((char*)(&record.key), Record::size);
		os << i << "(key: " << record.key << "): ";
		for (int i = 0; i < record.numElements; i++) {
			os << record.elements[i] << ' ';
		}
		os << '\n';
	}
	fileStream.seekp(pre);
}
