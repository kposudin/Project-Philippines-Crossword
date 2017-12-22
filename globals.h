#ifndef globals_h
#define globals_h
#include <iostream>
#include <vector>
#include <cassert>
#include <set>



using namespace std;


const int CAP = 100;
const int TOTAL = CAP * CAP + 1;

const int DIR = 4;
const int dy[4] = { -1, 0, 1, 0 };
const int dx[4] = { 0, 1, 0, -1 };

int value[CAP][CAP]; //value
bool mark[CAP][CAP]; //whether it is marked now or not
int n, m;
set <pair <int, int> > s[TOTAL + 2]; //s[i] contains all currently unmarked cells with value = i

pair <int, int> marked[TOTAL + 1]; //contains all marked spots to rollback.
int curmarked = 0;

#endif /* globals_h */
