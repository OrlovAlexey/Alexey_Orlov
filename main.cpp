#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void input(int m, vector<vector<int>>& g) {
    int v, u;
    for (int i = 0; i < m; ++i) {
        cin >> v >> u;
        g[v - 1].push_back(u - 1);
        g[u - 1].push_back(v - 1);
    }
}

void dfs(int v, const vector<vector<int>>& g, vector<int>& tin, vector<int>& ret, vector<bool>& used, int timer, vector<bool>& points, int ans, int p = -1) {
    tin[v] = timer++;
    ret[v] = tin[v];
    used[v] = true;
    int children = 0;
    for (int to : g[v]) {
        if (to == p)
            continue;
        if (used[to]) {
            ret[v] = min(ret[v], tin[to]);
        }
        else {
            dfs(to, g, tin, ret, used, timer, points, ans, v); ++children;
            ret[v] = min(ret[v], ret[to]);
            if (p != -1) {
                if (ret[to] >= tin[v]) {
                    if (!points[v])
                        ++ans;
                    points[v] = true;
                }
            }
        }
    }
    if (p == -1 && children >= 2) {
        if (!points[v])
            ++ans;
        points[v] = true;
    }
}

void solution(int n, const vector<vector<int>>& g, vector<int>& tin, vector<int>& ret, vector<bool>& used, int timer, vector<bool>& points, int ans) {
    for (int s = 0; s < n; ++s) {
        if (!used[s]) {
            dfs(s, g, tin, ret, used, timer, points, ans);
        }
    }
    cout << ans << '\n';
    for (int i = 0; i < n; ++i) {
        if (points[i]) {
            cout << i + 1 << " ";
        }
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> g(n);
    input(m, g);
    vector<int> tin, tout, ret;
    tin.resize(n), tout.resize(n), ret.resize(n);
    vector<bool> used(n, false), points(n);
    int timer = 0, ans = 0;
    solution(n, g, tin, ret, used, timer, points, ans);
    return 0;
}