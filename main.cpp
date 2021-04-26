#include <iostream>
#include <vector>

using namespace std;

void input(int n, vector<vector<bool>>& used) {
    bool input;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> input;
            if (i == j) {
                used[i][j] = true;
            }
            else {
                used[i][j] = input;
            }
        }
    }
}

void euler(int v, int n, vector<vector<bool>>& used, vector<int>& ans) {
    for (int u = 0; u < n; ++u) {
        if (!used[v][u]) {
            used[v][u] = true;
            euler(u, n, used, ans);
        }
    }
    ans.push_back(v + 1);
}

void output(const vector<int>& ans) {
    for (auto it = ans.rbegin(); it != ans.rend() - 1; ++it) {
        cout << *it << " " << *(it + 1) << '\n';
    }
}

int main() {
    int n, a;
    cin >> n >> a; --a;
    vector<vector<bool>> used(n, vector<bool>(n));
    input(n, used);
    vector<int> ans;
    euler(a, n, used, ans);
    output(ans);
    return 0;
}
