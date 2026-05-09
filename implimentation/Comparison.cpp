// comparison.cpp
#include <iostream>
#include <vector>
#include "scan.h"
#include "cscan.h"
#include "comparison.h"

using namespace std;

void compareAlgorithms(vector<int> requests, int head, int disk_size, string direction) {
    // Run SCAN
    vector<int> scanSeq = scan(requests, head, disk_size, direction);
    int scanSeek = calculateSeekTime(scanSeq, head);

    // Run C-SCAN
    vector<int> cscanSeq = cscan(requests, head, disk_size);
    int cscanSeek = calculateSeekTime(cscanSeq, head);

    // Print results
    cout << "SCAN Algorithm:" << endl;
    cout << "Seek Sequence: ";
    for (int pos : scanSeq) cout << pos << " ";
    cout << "\nTotal Seek Time: " << scanSeek << endl;

    cout << "\nC-SCAN Algorithm:" << endl;
    cout << "Seek Sequence: ";
    for (int pos : cscanSeq) cout << pos << " ";
    cout << "\nTotal Seek Time: " << cscanSeek << endl;

    // Comparison
    cout << "\nComparison:" << endl;
    if (scanSeek < cscanSeek)
        cout << "SCAN is more efficient (" << scanSeek << " vs " << cscanSeek << ")." << endl;
    else if (cscanSeek < scanSeek)
        cout << "C-SCAN is more efficient (" << cscanSeek << " vs " << scanSeek << ")." << endl;
    else
        cout << "Both algorithms have equal performance (" << scanSeek << ")." << endl;
}
