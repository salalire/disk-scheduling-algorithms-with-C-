// scan.h

#ifndef SCAN_H
#define SCAN_H

#include <vector>
#include <string>

using namespace std;

vector<int> scan(vector<int> requests, int head, int disk_size, string direction);

int calculateSeekTime(vector<int> sequence, int head);

#endif