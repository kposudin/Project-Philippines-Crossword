#ifndef functions_h
#define functions_h
#include <iostream>
#include <vector>
#include <cassert>
#include <set>
#include "globals.h"


using namespace std;
void placemark(const pair <int, int>& to) {
    marked[curmarked++] = to;
    mark[to.first][to.second] = true;
}

void placemark(int x, int y) {
    placemark({ x, y });
}

void rollback(int version) {
    while (curmarked > version) {
        --curmarked;
        mark[marked[curmarked].first][marked[curmarked].second] = 0;
    }
}

bool ismarked(int x, int y) {
    if (x < 0 || x >= n)
        return true;
    if (y < 0 || y >= m)
        return true;
    if (value[x][y])
        return true;
    return mark[x][y];
}

int distx(const pair <int, int>& lhs, const pair <int, int>& rhs) {
    return abs(lhs.first - rhs.first);
}

int disty(const pair <int, int>& lhs, const pair <int, int>& rhs) {
    return abs(lhs.second - rhs.second);
}

int mindist(const pair <int, int>& lhs, const pair <int, int>& rhs) {
    return distx(lhs, rhs) + disty(lhs, rhs);
}

void iterate();

void recursive(int curx, int cury, const pair <int, int>& dest, int distleft) {
    if (mindist({ curx, cury }, dest) > distleft) {
        //if I don't do this, this will be a disaster
        return;
    }
    placemark(curx, cury);
    int ver = curmarked;
    if (distleft == 0) {
        if (curx == dest.first && cury == dest.second) {
            iterate();
        }
        rollback(ver);
        return;
    }
    for (int dir = 0; dir < DIR; ++dir) {
        int x = curx + dx[dir];
        int y = cury + dy[dir];
        if (!ismarked(x, y) || pair<int, int>(x, y) == dest) {
            recursive(x, y, dest, distleft - 1);
            rollback(ver);
        }
    }
}

void match(const pair <int, int>& s, const pair <int, int>& t) {
    int ver = curmarked;
    recursive(s.first, s.second, t, value[s.first][s.second] - 1);
    rollback(ver);
}

void printfield() {
    for (int a = 0; a < n; ++a, cout << '\n') {
        for (int b = 0; b < m; ++b) {
            cout << (mark[a][b] ? "█" : " ");
        }
    }
}

void iterate() {
    int i = 2;
    for (; i <= TOTAL && s[i].size() == 0; ++i);
    if (i == TOTAL + 1) {
        cout << "Solvable\n";
        printfield();
        exit(0);
    }
    for (int value = 2; value <= TOTAL; ++value) {
        vector <pair <int, int> > things;
        things.reserve(s[value].size());
        for (const auto& x : s[value]) {
            things.push_back(x);
        }
        int len = things.size();
        for (int i = 0; i < len; ++i) {
            int cnt = 0;
            int pos = -1;
            for (int j = 0; j < len; ++j) {
                if (j == i) continue;
                int temp = mindist(things[i], things[j]);
                if (temp < value && temp % 2 != value % 2) {
                    pos = j;
                    ++cnt;
                }
            }
            if (cnt == 0) {
                return;
            }
            if (cnt == 1) {
                s[value].erase(things[i]);
                s[value].erase(things[pos]);
                //cerr << "Optimized\n";
                //printfield();
                match(things[i], things[pos]);
                s[value].insert(things[i]);
                s[value].insert(things[pos]);
                return;
            }
        }
    }
    int value = i;
    vector <pair <int, int> > things;
    things.reserve(s[value].size());
    for (const auto& x : s[value]) {
        things.push_back(x);
    }
    int len = things.size();
    for (int i = 0; i < 1; ++i) {
        for (int j = i + 1; j < len; ++j) {
            int temp = mindist(things[i], things[j]);
            if (temp < value && temp % 2 != value % 2) {
                s[value].erase(things[i]);
                s[value].erase(things[j]);
                //cerr << "NonOptimized\n";
                match(things[i], things[j]);
                s[value].insert(things[i]);
                s[value].insert(things[j]);
            }
        }
    }
}



#endif /* functions_h */
