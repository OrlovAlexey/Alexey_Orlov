#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
using std::pair;
using std::cin;
using std::cout;

void input(int n, int m, vector<pair<int, pair<int, int>>>& graph) {
    cin >> n >> m;
    for (int i = 0; i < m; ++i) {
        cin >> graph[i].second.first >> graph[i].second.second >> graph[i].first;
    }
}

int get(int v, vector<int>& p) {
    if (p[v] == -1) {
        return v;
    }
    else
        return p[v] = get(p[v], p);
}

void unite(int u, int v, vector<int>& p, vector<int>& size) {
    u = get(u, p);
    v = get(v, p);
    if (u == v)
        return;
    if (size[u] < size[v])
        std::swap(u, v);
    p[v] = u;
    size[u] += size[v];
}

long long solution(int n, int m, vector<pair<int, pair<int, int>>>& graph) {
    std::sort(graph.begin(), graph.end());
    vector<int> p(n, -1);
    vector<int> size(n);
    long long ans = 0;
    for (int i = 0; i < m; ++i) {
        int v = graph[i].second.first - 1;
        int to = graph[i].second.second - 1;
        int w = graph[i].first;
        if (get(v, p) != get(to, p)) {
            ans += w;
            unite(v, to, p, size);
        }
    }
    return ans;
}

int main() {
    int n, m;
    vector<pair<int, pair<int, int>>> graph(m);
    input(n, m, graph);
    cout << solution(n, m, graph) << '\n';
    return 0;
}
