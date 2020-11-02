#include <iostream>
#include <vector>

using std::vector;
using std::cin;
using std::cout;

vector<unsigned long long> stable_sort (vector<unsigned long long>& array, unsigned long long& digit) {
    long long base = 256;
    vector<long long> cnt(base);

    for ( long long i = 0; i < base; ++i ) {
        cnt[i] = 0;
    }

    for ( long long i = 0; i < array.size(); ++i ) {
        ++cnt[ ( array[i] >> (8 * (digit - 1)) ) % 256 ];
    }

    for ( long long i = 1; i < base; ++i ) {
        cnt[i] += cnt[i - 1];
    }

    vector<unsigned long long> ans(array.size());

    for ( long long i = array.size() - 1; i >= 0; --i ) {
        ans[ --cnt[ (array[i] >> (8 * (digit - 1)) ) % 256 ] ] = array[i];
    }
    return ans;
}

int main() {
    std::cin.tie(0);
    std::cout.tie(0);
    std::ios_base::sync_with_stdio(false);
    long long n;
    cin >> n;
    vector<unsigned long long> array;

    for ( long long i = 0; i < n; ++i ) {
        unsigned long long temp;
        cin >> temp;
        array.push_back(temp);
    }

    for ( unsigned long long i = 1; i < 9; ++i ) {
        array = stable_sort(array, i);
    }

    for ( long long i = 0; i < n; ++i ) {
        cout << array[i] << " ";
    }
    return 0;
}
