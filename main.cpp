#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;
using std::max;

int main() { // вся функциональность в main, за это 0
    long long n, m;
    cin >> n;
    vector<long long> s(n);
    for (long long i = 0; i < n; ++i) {
        cin >> s[i];
    }
    cin >> m;
    vector<long long> t(m); //однобуквенные имена лучше не использовать
    for (long long j = 0; j < m; ++j) {
        cin >> t[j];
    }
    vector<vector<long long>> dp(n+1, vector<long long>(m+1));
    for (long long i = 0; i <= n; ++i) {
        for (long long j = 0; j <= m; ++j) {
            dp[i][j] = 0;
        }
    }
    for (long long i = 1; i <= n; ++i) {
        for (long long j = 1; j <= m; ++j) {
            if (s[i-1] == t[j-1]) {
                dp[i][j] = max(1 + dp[i-1][j-1], max(dp[i-1][j], dp[i][j-1]));
            }
            else {
                dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
            }
        }
    }
    cout << dp[n][m] << '\n';
    return 0;
}
