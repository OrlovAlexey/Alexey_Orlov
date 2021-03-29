#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;
typedef long long ll;

inline bool bit(ll mask, ll pos) {
    return (mask >> pos) & 1;
}

void precounting_masks(ll n, vector<vector<bool>>& ok) {
    for (ll mask = 0; mask < (1 << n); ++mask) {
        for (ll mask1 = 0; mask1 < (1 << n); ++mask1) {
            for (ll i = 0; i < n - 1; ++i) {
                if (bit(mask, i) && bit(mask, i + 1) && bit(mask1, i) && bit(mask1, i + 1)) {
                    ok[mask][mask1] = false;
                }
                if (!bit(mask, i) && !bit(mask, i + 1) && !bit(mask1, i) && !bit(mask1, i + 1)) {
                    ok[mask][mask1] = false;
                }
            }
        }
    }
}

void solution(ll n, ll m, vector<vector<ll>>& dp, const vector<vector<bool>>& ok) {
    for (ll mask = 0; mask < (1 << n); ++mask) {
        dp[1][mask] = 1;
    }
    for (ll j = 1; j < m; ++j) {
        for (ll mask = 0; mask < (1 << n); ++mask) {
            for (ll mask1 = 0; mask1 < (1 << n); ++mask1) {
                if (ok[mask][mask1]) {
                    dp[j + 1][mask1] += dp[j][mask];
                }
            }
        }
    }
}

void printing_answer(ll n, ll m, const vector<vector<ll>>& dp) {
    ll ans = 0;
    for (ll mask = 0; mask < (1 << n); ++mask) {
        ans += dp[m][mask];
    }
    cout << ans << '\n';
}

int main() {
    ll m1, n1, m, n;
    cin >> m1 >> n1;
    n = std::min(m1, n1);
    m = std::max(m1, n1);
    vector<vector<bool> > ok((1 << n), vector<bool>(1 << n, true));
    precounting_masks(n, ok);
    vector<vector<ll> > dp(m + 1, vector<ll>(1 << n, 0));
    solution(n, m, dp, ok);
    printing_answer(n, m, dp);
    return 0;
}
