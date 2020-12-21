#include <iostream>
#include <vector>
#include <algorithm>

using std::cin;
using std::cout;
using std::min;
using std::max;
using std::pair;

typedef long long ll;

ll log(ll n) {
    if (n == 1 || n == 0) {
        return 0;
    }
    else {
        return log(n / 2) +1;
    }
}

int main() {
    ll n, m;
    
    cin >> n >> m;
    ll a[n];
    for (ll i = 0; i < n; ++i) {
        cin >> a[i];
    }
    ll logn = log(n);
    pair<pair<ll, ll>, pair<ll, ll>> sparse[logn + 1][n];// минимум и его индекс + вторая статистика и ее индекс

    for (ll i = 0; i < n; ++i) {
        sparse[0][i].first.first = a[i];
        sparse[0][i].first.second = i;
        sparse[0][i].second.first = -1;
        sparse[0][i].second.second = i;
    }
    for (ll i = 0; i < n; ++i) {
        ll j = min(n - 1, i + 1);
        if (a[i] < a[j]) {
            sparse[1][i].first.first = a[i];
            sparse[1][i].first.second = i;
            sparse[1][i].second.first = a[j];
            sparse[1][i].second.second = j;
        } else {
            sparse[1][i].first.first = a[j];
            sparse[1][i].first.second = j;
            sparse[1][i].second.first = a[i];
            sparse[1][i].second.second = i;
        }
    }
    std::vector<pair<ll, ll>> v;
    for (ll r = 1; r < logn; ++r) {
        for (ll i = 0; i < n; ++i) {
            v.clear();
            v.push_back(sparse[r][i].first);
            v.push_back(sparse[r][i].second);
            v.push_back(sparse[r][min(n - 1, i + (1 << r))].first);
            v.push_back(sparse[r][min(n - 1, i + (1 << r))].second);
            std::sort(v.begin(), v.end());
            sparse[r + 1][i].first = v[0];
            sparse[r + 1][i].second.first = v[1].first;
            sparse[r + 1][i].second.second = v[1].second;
        }
    }

    std::vector<ll> k(n + 1);
    k[2] = 1;
    for (ll len = 3; len < n + 1; ++len) {
        if (!(len & (len - 1))) {
            k[len] = k[len - 1] + 1;
        } else {
            k[len] = k[len - 1];
        }
    }

    ll l, r;
    for (ll i = 0; i < m; ++i) {
        cin >> l >> r;
        ll pwr = k[r - l + 1];
        ll j = r - (1 << pwr);
        ll ans;
        pair<ll, ll> min1 = sparse[pwr][l - 1].first;
        pair<ll, ll> rmq1 = sparse[pwr][l - 1].second;
        pair<ll, ll> min2 = sparse[pwr][j].first;
        pair<ll, ll> rmq2 = sparse[pwr][j].second;

        if (min1.first < min2.first) {
            if (rmq1.first < min2.first) {
                ans = rmq1.first;
            } else {
                ans = min2.first;
            }
        } else if (min1.first > min2.first) {
            if (rmq2.first < min1.first) {
                ans = rmq2.first;
            } else {
                ans = min1.first;
            }
        } else if (min1.second != min2.second) {
            ans = min1.first;
        } else if (rmq1.first < rmq2.first) {
            ans = rmq1.first;
        } else {
            ans = rmq2.first;
        }

        cout << ans << '\n';
    }
    return 0;
}
