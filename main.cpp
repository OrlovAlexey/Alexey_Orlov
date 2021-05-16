#include <iostream>
#include <vector>
#include <set>

using std::cin;
using std::cout;
using std::vector;
using std::pair;
using std::set;
using ll = long long;

const ll inf = 1000000000000000000;

void solution(ll n, ll s, const vector<vector<pair<ll, ll>>>& graph, vector<ll>& dist, vector<bool>& used, set<pair<ll, ll>>& set1, vector<set<pair<ll, ll>>::iterator>& iterators) {
    for (long long i = 0; i < n; ++i) {
        iterators[i] = set1.insert({((i == s - 1) ? 0 : inf), i}).first;
    }
    for (ll i = 0; i < n; ++i) {
        if (set1.begin() == set1.end())
            break;
        ll v = set1.begin()->second;
        for (auto edge : graph[v]) {
            if (!used[v]) {
                if (dist[v] + edge.second < dist[edge.first]) {
                    auto iter = iterators[edge.first];
                    set1.erase(iter);
                    iterators[edge.first] = set1.insert({dist[v] + edge.second, edge.first}).first;
                }
                dist[edge.first] = std::min(dist[edge.first], dist[v] + edge.second);
            }
        }
        used[v] = true;
        set1.erase(iterators[v]);
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    ll n, m;
    cin >> n >> m;
    ll s, t;
    cin >> s >> t;
    vector<vector<pair<ll, ll>>> graph(n);
    ll v1, v2, w;
    for (ll i = 0; i < m; ++i) {
        cin >> v1 >> v2 >> w;
        graph[v1 - 1].push_back({v2 - 1, w});
        graph[v2 - 1].push_back({v1 - 1, w});
    } // input
    vector<ll> dist(n, inf);
    dist[s - 1] = 0;
    vector<bool> used(n,false);
    set<pair<ll, ll>> set1;
    vector<set<pair<ll, ll>>::iterator> iterators(n);

    solution(n, s, graph, dist, used, set1, iterators);

    if (dist[t - 1] != inf) {
        cout << dist[t - 1] << '\n';
    }
    else {
        cout << -1 << '\n';
    } // answer
    return 0;
}
