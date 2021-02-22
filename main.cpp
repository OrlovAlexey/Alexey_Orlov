#include <iostream>
#include "string"
#include <vector>

using std::cin;
using std::cout;
using std::string;
using std::vector;

long long StringHashing(const string& s, long long t_size, long long key) {
    long long result = 0;
    for (long long i = 0; i != s.size(); ++i) {
        result = (key * result + s[i]) % t_size;
    }
    result = (result * 2 + 1) % t_size;
    return result;
}
long long HashFunction1(const string& s, long long t_size) {
    return StringHashing(s, t_size, 31);
}
long long HashFunction2(const string& s, long long t_size) {
    return StringHashing(s, t_size, 31);
}

struct Node {
    string name;
    bool state; // false == deleted
    long long value;
    Node(const string& Name, long long Value) : name(Name), state(true), value(Value) {}
};

class HashTable{
    const long long default_size = 8; // starting size
    const double resize_factor = 0.75;

    vector<Node*> arr;
    long long size;// без учета deleted
    long long capacity;
    long long size_all;// с учетом deleted

    void Resize() {
        long long prev_capacity = capacity;
        capacity *= 2;
        size_all = 0;
        size = 0;
        vector<Node*> new_arr(capacity);
        for (long long i = 0; i < capacity; ++i) {
            new_arr[i] = nullptr;
        }
        std::swap(arr, new_arr);
        for (long long i = 0; i < prev_capacity; ++i) {
            if (new_arr[i] && new_arr[i]->state) {
                Add(new_arr[i]->name, new_arr[i]->value);
            }
        }
        for (long long i = 0; i < prev_capacity; ++i) {
            if (new_arr[i]) {
                delete new_arr[i];
            }
        }
    }

    void Rehash() {
        size_all = 0;
        size = 0;
        vector<Node*> new_arr(capacity);
        for (long long i = 0; i < capacity; ++i) {
            new_arr[i] = nullptr;
        }
        std::swap(arr, new_arr);
        for (long long i = 0; i < capacity; ++i) {
            if (new_arr[i] && new_arr[i]->state)
                Add(new_arr[i]->name, new_arr[i]->value);
        }
        for (long long i = 0; i < capacity; ++i) {
            if (new_arr[i]) {
                delete new_arr[i];
            }
        }
    }

public:
    HashTable() {
        capacity = default_size;
        size = 0;
        size_all = 0;
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

    bool Add(const string& name, long long value) {
        if (size + 1 > int(resize_factor * capacity)) { // если много заполнено
            Resize();
        }
        else if (size_all > 2 * size) { // если слишком много deleted
            Rehash();
        }
        long long h1 = HashFunction1(name, capacity);
        long long h2 = HashFunction2(name, capacity);
        long long first_deleted = -1;
        for (long long i = 0; arr[h1] != nullptr && i < capacity; ++i) {
            if (arr[h1]->name == name && arr[h1]->state) {
                return false;
            }
            if (!arr[h1]->state && first_deleted == -1) {
                first_deleted = h1;
            }
            h1 = (h1 + h2) % capacity;
        }
        if (first_deleted == -1) {
            arr[h1] = new Node(name, value);
            ++size_all;
        }
        else
        {
            arr[first_deleted]->name = name;
            arr[first_deleted]->state = true;
            arr[first_deleted]->value = value;
        }
        ++size;
        return true;
    }
    long long Find(const string& name) {
        long long h1 = HashFunction1(name, capacity);
        long long h2 = HashFunction2(name, capacity);
        for (long long i = 0; arr[h1] != nullptr && i < capacity; ++i) {
            if (arr[h1]->name == name && arr[h1]->state) {
                return h1;
            }
            h1 = (h1 + h2) % capacity;
        }
        return -1;
    }
    long long Change(long long index, long long value){
        /*
        long long h1 = HashFunction1(name, capacity);
        long long h2 = HashFunction2(name, capacity);
        for (long long i = 0; arr[h1] != nullptr && i < capacity; ++i) {
            if (arr[h1]->name == name && arr[h1]->state) {
                arr[h1]->value += value;
                return arr[h1]->value;
            }
            h1 = (h1 + h2) % capacity;
        }
        */
        arr[index]->value += value;
        return arr[index]->value;
    }
};


int main() {
    std::ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    HashTable h_table;
    string variable;
    long long value;
    while(cin >> variable) {
        cin >> value;
        long long index = h_table.Find(variable);
        if(index != -1){
            cout << h_table.Change(index, value) << '\n';
        }
        else{
            h_table.Add(variable, value);
            cout << value << '\n';
        }
    }
    return 0;
}
