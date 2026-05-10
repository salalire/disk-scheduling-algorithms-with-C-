#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "cscan.h"

using namespace std;

vector<int> cscan(vector<int> requests, int head, int disk_size) {
    vector<int> sequence;
    vector<int> left, right;

    for (int req : requests) {
        if (req < head)
            left.push_back(req);
        else
            right.push_back(req);
    }

    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    // Service right side
    for (int req : right) {
        sequence.push_back(req);
    }

    // Move to end
    if (!right.empty()) {
        sequence.push_back(disk_size - 1);
    }

    // Jump and service left
    if (!left.empty()) {
        sequence.push_back(0);
        for (int req : left) {
            sequence.push_back(req);
        }
    }

    return sequence;
}

// calculateSeekTime is defined in scan.cpp 