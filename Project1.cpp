#include <iostream>
#include "SequentialFile.h"
#include "SequentialFileSorter.h"

#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */

#include "DebugLvls.h"

// Disk page size in bytes
const int DISK_PAGE_SIZE = 4096;

const int DIFFERENT_ELEMENTS_POSSIBLE = (1 << 5) -1;
void randomFillRecord(Record* record) {
    for (int i = 0; i < record->numElements; i++) {
        record->elements[i] = rand() % DIFFERENT_ELEMENTS_POSSIBLE; //- (DIFFERENT_ELEMENTS_POSSIBLE >> 1);
    }
    record->calculateKey();
}

void manualInput(SequentialFile& seqFile, int n) {
    Record record;
    for (int i = 0; i < n; i++) {
        std::cout << i << ": ";
        for (int j = 0; j < record.numElements; j++) {
            std::cin >> record.elements[j];
        }
        record.calculateKey();
        seqFile.nextRecord(&record);
    }
}

void writeRandomRecords(SequentialFile& seqFile, int n) {
    Record record;
    for (int i = 0; i < n; i++) {
        randomFillRecord(&record);
        seqFile.nextRecord(&record);
    }
}

void fromTxtFile(SequentialFile& seqFile, std::ifstream& file) {
    Record record;
    int elem;
    int i=0;
    while (file >> elem) {
        record.elements[i++] = elem;
        if (i == record.numElements) {
            record.calculateKey();
            seqFile.nextRecord(&record);
            i = 0;
        }
    }
}

int main(int argc, char* argv[])
{
    // Initialize random seed
    srand(time(NULL));
    
    SequentialFile seqFile("file.dat", DISK_PAGE_SIZE);
    seqFile.resetToWrite();

    int input = 0;

    std::cout << "Debug info?\n"
        << "0. No\n"
        << "1. Normal\n"
        << "2. Verbose\n";
    std::cin >> input;
    DebugLvl debugLvl = static_cast<DebugLvl>(input);

    std::cout << "File to sort: \n"
        << "0. Manual input\n"
        << "1. Txt file\n"
        << "2. Random\n";
    int fileSource;
    std::cin >> fileSource;
    switch (fileSource) {
        case 0: {
            int n;
            std::cout << "Number of records: ";
            std::cin >> n;
            manualInput(seqFile, n);
        }break;
        case 1:{
            std::string txtFilePath;
            std::cout << "Txt filepath: ";
            std::cin >> txtFilePath;
            std::ifstream txtFile(txtFilePath, std::ios::in);
            if (!txtFile.is_open()) {
                std::cerr << "Txt file does not exist";
                return 1;
            }
            fromTxtFile(seqFile, txtFile);
        }break;
        case 2: {
            int n;
            std::cout << "Number of records: ";
            std::cin >> n;
            writeRandomRecords(seqFile, n);
        }break;
    }

    seqFile.endWrite();
    seqFile.resetCounters();
    if (debugLvl != No) {
        std::cout << "Initial file: \n";
        seqFile.debugPrint(std::cout);
        std::cout << "\n";
    }
    SequentialFile t1("tape1.dat", DISK_PAGE_SIZE);
    SequentialFile t2("tape2.dat", DISK_PAGE_SIZE);

    SequentialFileSorter sorter(&seqFile, &t1, &t2);
    
    sorter.sortFile(debugLvl);
    if (debugLvl != No) {
        std::cout << "Sorted: \n";
        seqFile.debugPrint(std::cout);
        std::cout << "\n";
    }

    
    std::cout << "Number of phases: " << sorter.getPhases() << '\n';
    std::cout << "All disk page reads: " << seqFile.getCounterReads() + t1.getCounterReads() + t2.getCounterReads() << '\n';
    std::cout << "All disk page writes: " << seqFile.getCounterWrites() + t1.getCounterWrites() + t2.getCounterWrites() << '\n';
    
    return 0;
}


