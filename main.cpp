#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;
using std::max;

int main() {
    int S, N;
    cin >> S >> N;
    vector<int> w(N);
    for (int i = 0; i < N; ++i) {
        cin >> w[i];
    }
    vector<vector<int>> dp(S+1);
    for (int i = 0; i < S+1; ++i) {
        dp[i].resize(N+1);
        for (int j = 0; j < N+1; ++j) {
            dp[i][j] = 0;
        }
    }
    for (int i = 1; i < N+1; ++i) {
        for (int j = 1; j < S+1; ++j) {
            if (w[i-1] <= j) {
                dp[j][i] = max(dp[j][i-1], dp[j - w[i-1]][i-1] + w[i-1]);
            }
            else {
                dp[j][i] = dp[j][i-1];
            }
        }
    }
    cout << dp[S][N] << '\n';
    return 0;
}
