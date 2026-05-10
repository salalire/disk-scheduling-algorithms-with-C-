#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

#include "scan.h"
#include "cscan.h"
#include "comparison.h"
#include "headmovement.h"

using namespace std;

bool isValidDirection(const string& direction) {
    return direction == "left" || direction == "right";
}

bool areRequestsWithinDiskRange(const vector<int>& requests, int diskSize) {
    for (int request : requests) {
        if (request < 0 || request >= diskSize) {
            return false;
        }
    }
    return true;
}

void printSequence(const vector<int>& sequence) {
    for (size_t i = 0; i < sequence.size(); ++i) {
        cout << sequence[i];

        if (i + 1 < sequence.size()) {
            cout << " -> ";
        }
    }

    cout << '\n';
}

void printAlgorithmBlock(
    const string& title,
    const vector<int>& sequence,
    int seekTime,
    int head
) {
    cout << "\n=== " << title << " ===\n";

    cout << "Initial Head Position: "
         << head << '\n';

    cout << "Seek Sequence: ";
    printSequence(sequence);

    cout << "Total Seek Time: "
         << seekTime << '\n';
}

bool readInt(const string& prompt, int& value) {
    cout << prompt;

    if (cin >> value) {
        return true;
    }

    cin.clear();

    cin.ignore(
        numeric_limits<streamsize>::max(),
        '\n'
    );

    return false;
}

int readMenuChoice() {
    int choice = 0;

    while (true) {
        cout << "Enter choice: ";

        if (cin >> choice &&
            choice >= 1 &&
            choice <= 3) {

            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            return choice;
        }

        cout << "Error: choose 1, 2, or 3.\n";

        cin.clear();

        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );
    }
}

void saveToFile(
    const string& algorithm,
    const vector<int>& sequence
) {
    ofstream file("output.txt", ios::app);

    if (!file) {
        cout << "Error opening output file.\n";
        return;
    }

    file << algorithm << ": ";

    for (int value : sequence) {
        file << value << " ";
    }

    file << '\n';

    file.close();
}

int main() {

    int diskSize = 0;
    int requestCount = 0;
    int head = 0;
    int choice = 0;

    string direction;

    vector<int> requests;

    cout << "===== Disk Scheduling Simulator =====\n";
    cout << "Member 4: Input, Validation, and Output\n\n";

    while (
        !readInt("Enter disk size: ", diskSize)
        || diskSize <= 0
    ) {
        cout << "Error: disk size must be greater than 0.\n";
    }

    while (
        !readInt(
            "Enter number of requests: ",
            requestCount
        )
        || requestCount <= 0
    ) {
        cout << "Error: number of requests must be greater than 0.\n";
    }

    requests.resize(requestCount);

    cout << "Enter request sequence separated by spaces:\n";

    for (int i = 0; i < requestCount; ++i) {

        while (
            !(cin >> requests[i])
            || requests[i] < 0
            || requests[i] >= diskSize
        ) {

            cout << "Error: request must be between 0 and "
                 << diskSize - 1
                 << ". Enter request "
                 << i + 1
                 << " again: ";

            cin.clear();

            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );
        }
    }

    cin.ignore(
        numeric_limits<streamsize>::max(),
        '\n'
    );

    while (
        !readInt(
            "Enter initial head position: ",
            head
        )
        || head < 0
        || head >= diskSize
    ) {

        cout << "Error: head position must be between 0 and "
             << diskSize - 1
             << ".\n";
    }

    while (true) {

        cout << "Enter direction (left/right): ";

        if (!(cin >> direction)) {

            cout << "Error: invalid input.\n";

            cin.clear();

            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            continue;
        }

        if (isValidDirection(direction)) {
            break;
        }

        cout << "Error: direction must be "
             << "'left' or 'right'.\n";
    }

    cin.ignore(
        numeric_limits<streamsize>::max(),
        '\n'
    );

    cout << "\nChoose Operation:\n";
    cout << "1. Run SCAN\n";
    cout << "2. Run C-SCAN\n";
    cout << "3. Compare Both\n";

    choice = readMenuChoice();

    if (choice == 1) {

        vector<int> sequence =
            scan(requests, head, diskSize, direction);

        int seekTime =
            calculateSeekTime(sequence, head);

        printAlgorithmBlock(
            "SCAN Algorithm",
            sequence,
            seekTime,
            head
        );

        printHeadMovements(sequence, head);

        saveToFile("SCAN", sequence);
    }

    else if (choice == 2) {

        vector<int> sequence =
            cscan(requests, head, diskSize);

        int seekTime =
            calculateSeekTime(sequence, head);

        printAlgorithmBlock(
            "C-SCAN Algorithm",
            sequence,
            seekTime,
            head
        );

        printHeadMovements(sequence, head);

        saveToFile("CSCAN", sequence);
    }

    else {

        vector<int> scanSequence =
            scan(requests, head, diskSize, direction);

        vector<int> cscanSequence =
            cscan(requests, head, diskSize);

        int scanSeek =
            calculateSeekTime(scanSequence, head);

        int cscanSeek =
            calculateSeekTime(cscanSequence, head);

        cout << "\n=== Comparison ===\n";

        printAlgorithmBlock(
            "SCAN Algorithm",
            scanSequence,
            scanSeek,
            head
        );

        printHeadMovements(
            scanSequence,
            head
        );

        printAlgorithmBlock(
            "C-SCAN Algorithm",
            cscanSequence,
            cscanSeek,
            head
        );

        printHeadMovements(
            cscanSequence,
            head
        );

        saveToFile("SCAN", scanSequence);
        saveToFile("CSCAN", cscanSequence);

        cout << "\nWinner: ";

        if (scanSeek < cscanSeek) {
            cout << "SCAN\n";
        }

        else if (cscanSeek < scanSeek) {
            cout << "C-SCAN\n";
        }

        else {
            cout << "TIE\n";
        }
    }

    cout << "\nProgram finished successfully.\n";

    return 0;
}
