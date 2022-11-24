#include <iostream>
#include "SequentialFile.h"
#include "SequentialFileSorter.h"

#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */

// Disk page size in bytes
const int DISK_PAGE_SIZE = 4096;

const int DIFFERENT_ELEMENTS_POSSIBLE = (1 << 5) -1;
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


int main(int argc, char* argv[])
{
    // Initialize random seed
    srand(time(NULL));
    
    int debugging = 0;



    SequentialFile seqFile("file.dat", DISK_PAGE_SIZE);

    seqFile.resetToWrite();
    writeRandomRecords(seqFile, 9);
    seqFile.endWrite();
    seqFile.resetCounters();
    if (debugging) {
        std::cout << "Initial file: \n";
        seqFile.debugPrint(std::cout);
        std::cout << "\n";
    }
    SequentialFile t1("tape1.dat", DISK_PAGE_SIZE);
    SequentialFile t2("tape2.dat", DISK_PAGE_SIZE);

    SequentialFileSorter sorter(&seqFile, &t1, &t2);
    
    sorter.sortFile(debugging);
    if (debugging) {
        std::cout << "Sorted: \n";
        seqFile.debugPrint(std::cout);
        std::cout << "\n";
    }


    std::cout << "Number of phases: " << sorter.getPhases() << '\n';
    std::cout << "All disk page reads: " << seqFile.getCounterReads() + t1.getCounterReads() + t2.getCounterReads() << '\n';
    std::cout << "All disk page writes: " << seqFile.getCounterWrites() + t1.getCounterWrites() + t2.getCounterWrites() << '\n';

    return 0;
}


