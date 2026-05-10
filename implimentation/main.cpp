#include <iostream>
#include <vector>
#include <string>
#include "scan.h"
#include "cscan.h"
#include "comparison.h"
#include "headmovement.h"

using namespace std;

// ===== Validation =====
bool isValidDirection(string dir) {
    return (dir == "left" || dir == "right");
}

bool isValidRequest(vector<int>& requests, int disk_size) {
    for (int r : requests) {
        if (r < 0 || r >= disk_size)
            return false;
    }
    return true;
}

int main() {
    int disk_size, n, head, choice;
    string direction;

    cout << "===== Disk Scheduling Simulator =====\n";

    // ===== Input =====
    cout << "Enter disk size: ";
    cin >> disk_size;

    cout << "Enter number of requests: ";
    cin >> n;

    vector<int> requests(n);

    cout << "Enter request sequence: ";
    for (int i = 0; i < n; i++) {
        cin >> requests[i];
    }

    cout << "Enter initial head position: ";
    cin >> head;

    cout << "Enter direction (left/right): ";
    cin >> direction;

    // ===== Validation ====
    if (head < 0 || head >= disk_size) {
        cout << "Error: Invalid head position!\n";
        return 1;
    }

    if (!isValidDirection(direction)) {
        cout << "Error: Direction must be 'left' or 'right'!\n";
        return 1;
    }

    if (!isValidRequest(requests, disk_size)) {
        cout << "Error: Request out of disk range!\n";
        return 1;
    }

    // ===== Menu =====
    cout << "\nChoose Operation:\n";
    cout << "1. Run SCAN\n";
    cout << "2. Run C-SCAN\n";
    cout << "3. Compare Both\n";
    cout << "Enter choice: ";
    cin >> choice;

    cout << "\n";

    // ===== Execution =====
    if (choice == 1) {
        vector<int> seq = scan(requests, head, disk_size, direction);
        int seek = calculateSeekTime(seq, head);

        cout << "=== SCAN Algorithm ===\n";
        cout << "Seek Sequence: ";
        for (int x : seq) cout << x << " ";
        cout << "\nTotal Seek Time: " << seek << "\n\n";

        printHeadMovements(seq, head);
    }
    else if (choice == 2) {
        vector<int> seq = cscan(requests, head, disk_size);
        int seek = calculateSeekTime(seq, head);

        cout << "=== C-SCAN Algorithm ===\n";
        cout << "Seek Sequence: ";
        for (int x : seq) cout << x << " ";
        cout << "\nTotal Seek Time: " << seek << "\n\n";

        printHeadMovements(seq, head);
    }
    else if (choice == 3) {
        compareAlgorithms(requests, head, disk_size, direction);
    }
    else {
        cout << "Invalid choice!\n";
    }

    return 0;
}
