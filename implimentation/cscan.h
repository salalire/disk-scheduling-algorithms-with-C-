#ifndef CSCAN_H
#define CSCAN_H

#include <vector>
using namespace std;

vector<int> cscan(vector<int> requests, int head, int disk_size);
int calculateSeekTime(vector<int> sequence, int head);

#endif