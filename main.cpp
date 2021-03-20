#include <iostream>
#include <vector>
#include <algorithm>

using std::cin;
using std::cout;
using std::vector;
using std::pair;
using std::min;
using std::max;

typedef long long ll;

void dp_solution(int n, const vector<ll>& v) {
    const ll inf = 10000000000;
    vector<ll> p(n+1);
    vector<ll> dp(n+1);
    vector<ll> prev_p(n);
    p[0] = -1;
    dp[0] = -inf;
    for (ll i = 1; i <= n; ++i) {
        dp[i] = inf;
    }
    ll length = 0;
    for (ll i = 0; i < n; ++i) {
        ll j = (ll)(std::upper_bound(dp.begin(), dp.end(), v[i]) - dp.begin());
        if (dp[j-1] <= v[i] && v[i] <= dp[j]) {
            dp[j] = v[i];
            p[j] = i;
            prev_p[i] = p[j-1];
            if (length < j)
                length = j;
        }
    }//filling in dp[i], parents[i], previous_parents[i]
    cout << length << '\n';
    for (ll j = p[length]; j > -1; j = prev_p[j]) {
        cout << n - j << " ";
    }//searching and printing answer
}

int main() {
    std::ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    ll n;
    cin >> n;
    vector<ll> original(n), v(n);
    for (ll i = 0; i < n; ++i) {
        cin >> original[i];
    }
    for (ll i = 0; i < n; ++i) {
        v[i] = original[n-1-i];
    }
    dp_solution(n, v);
    return 0;
}
