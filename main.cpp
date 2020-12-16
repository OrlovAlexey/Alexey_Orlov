#include <iostream>
#include <bits/stdc++.h>

class heap{
public:
    long long n = 0;
    long long* heap1 = new long long[1000000];
    long long* indexes = new long long[1000000];

    void siftUp(long long i) {
        while ( i > 0 && heap1[i] < heap1[(i-1)/2] ) {
            std::swap(heap1[i], heap1[(i-1)/2]);
            std::swap(indexes[i], indexes[(i-1)/2]);
            i = (i-1) / 2;
        }
    }

    void siftDown(long long i) {
        while ( 2*i + 1 <= n-1 ) {
            long long j = -1;
            if ( heap1[2*i + 1] < heap1[i] ) { j = 2*i + 1; }
            if (( 2*i + 2 <= n-1 ) && ( heap1[2*i + 2] < heap1[i] ) && ( j==-1 || heap1[2*i + 1] > heap1[2*i + 2] )) { j = 2*i + 2; }
            if ( j == -1 ) break;
            else {
                std::swap(heap1[i], heap1[j]);
                std::swap(indexes[i], indexes[j]);
                i = j;
            }
        }
    }

    void insert(long long x) {
        heap1[++n - 1] = x;
        siftUp(n-1);
    }

    long long getMin() {
       return heap1[0];
    }

    void extractMin() {
        heap1[0] = heap1[n-1];
        indexes[0] = indexes[n-1];
        --n;
        siftDown(0);
    }

    void decreaseKey(long long ptr, long long delta) {
        heap1[ptr] -= delta;
        siftUp(ptr);
    }

    ~heap() {
        delete[] heap1;
    }
};

long long binary_search(long long a[], long long left, long long right, long long value) {
    long long middle;
    while(true) {
        middle = (left + right) / 2;
        if (value < a[middle]) {
            right = middle - 1;
        }
        else if (value > a[middle]) {
            left = middle + 1;
        }
        else {
            return middle;
        }
        if (left > right) {
            return 0;
        }
    }
}

int main() {
    heap Heap;
    long long q;
    scanf("%lld", &q);
    for ( long long i = 0; i < q; ++i ) {
        char input[12];
        scanf("%11s", input);
        if ( strcmp(input, "insert") == 0 ) {
            long long k;
            scanf("%lld", &k);
            Heap.indexes[Heap.n] = i;
            Heap.insert(k);
        }
        else if ( strcmp(input, "decreaseKey") == 0 ) {
            long long p, d;
            scanf("%lld%lld", &p, &d);
            long long t;
            p--;
//            for ( long long i = 0; i < Heap.n; ++i ) {
//                if ( Heap.indexes[i] == p ) {
//                    t = i;
//                    break;
//                }
//            }// заменил цикл на двоичный поиск
            t =  binary_search(Heap.indexes, 0, Heap.n, p);
            Heap.decreaseKey(t, d);
        }
        else if ( strcmp(input, "extractMin") == 0 ) {
            Heap.extractMin();
        }
        else if ( strcmp(input, "getMin") == 0 ) {
            long long r = Heap.getMin();
            printf("%lld \n", r);
        }
    }
    return 0;
}

