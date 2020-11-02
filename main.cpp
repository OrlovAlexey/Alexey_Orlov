#include <iostream>
#include <algorithm>

using std::cin;
using std::cout;

class heap{
public:
    long long n = 0;
    long long* heap1 = new long long[520];

    void siftUp(long long i) {
        while ( i > 0 && heap1[i] > heap1[(i-1)/2] ) {
            std::swap(heap1[i], heap1[(i-1)/2]);
            i = (i-1) / 2;
        }
    }

    void siftDown(long long i) {
        while ( 2*i+1 <= n-1 ) {
            long long j = -1;
            if ( heap1[2*i + 1] > heap1[i] ) { j = 2*i + 1; }
            if (( 2*i + 2 <= n-1) && ( heap1[2*i + 2] > heap1[i] ) && ( j == -1 || heap1[2*i + 1] < heap1[2*i + 2] )) { j = 2*i + 2; }
            if ( j == -1 ) break;
            else {
                std::swap(heap1[i], heap1[j]);
                i = j;
            }
        }
    }

    void insert(long long x) {
        heap1[++n - 1] = x;
        siftUp(n-1);
    }

    long long getMax() {
        return heap1[0];
    }

    void extractMax() {
        heap1[0] = heap1[n-1];
        --n;
        siftDown(0);
    }

    ~heap() {
        delete[] heap1;
    }
};

int main() {
    long long n, k;
    cin >> n >> k;
    if ( n <= k ) {
        long long array[n];
        for ( long long i = 0; i < n; ++i ) {
            cin >> array[i];
        }
        std::sort(array, array+n);
        for ( long long i = 0; i < n; ++i ) {
            cout << array[i] << " ";
        }
    }
    else {
        heap Heap;
        long long temp;
        for ( long long i = 0; i < k; ++i ) {
            cin >> temp;
            Heap.insert(temp);
        }

        for ( long long i = k; i < n; ++i ) {
            cin >> temp;
            Heap.insert(temp);
            Heap.extractMax();
        }
        long long additional_array[k];
        for ( long long i = 0; i < k; ++i ) {
            additional_array[i] = Heap.getMax();
            Heap.extractMax();
        }
        for ( long long i = k - 1; i >= 0; --i ) {
            cout << additional_array[i] << " ";
        }
    }
    return 0;
}