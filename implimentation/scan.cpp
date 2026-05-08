#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "scan.h"

using namespace std;

vector<int> scan(vector<int> requests, int head, int disk_size, string direction) {
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

    if (direction == "right") {

        for (int req : right) {
            sequence.push_back(req);
        }

        if (!right.empty() && sequence.back() != disk_size - 1) {
            sequence.push_back(disk_size - 1);
        }

        for (int i = left.size() - 1; i >= 0; i--) {
            sequence.push_back(left[i]);
        }

    }
    else {

        for (int i = left.size() - 1; i >= 0; i--) {
            sequence.push_back(left[i]);
        }

        if (!left.empty() && sequence.back() != 0) {
            sequence.push_back(0);
        }

        for (int req : right) {
            sequence.push_back(req);
        }
    }

    return sequence;
}

int calculateSeekTime(vector<int> sequence, int head) {
    int total = 0;
    int current = head;

    for (int pos : sequence) {
        total += abs(pos - current);
        current = pos;
    }

    return total;
}