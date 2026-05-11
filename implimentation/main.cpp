#include <iostream>
#include <vector>
#include "scan.h"
#include "cscan.h"
#include "comparison.h"
#include "headmovement.h"

using namespace std;

int main() {

    int disk_size, n, head;
    string direction;
    vector<int> requests;

    cout << "===== Disk Scheduling Simulator =====\n";

    cout << "Enter disk size: ";
    cin >> disk_size;

    cout << "Enter number of requests: ";
    cin >> n;

    cout << "Enter request queue:\n";

    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;

        // validation
        if (x < 0 || x >= disk_size) {
            cout << "Invalid request!\n";
            return 1;
        }

        requests.push_back(x);
    }

    cout << "Enter initial head position: ";
    cin >> head;

    if (head < 0 || head >= disk_size) {
        cout << "Invalid head position!\n";
        return 1;
    }

    cout << "Enter direction (left/right): ";
    cin >> direction;

    if (direction != "left" && direction != "right") {
        cout << "Invalid direction!\n";
        return 1;
    }

    // SCAN
    vector<int> scanSeq = scan(requests, head, disk_size, direction);
    int scanSeek = calculateSeekTime(scanSeq, head);

    cout << "\n===== SCAN Algorithm =====\n";
    cout << "Seek Sequence:\n";

    for (int pos : scanSeq) {
        cout << pos << " ";
    }

    cout << "\nTotal Seek Time: " << scanSeek << endl;
    printHeadMovements(scanSeq, head);

    // C-SCAN
    vector<int> cscanSeq = cscan(requests, head, disk_size);
    int cscanSeek = calculateSeekTime(cscanSeq, head);

    cout << "\n===== C-SCAN Algorithm =====\n";
    cout << "Seek Sequence:\n";

    for (int pos : cscanSeq) {
        cout << pos << " ";
    }

    cout << "\nTotal Seek Time: " << cscanSeek << endl;
    printHeadMovements(cscanSeq, head);

    // Comparison
    cout << "\n===== Comparison =====\n";

    if (scanSeek < cscanSeek)
        cout << "SCAN is more efficient.\n";
    else if (cscanSeek < scanSeek)
        cout << "C-SCAN is more efficient.\n";
    else
        cout << "Both have equal performance.\n";

    return 0;
}
