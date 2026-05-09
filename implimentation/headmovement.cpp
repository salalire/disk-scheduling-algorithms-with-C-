// headmovement.cpp
#include <iostream>
#include <vector>
#include <cmath>
#include "headmovement.h"

using namespace std;

void printHeadMovements(vector<int> sequence, int head) {
    int current = head;
    int total = 0;

    cout << "Step-by-step head movements:\n";
    for (int pos : sequence) {
        int dist = abs(pos - current);
        cout << current << " -> " << pos << " (distance " << dist << ")\n";
        total += dist;
        current = pos;
    }
    cout << "Total Seek Time (Head Movement): " << total << "\n";
}
