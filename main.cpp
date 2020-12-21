#include <iostream>
#include <vector>
#include <algorithm>

using std::cin;
using std::cout;
using std::min;
using std::max;
using std::pair;
using std::vector;

typedef long long ll;

ll log(ll n) {
    if (n == 1 || n == 0) {
        return 0;
    }
    else {
        return log(n / 2) +1;
    }
}// двоичный логарифм эн

class Sparse_table {
    vector<vector<pair<pair<ll, ll>, pair<ll, ll>>>> sparse;// минимум и его индекс + вторая статистика и ее индекс
    std::vector<ll> powers_of_two;// вектор степеней двойки, не превосходящей число i

public:
    Sparse_table(const ll array[], ll n) {
        ll logn = log(n);
        sparse.resize(logn + 1);
        for (ll i = 0; i < logn + 1; ++i) {
            sparse[i].resize(n);
        } //ресайз

        for (ll i = 0; i < n; ++i) {
            sparse[0][i].first.first = array[i];
            sparse[0][i].first.second = i;
            sparse[0][i].second.first = -1;
            sparse[0][i].second.second = i;
        }// впринципе в задаче не нужно, т.к. отрезки длины >= 2, а это для вырожденных отрезков

        for (ll i = 0; i < n; ++i) {
            ll j = min(n - 1, i + 1);
            if (array[i] < array[j]) {
                sparse[1][i].first.first = array[i];
                sparse[1][i].first.second = i;
                sparse[1][i].second.first = array[j];
                sparse[1][i].second.second = j;
            } else {
                sparse[1][i].first.first = array[j];
                sparse[1][i].first.second = j;
                sparse[1][i].second.first = array[i];
                sparse[1][i].second.second = i;
            }
        }// заполнение таблицы для отрезков длины 2

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
        }//заполнение таблицы для отрезков 2^r
    }

    void get_powers(ll n) {
        powers_of_two.resize(n+1);
        powers_of_two[2] = 1;
        for (ll len = 3; len < n + 1; ++len) {
            if (!(len & (len - 1))) {
                powers_of_two[len] = powers_of_two[len - 1] + 1;
            } else {
                powers_of_two[len] = powers_of_two[len - 1];
            }
        }
    }// чисто вспомогательная вещь, с лекции

    void request(ll l, ll r) {
        ll pwr = powers_of_two[r - l + 1];
        ll j = r - (1 << pwr);
        ll ans;
        pair<ll, ll> min1 = sparse[pwr][l - 1].first; //минимум на первом отрезке
        pair<ll, ll> rmq1 = sparse[pwr][l - 1].second; // и его вторая статистика
        pair<ll, ll> min2 = sparse[pwr][j].first; // минимум на втором отрезке
        pair<ll, ll> rmq2 = sparse[pwr][j].second; // и его вторая статистика

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
        }// довольно сложная логика вычисления второй статистики на объединении отрезков

        cout << ans << '\n';
    }
};

int main() {
    ll n, m;
    cin >> n >> m;
    ll a[n];
    for (ll i = 0; i < n; ++i) {
        cin >> a[i];
    }
    Sparse_table sparse_t(a, n);
    sparse_t.get_powers(n);

    ll l, r;
    for (ll i = 0; i < m; ++i) {
        cin >> l >> r;
        sparse_t.request(l, r);
    }
    return 0;
}
