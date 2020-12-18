#include <iostream>

using std::cin;
using std::cout;

typedef long long ll;

ll f(ll x) {
    return x & (x+1);
}

ll g(ll x) {
    return x | (x+1);
}

void build(ll s[], ll a[], ll n) {
    ll p[n];
    p[0] = a[0];
    for (ll i = 0; i < n-1; ++i) {
        if (i % 2 == 0) {
            p[i+1] = p[i] - a[i+1];
        }
        else {
            p[i+1] = p[i] + a[i+1];
        }
    }
    for (ll i = 0; i < n; ++i) {
        if (f(i) > 0) {
            s[i] = p[i] - p[f(i) - 1];
        }
        else {
            s[i] = p[i];
        }
    }
}

ll get_sum(ll s[], ll pos) {
    ll ans = 0;
    for (ll i = pos; i >= 0; i = f(i) - 1) {
        ans += s[i];
    }
    return ans;
}

void update(ll s[], ll a[], ll n, ll pos, ll value) {
    ll prev_value = a[pos-1];
    if (pos % 2 != 0) {// лежит с плюсом
        for (ll i = pos-1; i < n; i = g(i)) {
            s[i] += (value - prev_value);
        }
        a[pos-1] = value;
    }
    else {// лежит с минусом
        for (ll i = pos-1; i < n; i = g(i)) {
            s[i] += (-value + prev_value);
        }
        a[pos-1] = value;
    }
}

int main() {
    ll n, m;
    cin >> n;
    ll a[n];
    for (ll i = 0; i < n; ++i) {
        cin >> a[i];
//        if (i % 2 != 0) {
//            a[i] = -a[i];
//        }
    }
    ll s[n];
    build(s, a, n);
    cin >> m;
    ll x, l, r;
    for (ll i = 0; i < m; ++i) {
        cin >> x >> l >> r;
        if (x == 0) {
            update(s, a, n, l, r);
        }
        if (x == 1) {
            ll pref1 = get_sum(s, l-2);
            ll pref2 = get_sum(s, r-1);
            if (l % 2 != 0) {
                cout << pref2 - pref1 << '\n';
            }
            else {
                cout << pref1 - pref2 << '\n';
            }
        }
    }
    return 0;
}
