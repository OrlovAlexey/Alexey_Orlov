#include <iostream>
#include <vector>
#include <cmath>

using std::cin;
using std::cout;
using std::string;
using std::vector;

long long alpha = 1046527;//prime
long long beta = 65537;//prime

long long UniversalHashing(long long x, long long t_size, long long p) {
    long long result = (((alpha * x) + beta) % p) % t_size;
    return (2 * result + 1) % t_size;
}
long long HashFunction1(long long x, long long t_size) {
    return UniversalHashing(x, t_size, 31019);
}
long long HashFunction2(long long x, long long t_size) {
    return UniversalHashing(x, t_size, 31033);
}

struct Node{
    long long index;
    long long value;
    Node(long long Index, long long Value) : index(Index), value(Value) {}
};

class HashTable{
    const long long default_size = 8; // starting size
    const double resize_factor = 0.75;

    vector<Node*> arr;
    long long size;
    long long capacity;

    void Resize() {
        long long prev_capacity = capacity;
        capacity *= 2;
        size = 0;
        vector<Node*> new_arr(capacity);
        for (long long i = 0; i < capacity; ++i) {
            new_arr[i] = nullptr;
        }
        std::swap(arr, new_arr);
        for (long long i = 0; i < prev_capacity; ++i) {
            if (new_arr[i]) {
                Add(new_arr[i]->index, new_arr[i]->value);
            }
        }
        for (long long i = 0; i < prev_capacity; ++i) {
            if (new_arr[i]) {
                delete new_arr[i];
            }
        }
    }

public:
    HashTable() {
        capacity = default_size;
        size = 0;
        arr.resize(capacity);
        for (long long i = 0; i < capacity; ++i) {
            arr[i] = nullptr;
        }
    }
    ~HashTable() {
        for (long long i = 0; i < capacity; ++i) {
            if (arr[i]) {
                delete arr[i];
            }
        }
    }

    void Add(long long index, long long value) {
        if (size + 1 > int(resize_factor * capacity)) { // если много заполнено
            //cout << resize_factor * capacity << '\t';
            Resize();
        }
        long long h1 = HashFunction1(value, capacity);
        long long h2 = HashFunction2(value, capacity);
        for (long long i = 0; arr[h1] != nullptr && i < capacity; ++i) {
            h1 = (h1 + h2) % capacity;
        }
        arr[h1] = new Node(index, value);
        ++size;
        //return h1;
    }
    long long Find(long long value) {
        long long h1 = HashFunction1(value, capacity);
        long long h2 = HashFunction2(value, capacity);
        for (long long i = 0; arr[h1] != nullptr && i < capacity; ++i) {
            if (arr[h1]->value == value) {
                return h1;
            }
            h1 = (h1 + h2) % capacity;
        }
        return -1;
    }
    long long Get(long long index1, long long index2) {
        return std::abs(arr[index1]->index - arr[index2]->index);
    }
    void Change(long long index1, long long index2){
        long long temp = arr[index1]->index;
        arr[index1]->index = arr[index2]->index;
        arr[index2]->index = temp;
    }
};

int main() {
    HashTable h_table;
    int n;
    cin >> n;
    long long x, y;
    for (int i = 0; i < n; ++i) {
        cin >> x >> y;
        long long x_index = h_table.Find(x);
        if (x_index == -1) {
            h_table.Add(x, x);
        }
        long long y_index = h_table.Find(y);
        if (y_index == -1) {
            h_table.Add(y, y);
        }
        x_index = h_table.Find(x);
        y_index = h_table.Find(y);
        cout << h_table.Get(x_index, y_index) << '\n';
        h_table.Change(x_index, y_index);
    }
    return 0;
}
