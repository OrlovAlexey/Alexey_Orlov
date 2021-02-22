#include <iostream>
#include <vector>
#include <unordered_set>
#include <iomanip>

using std::vector;
using std::cin;
using std::cout;
using std::pair;

int count_cycles(const vector<pair<int, int>>& ust, const vector<std::unordered_set<int>>& v){
    int count = 0;
    for (int i = 0; i < ust.size(); ++i) {
        pair<int, int> p = ust[i];
        if (v[p.first].size() <= v[p.second].size()) {
            for (auto vertex : v[p.first]) {
                if (vertex > p.second && vertex > p.first) {
                    if (v[p.second].find(vertex) != v[p.second].end()) {
                        ++count;
                    }
                }
            }
        }
        else {
            for (auto vertex : v[p.second]) {
                if (vertex > p.second && vertex > p.first) {
                    if (v[p.first].find(vertex) != v[p.first].end()) {
                        ++count;
                    }
                }
            }
        }
    }
    return count;
}

int main() {
    std::ios::sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    int x, y;
    vector<std::unordered_set<int>> v(n); // unordered_set uses perfect hashing
    vector<pair<int, int>> ust(m);
    for (int i = 0; i < m; ++i) {
        cin >> x >> y;
        ust[i] = {x-1, y-1};
        v[x-1].insert(y-1);
        v[y-1].insert(x-1);
    }
    int cycles = count_cycles(ust, v);
    cout << std::fixed << std::setprecision(17) << (double)(cycles) / (double)(4);
    return 0;
}