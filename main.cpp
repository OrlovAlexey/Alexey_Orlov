#include <iostream>
#include <vector>

using namespace std;

void input(int n, vector<vector<int>>& g) {
    int v;
    for (int i = 1; i <= n; ++i) {
        cin >> v;
        g.at(v).push_back(i);
    }
}

void dfs(int v, vector<int>& tin, vector<int>& tout, vector<char>& color, vector<int>& par, const vector<vector<int>>& g, int& timer, int p = -1) {
    par.at(v) = p;
    tin.at(v) = timer++;
    color.at(v) = 'g';
    for (int to : g.at(v)) {
        if (color.at(to) != 'w') {
            continue;
        }
        dfs(to, tin, tout, color, par, g, timer, v);
    }
    tout.at(v) = timer++;
    color.at(v) = 'b';
}

void requests(int m, const vector<int>& tin, const vector<int>& tout) {
    cin >> m;
    int v, u;
    for (int j = 0; j < m; ++j) {
        cin >> v >> u;
        if (tin[v] < tin[u] && tout[u] < tout[v]) {
            cout << true << '\n';
        }
        else {
            cout << false << '\n';
        }
    }
}

int main() {
    int n, m;
    cin >> n;
    vector<vector<int>> graph(n + 1);
    input(n, graph);
    vector<int> par(n + 1);
    vector<int> tin(n + 1), tout(n + 1);
    int timer = 0;
    vector<char> color(n + 1, 'w');
    dfs(0, tin, tout, color, par, graph, timer);
    requests(m, tin, tout);
    return 0;
}