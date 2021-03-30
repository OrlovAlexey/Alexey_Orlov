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

void dp_solution(int n, const vector<ll>& input) {
    const ll inf = 10000000000;
    vector<ll> parents(n + 1);
    vector<ll> dynamic_programming(n + 1);
    vector<ll> previous_parents(n);
    parents[0] = -1;
    dynamic_programming[0] = -inf;
    for (ll i = 1; i <= n; ++i) {
        dynamic_programming[i] = inf;
    }
    ll length = 0;
    for (ll i = 0; i < n; ++i) {
        ll j = (ll)(std::upper_bound(dynamic_programming.begin(), dynamic_programming.end(), input[i]) - dynamic_programming.begin());
        dynamic_programming[j] = input[i];// убрал условие, поменял переменные
        parents[j] = i;
        previous_parents[i] = parents[j-1];
        if (length < j)
            length = j;
    }//filling in dp[i], parents[i], previous_parents[i]
    cout << length << '\n';
    for (ll j = parents[length]; j > -1; j = previous_parents[j]) {
        cout << n - j << " ";
    }//searching and printing answer
}

int main() {
    std::ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    ll n;
    cin >> n;
    vector<ll> original(n), reversed(n);
    for (ll i = 0; i < n; ++i) {
        cin >> original[i];
    }
    for (ll i = 0; i < n; ++i) {
        reversed[i] = original[n-1-i];
    }
    dp_solution(n, reversed);
    return 0;
}
