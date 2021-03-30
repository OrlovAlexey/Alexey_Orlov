#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;
using std::max;

long long solution(long long n, const vector<long long>& first_sequence, long long m, const vector<long long>& second_sequence, vector<vector<long long>>& dp) {
    for (long long i = 0; i <= n; ++i) {
        for (long long j = 0; j <= m; ++j) {
            dp[i][j] = 0;
        }
    }
    for (long long i = 1; i <= n; ++i) {
        for (long long j = 1; j <= m; ++j) {
            if (first_sequence[i-1] == second_sequence[j-1]) {
                dp[i][j] = max(1 + dp[i-1][j-1], max(dp[i-1][j], dp[i][j-1]));
            }
            else {
                dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
            }
        }
    }
    return dp[n][m];
}

int main() {
    long long n, m;
    cin >> n; // length of first_sequence
    vector<long long> first_sequence(n);
    for (long long i = 0; i < n; ++i) {
        cin >> first_sequence[i];
    }
    cin >> m; // length of second_sequence
    vector<long long> second_sequence(m);
    for (long long j = 0; j < m; ++j) {
        cin >> second_sequence[j];
    }
    vector<vector<long long>> dp(n+1, vector<long long>(m+1));
    cout << solution(n, first_sequence, m, second_sequence, dp) << '\n';
    return 0;
}
