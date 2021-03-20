#include <iostream>
#include "string"
#include <vector>
#include <fstream>
#include <cstring>

using std::string;
using std::vector;

long long PolynomialStringHashing(const string& s, long long t_size, long long key) { //метод Горнера
    long long result = 0;
    for (long long i = 0; i < s.size(); ++i) {
        result = (key * result + s[i]) % t_size;
    }
    result = (result * 2 + 1) % t_size;
    return result;
}
long long HashFunction1(const string& s, long long t_size) {
    return PolynomialStringHashing(s, t_size, 31);
}
long long HashFunction2(const string& s, long long t_size) {
    return PolynomialStringHashing(s, t_size, 31);
}

struct Node {
    string name;
    bool state; // false == deleted
    string value;
    Node(const string& Name, string Value) : name(Name), state(true), value(Value) {}
};

class HashTable{
    static const long long default_size = 8; // starting size
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

    bool Add(const string& name, const string& value) {
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
                arr[h1]->value = value;
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
    bool Delete(const string& name) {
        long long h1 = HashFunction1(name, capacity);
        long long h2 = HashFunction2(name, capacity);
        for (long long i = 0; arr[h1] != nullptr && i < capacity; ++i) {
            if (arr[h1]->name == name && arr[h1]->state) {
                arr[h1]->state = false;
                --size;
                return true;
            }
            h1 = (h1 + h2) % capacity;
        }
        return false;
    }
    string Find(const string& name) {
        long long h1 = HashFunction1(name, capacity);
        long long h2 = HashFunction2(name, capacity);
        for (long long i = 0; arr[h1] != nullptr && i < capacity; ++i) {
            if (arr[h1]->name == name && arr[h1]->state) {
                return arr[h1]->value;
            }
            h1 = (h1 + h2) % capacity;
        }
        return "none";
    }
};

int main() {
    std::ifstream inf("map.in");
    std::ofstream outf("map.out");
    std::ios::sync_with_stdio(false);
    HashTable h_table;
    string command;
    string s1, s2;
    while (inf >> command) {
        inf >> s1;
        if (strcmp(command[0], "put")) {
            inf >> s2;
            h_table.Add(s1, s2);
        }
        if (strcmp(command[0], "delete")) {
            h_table.Delete(s1);
        }
        if (strcmp(command[0], "get")) {
            outf << h_table.Find(s1) << '\n';
        }
    }
    return 0;
}
