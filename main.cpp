#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>

using std::cin;
using std::cout;
using std::vector;
using std::queue;
using std::unordered_map;
using std::unordered_set;
using str = std::string;

long long input_perm(long long n) {
    str permutation;
    long long digit;
    for (long long i = 0; i < n; ++i) {
        cin >> digit;
        --digit;
        permutation += std::to_string(digit);
    }
    return std::stoll(permutation);
}

inline long long power_of_10(short z) {
    long long x = 1;
    for (short i = 0; i < z; ++i) {
        x *= 10;
    }
    return x;
}

unordered_set<long long> get_perms(long long n, long long perm) {
    unordered_set<long long> sons;
    vector<short> original;
    short sh;
    for (short z = 0; z < n; ++z) {
        sh = (perm % power_of_10(z + 1)) / power_of_10(z);
        original.push_back(sh);
    }
    vector<short> temp_perm;
    for (long long i = 0; i < n; ++i) {
        for (long long j = i + 1; j < n; ++j) {
            temp_perm = original;
            for (long long k = 0; k < (j - i) / 2 + 1; ++k) {
                std::swap(temp_perm[i + k], temp_perm[j - k]);
            }
            long long new_perm = 0;
            for (auto it = temp_perm.rbegin(); it != temp_perm.rend(); ++it) {
                new_perm += *it;
                new_perm *= 10;
            }
            new_perm /= 10;
            sons.insert(new_perm);
        }
    }
    return sons;
}

long long double_bfs(long long n, long long s, long long t, unordered_map<long long, long long>& dist1, unordered_map<long long, long long>& dist2, queue<long long>& layer1, queue<long long> layer2){
    dist1[s] = 0;
    dist2[t] = 0;
    layer1.push(s);
    layer2.push(t);
    while(!layer1.empty() && !layer2.empty()) {
        long long v;
        long long size1 = layer1.size();
        for (long long z = 0; z < size1; ++z) {
            v = layer1.front();
            layer1.pop();
            for (long long to : get_perms(n, v)) {
                if (dist1.find(to) != dist1.end()) {
                    continue;
                }
                dist1[to] = dist1[v] + 1;
                layer1.push(to);
                if (dist2.find(to) != dist2.end()) {
                    return dist1[to] + dist2[to];
                }
            }
        }
        long long size2 = layer2.size();
        for (long z = 0; z < size2; ++z) {
            v = layer2.front();
            layer2.pop();
            for (long long to : get_perms(n, v)) {
                if (dist2.find(to) != dist2.end()) {
                    continue;
                }
                dist2[to] = dist2[v] + 1;
                layer2.push(to);
                if (dist1.find(to) != dist1.end()) {
                    return dist1[to] + dist2[to];
                }
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cin.tie(nullptr);
    long long n;
    cin >> n;
    unordered_map<long long, long long> dist1;
    unordered_map<long long, long long> dist2;
    queue<long long> layer1;
    queue<long long> layer2;
    long long start = input_perm(n);
    long long finish = input_perm(n);
    if (start == finish) {
        cout << 0 << '\n';
        return 0;
    }
    cout << double_bfs(n, start, finish, dist1, dist2, layer1, layer2);
    return 0;
}
