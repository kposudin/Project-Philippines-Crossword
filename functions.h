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
    mark[to.first][to.second] = true; //Placing mark, increasing version
}

void placemark(int x, int y) {
    placemark({ x, y });
}

void rollback(int version) {
    while (curmarked > version) {
        --curmarked;
        mark[marked[curmarked].first][marked[curmarked].second] = 0; //deleting mark in the cell untill we didnt downgrade to the needed version
    }
}

bool ismarked(int x, int y) {
    //is cell marked
    //cell is market if there's a number in it (except the beginnig or the end of the way) or its a part of the way.
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
        //A useful cut not to consider a useless ways many times
        return;
    }
    placemark(curx, cury);
    int ver = curmarked;
    if (distleft == 0) {
        if (curx == dest.first && cury == dest.second) {
            iterate(); //If we made a way (s-t), we're trying to build the whole board.
        }
        rollback(ver); //If we didn't, we got to clean everything and finish
        return;
    }
    for (int dir = 0; dir < DIR; ++dir) {
        int x = curx + dx[dir];
        int y = cury + dy[dir];
        if (!ismarked(x, y) || pair<int, int>(x, y) == dest) {
            recursive(x, y, dest, distleft - 1); //going to a random way, searching the rest of the way
            rollback(ver); //if none of the ends of the way is working, rolling back.
        }
    }
}

void match(const pair <int, int>& s, const pair <int, int>& t) {
    int ver = curmarked; //remembering the current state of the board
    recursive(s.first, s.second, t, value[s.first][s.second] - 1); //Recursively we sort out all possible ways
    rollback(ver); //If we can not complete any way to the full board, then we roll back the version back
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
        //searching the cells that would be the easiest to find a couple to
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
                //if there're no potential couple then we can not build a pair
                return;
            }
            if (cnt == 1) {
                s[value].erase(things[i]);
                s[value].erase(things[pos]);
                
                //making a couples and trying to build the way. if we can not we're returning them back to the set end trying again because we can't build it to the end
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
    { int const i = 0;
        for (int j = i + 1; j < len; ++j) {
            int temp = mindist(things[i], things[j]);
            if (temp < value && temp % 2 != value % 2) {
                s[value].erase(things[i]);
                s[value].erase(things[j]);
                
                match(things[i], things[j]);
                
                s[value].insert(things[i]);
                s[value].insert(things[j]);
            }
        }
    }
}


#endif /* functions_h */
