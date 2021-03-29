#include <iostream>
#include <vector>

using std::vector;
using std::cin;
using std::cout;

typedef long long ll;
ll mod = 999999937;

class matrix{
    vector<vector<ll>> array2;
    ll n, m;
public:
    matrix(const vector<vector<ll>>& vector1) : n(vector1.size()), m(vector1[0].size()) {
        array2.resize(n);
        for (ll i = 0; i < n; ++i) {
            array2[i].resize(m);
            for (ll j = 0; j < m; ++j) {
                array2[i][j] = vector1[i][j] % mod;
            }
        }
    }
    matrix(const matrix& matrix1) : n(matrix1.array2.size()), m(matrix1.array2[0].size()) {
        array2.resize(n);
        for (ll i = 0; i < n; ++i) {
            array2[i].resize(m);
            for (ll j = 0; j < m; ++j) {
                array2[i][j] = matrix1.array2[i][j] % mod;
            }
        }
    }
    matrix(ll N) : n(N), m(N) {
        array2.resize(N);
        for (ll i = 0; i < N; ++i) {
            array2[i].resize(N);
            for (ll j = 0; j < N; ++j) {
                array2[i][j] = ((i == j) ? 1 : 0);
            }
        }
    }
    matrix operator* (const matrix& matrix1) const{
        if (m != matrix1.n) {
            throw 1;
        }
        matrix copy(matrix1.array2);
        for (ll i = 0; i < n; ++i) {
            for (ll j = 0; j < matrix1.m; ++j) {
                copy.array2[i][j] = 0;
                for (ll k = 0; k < m; ++k) {
                    copy.array2[i][j] += ((array2[i][k] % mod) * (matrix1.array2[k][j] % mod) % mod);
                }
            }
        }
        return copy;
    }
    ll rows_count() const {
        return n;
    }
    ll sum() const {
        ll res = 0;
        for (ll i = 0; i < n; ++i) {
            for (ll j = 0; j < m; ++j) {
                res += (array2[i][j] % mod);
                res %= mod;
            }
        }
        return res;
    }
};

matrix bin_pow(const matrix& m1, ll n) {
    if (n == 0) {
        matrix e(m1.rows_count());
        return e;
    }
    if (n % 2 == 1) {
        return m1 * bin_pow(m1, n-1);
    }
    else {
        matrix m2 = bin_pow(m1, n / 2);
        return m2 * m2;
    }
}

void solution(ll n) {
    vector<vector<ll>> dp_step({{1,1,1,1,1}, {1,1,1,1,1}, {1,1,1,1,1}, {1,1,0,1,0}, {1,1,0,1,0}});
    matrix matrix_dp(dp_step);
    matrix start_column({{1}, {1}, {1}, {1}, {1}});
    while (n != 0) {
        cout << (bin_pow(matrix_dp, n-1) * start_column).sum() % mod << '\n';
        cin >> n;
    }
}

int main() {
    ll n;
    cin >> n;
    solution(n);
    return 0;
}
