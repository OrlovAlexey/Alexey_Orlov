#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;
using std::max;

void filling_base(int capacity, int number_of_gold, vector<vector<int>>& dp) {
    for (int i = 0; i < capacity + 1; ++i) {
        dp[i].resize(number_of_gold + 1);
        for (int j = 0; j < number_of_gold + 1; ++j) {
            dp[i][j] = 0;
        }
    }
}

void solution_actually(int capacity, int number_of_gold, const vector<int>& weights, vector<vector<int>>& dp) {
    for (int i = 1; i < number_of_gold + 1; ++i) {
        for (int j = 1; j < capacity + 1; ++j) {
            if (weights[i-1] <= j) {
                dp[j][i] = max(dp[j][i-1], dp[j - weights[i-1]][i-1] + weights[i-1]);
            }
            else {
                dp[j][i] = dp[j][i-1];
            }
        }
    }
    cout << dp[capacity][number_of_gold] << '\n';
}

int main() {
    int capacity, number_of_gold;
    cin >> capacity >> number_of_gold;
    vector<int> weights(number_of_gold);
    for (int i = 0; i < number_of_gold; ++i) {
        cin >> weights[i];
    }
    vector<vector<int>> dp(capacity + 1);
    filling_base(capacity, number_of_gold, dp);
    solution_actually(capacity, number_of_gold, weights, dp);
    return 0;
}
