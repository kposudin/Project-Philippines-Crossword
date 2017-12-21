#include <iostream>
#include <vector>
#include <cassert>
#include <set>
#include <string>
#include <fstream>
#include "functions.h"
#include "globals.h"
using namespace std;

int main() {
    cout << "1 for file input // 0 for keyboard input"<< endl;
    bool inp;
    cin >> inp;
    if (!inp){
        cout << "Input size (rows, columns):\n";
        cin >> n >> m;
        assert(n <= CAP && n >= 1);
        assert(m <= CAP && m >= 1);
        cout << "..." << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                cin >> value[i][j];
                if (value[i][j] == 1) {
                    mark[i][j] = 1;
                }
                if (value[i][j] > TOTAL) {
                    cout << "No solution\n";
                    return 0;
                }
                if (value[i][j] > 1) {
                    s[value[i][j]].insert({ i, j });
                }
            }
        }
    }
    else {
        string path;
        cout <<"Input file path" << endl;
        cin >> path;
        ifstream fin;
        fin.open(path);
        fin >> n >> m;
        assert(n <= CAP && n >= 1);
        assert(m <= CAP && m >= 1);
        //cout << "..." << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                fin >> value[i][j];
                if (value[i][j] == 1) {
                    mark[i][j] = 1;
                }
                if (value[i][j] > TOTAL) {
                    cout << "No solution\n";
                    return 0;
                }
                if (value[i][j] > 1) {
                    s[value[i][j]].insert({ i, j });
                }
            }
        }
        
    }
    for (int i = 0; i <= n * m; ++i) {
        if (s[i].size() % 2 != 0) {
            cout << "No solution\n";
            return 0;
        }
    }
    iterate();
    cout << "No solution\n";
    return 0;
}
