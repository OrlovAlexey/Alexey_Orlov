#include <iostream>
#include <vector>
using std::cin;
using std::cout;
using std::vector;
typedef long long ll;

struct HashTable{
    vector<vector<bool>> table;
    ll size = 0;
    ll W, H;
    HashTable(ll width, ll height) : W(width), H(height) {
        table.resize(width);
        for (ll i = 0; i < width; ++i) {
            table[i].resize(height);
        }
        for (ll i = 0; i < width; ++i) {
            for (ll j = 0; j < height; ++j) {
                table[i][j] = false;
            }
        }
    }
    void Add(ll x, ll y) {
        if (x >= 0 && x < W && y >=0 && y < H) {
            if (!table[x][y]) {
                table[x][y] = true;
                ++size;
            }
        }
        if (x+1 < W && y >= 0 && y < H) {
            if (!table[x+1][y]){
                table[x+1][y] = true;
                ++size;
            }
        }
        if (x-1 >= 0 && y >= 0 && y < H) {
            if (!table[x-1][y]) {
                table[x-1][y] = true;
                ++size;
            }
        }
        if (x >= 0 && x < W && y+1 < H) {
            if (!table[x][y+1]) {
                table[x][y+1] = true;
                ++size;
            }
        }
        if (x >= 0 && x < W && y-1 >= 0) {
            if (!table[x][y-1]) {
                table[x][y-1] = true;
                ++size;
            }
        }
    }
    bool Check() {
        return (size == (W * H));
    }
};
int main() {
    std::ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    ll w, h, n;
    cin >> w >> h >> n;
    if (w * h > 5 * n || w * h >= 5'000'000) {
        cout << "No";
    }
    else {
        HashTable h_table(w, h);
        ll x, y;
        for(ll i = 0; i < n; ++i) {
            cin >> x >> y;
            h_table.Add(x-1, y-1);
        }
        if (h_table.Check())
            cout << "Yes";
        else
            cout << "No";
    }
    return 0;
}
