#include <iostream>

using std::cin;
using std::cout;
typedef long long ll;
const ll mod = 1000000007;

ll bin_pow_mod(ll a, ll k) {
    ll ans = 1;
    while (k > 0) {
        if (k % 2) {
            ans = (ans % mod) * (a % mod) % mod;
        }
        a = (a % mod) * (a % mod) % mod;
        k /= 2;
    }
    return ans;
}

ll C(ll n, ll k, ll fact[], ll inf[]) {
    if (k > n || n < 0) {
        return 0;
    }
    if (k == 0 || n == k) {
        return 1;
    }

    return fact[n] * ((inf[n - k] % mod) * (inf[k] % mod) % mod) % mod;
}

ll S(ll n, ll k, ll fact[], ll inf[]) {
    if (n == 0)  {
        return (k == 0);
    }
    if (k == 0) {
        return (n == 0);
    }

    ll ans = 0;
    ll sgn[2] = {1, mod - 1};
    for (ll cnt = 0; cnt < k; ++cnt) {
        ans = (ans + (sgn[cnt & 1] * (((C(k, cnt, fact, inf) % mod) * bin_pow_mod(k - cnt, n)) % mod))) % mod;
    }
    return (ans * inf[k]) % mod;
}

ll normalization_mod(ll a) {
    while (a < 0) {
        a += mod;
    }
    return a % mod;
}

int main() {
    const ll N = 201 * 1000;
    ll fact[N], inf[N];
    fact[0] = 1;
    inf[0] = 1;
    for (ll i = 1; i < N; ++i) {
        fact[i] = (fact[i - 1] % mod) * (i % mod) % mod;
        inf[i] = (inf[i - 1] % mod) * (bin_pow_mod(i, mod - 2) % mod) % mod;
    }
    ll n, k, w[N];
    cin >> n >> k;
    for (ll i = 0; i < n; ++i) {
        cin >> w[i];
    }
    ll sum = 0;
    for (ll i = 0; i < n; i++) {
        sum = normalization_mod(sum + w[i]);
    }

    ll S0 = S(n, k, fact, inf);
    ll S1 = ((n - 1) * S(n - 1, k, fact, inf)) % mod;

    cout << ((sum % mod) * normalization_mod(S0 + S1)) % mod << '\n';
    return 0;
}
