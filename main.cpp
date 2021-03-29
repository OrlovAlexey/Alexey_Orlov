#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;
using std::pair;
typedef long long ll;

inline bool bit(ll mask, ll pos) {
    return (mask >> pos) & 1;
}//checks if pos bit is in mask

int main() {
    ll inf = 1000000000;
    ll n;
    cin >> n;
    vector<vector<ll>> roads(n, vector<ll>(n));
    for (ll i = 0; i < n; ++i) {
        for (ll j = 0; j < n; ++j) {
            cin >> roads[i][j];
        }
    }
    vector<vector<ll>> dp(n, vector<ll>((1 << n) + 1, inf));
    vector<vector<pair<ll, ll>>> prev(n, vector<pair<ll, ll>>(((1 << n) + 1), {-1, 0}));
    for (ll v = 0; v < n; ++v) {
        dp[v][1 << v] = 0;
    }
    ll newmask;
    for (ll mask = 1; mask < (1 << n); ++mask) {
        for (ll v = 0; v < n; ++v) {
            for (ll u = 0; u < n; ++u) {
                if (u != v) {
                    if (bit(mask, u)) {
                        continue;
                    }
                    newmask = mask | (1 << u);
                    if (dp[v][mask] + roads[v][u] < dp[u][newmask]) {
                        dp[u][newmask] = dp[v][mask] + roads[v][u];
                        prev[u][newmask] = {v, mask};
                    }
                }
            }
        }
    }
    ll ans = inf;
    ll best_i;
    for (ll i = 0; i < n; ++i) {
        if (dp[i][(1 << n) - 1] <= ans) {
            ans = dp[i][(1 << n) - 1];
            best_i = i;
        }
    }
    cout << ans << '\n';
    ll temp;
    for(ll i = best_i, mask = (1 << n) - 1; i >= 0;) {
        cout << i + 1 << '\t';
        temp = i;
        i = prev[temp][mask].first;
        mask = prev[temp][mask].second;
    }
    return 0;
}
