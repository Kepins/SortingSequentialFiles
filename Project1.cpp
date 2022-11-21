#include <iostream>
#include "SequentialFile.h"

#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */

// Disk page size in bytes
const int DISK_PAGE_SIZE = 1024;

const int DIFFERENT_ELEMENTS_POSSIBLE = (1 << 10) -1;
void randomFillRecord(Record* record) {
    for (int i = 0; i < record->numElements; i++) {
        record->elements[i] = rand() % DIFFERENT_ELEMENTS_POSSIBLE - (DIFFERENT_ELEMENTS_POSSIBLE >> 1);
    }
}

void writeRandomRecords(SequentialFile& seqFile, int n) {
    Record record;
    for (int i = 0; i < n; i++) {
        randomFillRecord(&record);
        seqFile.nextRecord(&record);
    }
}

int main()
{
    // Initialize random seed
    srand(time(NULL));

    SequentialFile seqFile("file.dat", DISK_PAGE_SIZE);

    seqFile.resetToWrite();
    writeRandomRecords(seqFile, 2500000);
    seqFile.endWrite();
    // seqFile.debugPrint(std::cout);
    Record readRec;
    while (!seqFile.nextRecord(&readRec))
    {
        
    }
    

    return 0;
}


