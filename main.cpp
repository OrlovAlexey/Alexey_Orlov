#include <iostream>
#include <vector>

using namespace std;

long long cycle_start = -1;
long long cycle_end = -1;

bool dfs(long long v, vector<char>& color, vector<long long>& par, const vector<vector<long long>>& graph) {
    color[v] = 'g';
    for (long long to : graph[v]) {
        if (color[to] == 'g') {
            cycle_start = to;
            cycle_end = v;
            return true;
        }
        if (color[to] == 'w') {
            par[to] = v;
            if (dfs(to, color, par, graph)) {
                return true;
            }
        }
    }
    color[v] = 'b';
    return false;
}

void solution(long long n, bool& verdict, vector<char>& color, vector<long long>& par, const vector<vector<long long>>& graph) {
    for (long long s = 0; s < n; ++s) {
        verdict = dfs(s, color, par, graph);
        if (verdict) {
            break;
        }
    }
}

void print_of_answer(bool verdict, const vector<long long>& par) {
    if (!verdict) {
        cout << "NO";
    }
    else {
        cout << "YES" << '\n';
        vector<long long> ans;
        ans.push_back(cycle_start + 1);
        for (long long v = cycle_end; v != -1 && v != cycle_start; v = par[v]) {
            ans.push_back(v + 1);
        }
        for (auto it = ans.rbegin(); it != ans.rend(); ++it) {
            cout << *it << " ";
        }
    }
}

int main() {
    long long n, m;
    cin >> n >> m;
    vector<vector<long long>> graph(n);
    long long x, y;
    for (long long i = 0; i < m; ++i) {
        cin >> x >> y;
        graph[x - 1].push_back(y - 1);
    }// input
    vector<char> color(n, 'w');
    vector<long long> par(n, -1);
    bool verdict;
    solution(n, verdict, color, par, graph);
    print_of_answer(verdict, par);
    return 0;
}