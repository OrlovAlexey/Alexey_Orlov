#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

int main() {
    int n, m;
    cin >> n >> m;
    vector<int> a(n), b(m);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    for (int j = 0; j < m; ++j) {
        cin >> b[j];
    }
    vector<vector<int>> dp(n+1, vector<int>(m+1, 0));
    vector<int> p(m+1);
    int best;
    for (int i = 1; i <= n; ++i) {
        best = 0;
        for (int j = 1; j <= m; ++j) {
            dp[i][j] = dp[i - 1][j];
            if (a[i-1] == b[j-1] && dp[i-1][j] < best + 1) {
                dp[i][j] = best + 1;
            }
            if (a[i-1] > b[j-1] && dp[i-1][j] > best) {
                best = dp[i-1][j];
            }
        }
    }
    int ans = 0;
    for (int j = 1; j <= m; ++j) {
        if (ans < dp[n][j]) {
            ans = dp[n][j];
        }
    }
    cout << ans << '\n';
    return 0;
}
