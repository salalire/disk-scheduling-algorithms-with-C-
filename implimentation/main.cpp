#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#include "scan.h"
#include "cscan.h"

using namespace std;

// ======================= INPUT STRUCT =======================

struct RequestData {
    int disk_size;
    int head;
    string direction;
    vector<int> requests;
};

// ======================= VALIDATION =========================

bool validate(const RequestData& data) {

    if (data.disk_size <= 0) {
        cout << "Error: Invalid disk size\n";
        return false;
    }

    if (data.head < 0 ||
        data.head >= data.disk_size) {

        cout << "Error: Invalid head position\n";
        return false;
    }

    if (data.direction != "left" &&
        data.direction != "right") {

        cout << "Error: Direction must be left or right\n";
        return false;
    }

    if (data.requests.empty()) {
        cout << "Error: Request queue is empty\n";
        return false;
    }

    for (int r : data.requests) {

        if (r < 0 || r >= data.disk_size) {

            cout << "Error: Request "
                 << r
                 << " out of range\n";

            return false;
        }
    }

    return true;
}

// ======================= SEEK TIME ==========================

int calculateSeekTime(
    const vector<int>& seq,
    int head
) {

    int total = 0;

    for (int x : seq) {

        total += abs(x - head);

        head = x;
    }

    return total;
}

// ======================= PRINT RESULT =======================

void printResult(
    string name,
    vector<int> seq,
    int seek
) {

    cout << "\n=== "
         << name
         << " ===\n";

    cout << "Seek Sequence: ";

    for (size_t i = 0; i < seq.size(); i++) {

        cout << seq[i];

        if (i != seq.size() - 1)
            cout << " -> ";
    }

    cout << "\nTotal Seek Time: "
         << seek
         << "\n";
}

// ======================= MAIN PROCESS =======================

void process(RequestData data) {

    // Validate input
    if (!validate(data)) {
        return;
    }

    // Run SCAN
    vector<int> scanSeq =
        scan(
            data.requests,
            data.head,
            data.disk_size,
            data.direction
        );

    // Run C-SCAN
    vector<int> cscanSeq =
        cscan(
            data.requests,
            data.head,
            data.disk_size
        );

    // Calculate seek times
    int scanSeek =
        calculateSeekTime(
            scanSeq,
            data.head
        );

    int cscanSeek =
        calculateSeekTime(
            cscanSeq,
            data.head
        );

    // Print outputs
    printResult(
        "SCAN",
        scanSeq,
        scanSeek
    );

    printResult(
        "C-SCAN",
        cscanSeq,
        cscanSeek
    );

    // Comparison
    cout << "\nWinner: ";

    if (scanSeek < cscanSeek)
        cout << "SCAN\n";

    else if (cscanSeek < scanSeek)
        cout << "C-SCAN\n";

    else
        cout << "TIE\n";
}
