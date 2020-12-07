#include <iostream>
#include <set>
#include <map>

using std::map;
using std::set;
using std::cin;
using std::cout;

typedef unsigned long long ll;

class multiset1 {
public:
    map<ll, set<ll>> v;
    map<ll, set<ll>> mp;

    multiset1() = default;

    void add_element(ll e, ll s) {
        v[s].insert(e);
        mp[e].insert(s);
    }
    void delete_element(ll e, ll s) {
        v[s].erase(e);
        mp[e].erase(s);
    }
    void clear_set(ll s) {
        for (auto& item: v[s]) {
            mp[item].erase(s);
        }
        v[s].clear();
    }
    void listset(ll s) {
        if (v[s].empty()) {
            cout << -1 << '\n';
        }
        else {
            for (auto &item : v[s]) {
                cout << item << " ";
            }
            cout << '\n';
        }
    }
    void listsetsof(ll e) {
        ll flag = 0;
        for (auto &item : mp[e]) {
            cout << item << " ";
            flag++;
        }
        if (flag == 0) {
            cout << -1;
        }
        cout << '\n';
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    ll n, m, k;
    cin >> n >> m >> k;
    multiset1 main_set;
    for (ll i = 0; i < k; ++i) {
        std::string str;
        cin >> str;
        ll e, s;
        if (str == "ADD") {
            cin >> e >> s;
            main_set.add_element(e, s);

        }
        else if (str == "DELETE") {
            cin >> e >> s;
            main_set.delete_element(e, s);
        }
        else if (str == "CLEAR") {
            cin >> s;
            main_set.clear_set(s);
        }
        else if (str == "LISTSET") {
            cin >> s;
            main_set.listset(s);
        }
        else if (str == "LISTSETSOF") {
            cin >> e;
            main_set.listsetsof(e);
        }
    }
    return 0;
}
