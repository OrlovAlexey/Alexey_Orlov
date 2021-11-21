#include <iostream>

using ll = long long;

void solution(ll n, ll lp[], ll primes[], ll k) {
    ll answer = 0;
    for (ll i = 2; i <= n; ++i) {
        if (lp[i] == 0) {
            lp[i] = i;
            primes[k++] = i;
        }
        else {
            answer += lp[i];
        }
        for (int j = 0; j < k && primes[j] <= lp[i] && i * primes[j] <= n; ++j) {
            lp[i * primes[j]] = primes[j];
        }
    }
    std::cout << answer;
}

int main() {
    ll n;
    std::cin >> n;
    ll lp[n + 1];
    for (ll i = 0; i < n + 1; ++i) {
        lp[i] = 0;
    }
    ll primes[n + 1];
    ll k = 0;

    solution(n, lp, primes, k);
    return 0;
}