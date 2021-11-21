#include <iostream>

using std::string;
using std::cin;
using std::cout;

int ALPHABET_SIZE = 256;

int first_iteration(int n, int cnt[], const string& s, int c[], int p[]) {
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        cnt[i] = 0;
    }
    for (int i = 0; i < n; ++i) {
        ++cnt[s[i]];
    }
    for (int i = 1; i < ALPHABET_SIZE; ++i) {
        cnt[i] += cnt[i-1];
    }
    for (int i = n - 1; i >= 0; --i) {
        p[--cnt[s[i]]] = i;
    }
    c[p[0]] = 0;
    int classes = 1;
    for (int i = 1; i < n; ++i) {
        if (s[p[i]] != s[p[i-1]]) {
            classes++;
        }
        c[p[i]] = classes - 1;
    }
    return classes;
}

void next_iteration(int k, int n, int& classes, int c[], int p[]) {
    int new_p[n], new_c[n];
    for (int i = 0; i < n; ++i) {
        new_p[i] = (p[i] + n - (1 << k)) % n;
    }
    int cnt[ALPHABET_SIZE];
    for (int i = 0; i < classes; ++i) {
        cnt[i] = 0;
    }
    for (int i = 0; i < n; ++i) {
        ++cnt[c[new_p[i]]];
    }
    for (int i = 1; i < classes; ++i) {
        cnt[i] += cnt[i-1];
    }
    for (int i = n - 1; i >= 0; --i) {
        p[--cnt[c[new_p[i]]]] = new_p[i];
    }
    new_c[p[0]] = 0;
    classes = 1;
    for (int i = 1; i < n; ++i) {
        if (c[p[i]] != c[p[i-1]] || c[(p[i] + (1 << k)) % n] != c[(p[i-1] + (1 << k)) % n]) {
            ++classes;
        }
        new_c[p[i]] = classes - 1;
    }
    for (int i = 0; i < n; ++i) {
        c[i] = new_c[i];
    }
}

void out(int n, int p[]) {
    for (int i = 1; i < n; ++i) {
        cout << p[i] + 1 << " ";
    } cout << '\n';
}

int main() {
    string s;
    cin >> s;
    s = s + '#';
    int n = (int)s.size();
    int cnt[ALPHABET_SIZE];
    int c[n];
    int p[n];
    int classes = first_iteration(n, cnt, s, c, p);

    for (int k = 0; (1 << k) < n; ++k) {
        next_iteration(k, n, classes, c, p);
    }
    out(n, p);
    return 0;
}
