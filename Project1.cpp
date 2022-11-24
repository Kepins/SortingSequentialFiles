#include <iostream>
#include "SequentialFile.h"
#include "SequentialFileSorter.h"

#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */

// Disk page size in bytes
const int DISK_PAGE_SIZE = 512;

const int DIFFERENT_ELEMENTS_POSSIBLE = (1 << 4) -1;
void randomFillRecord(Record* record) {
    for (int i = 0; i < record->numElements; i++) {
        record->elements[i] = rand() % DIFFERENT_ELEMENTS_POSSIBLE; //- (DIFFERENT_ELEMENTS_POSSIBLE >> 1);
    }
    record->calculateKey();
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
    writeRandomRecords(seqFile, 250);
    seqFile.endWrite();
    SequentialFile t1("tape1.dat", DISK_PAGE_SIZE);
    SequentialFile t2("tape2.dat", DISK_PAGE_SIZE);

    SequentialFileSorter sorter(&seqFile, &t1, &t2);
    
    sorter.sortFile();

    return 0;
}


