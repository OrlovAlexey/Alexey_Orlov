#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <set>

using namespace std;

const int inf = 100000000;

void input(int n, int m, vector<vector<pair<int, int>>>& graph) {
    cin >> n >> m;
    int x, y, w;
    for (int i = 0; i < m; ++i) {
        cin >> x >> y >> w;
        graph[x - 1].push_back({y - 1, w});
        graph[y - 1].push_back({x - 1, w});
    }
}

void decreaseKey(multiset<pair<int, int>>& q, int x, int delta, vector<int>& key) {
    q.erase({key[x], x});
    q.insert({delta, x});
}

void prim(int start, int n, const vector<vector<pair<int, int>>>& graph, vector<int>& key, vector<int>& p, multiset<pair<int, int>>& q, unordered_set<int>& s) {
    for (int v = 0; v < n; ++v) {
        key[v] = inf;
        p[v] = inf;
    }
    int r = start;
    key[r] = 0;
    q.clear();
    s.clear();
    for (int v = 0; v < n; ++v) {
        q.insert({key[v], v});
        s.insert(v);
    }
    set<pair<int, int>>::iterator it;
    int v;
    while (!q.empty()) {
        it = q.begin();
        v = it->second;
        q.erase(it);
        s.erase(v);
        for (auto u : graph[v]) {
            int weight = u.second;
            if ((s.find(u.first) != s.end()) && key[u.first] > weight) {
                p[u.first] = v;
                decreaseKey(q, u.first, u.second, key);
                key[u.first] = weight;
            }
        }
    }
}

void answer_output(const vector<int>& key) {
    long long ans = 0;
    for (int weight : key) {
        ans += weight;
    }
    cout << ans << '\n';
}

int main() {
    int n, m;
    vector<vector<pair<int, int>>> graph(n);
    input(n, m, graph);
    vector<int> key(n);
    vector<int> p(n);
    multiset<pair<int, int>> q;
    unordered_set<int> s;
    prim(0, n, graph, key, p, q, s);
    answer_output(key);
    return 0;
}
