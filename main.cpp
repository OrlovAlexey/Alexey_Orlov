#include <bits/stdc++.h>
#include <iomanip>

using std::cin;
using std::cout;
using std::ios_base;
using std::deque;

typedef long long ll;

void type_plus(deque<ll>& left, deque<ll>& right){
    ll num;
    cin >> num;
    right.push_back(num);
    if (left.size() < right.size()) {
        left.push_back(right.front());
        right.pop_front();
    }
}

void type_multiply(deque<ll>& left, deque<ll>& right){
    ll num;
    cin >> num;
    if (left.size() > right.size()) {
        right.push_front(num);
    }
    else {
        left.push_back(num);
    }
}

void type_subtract(deque<ll>& left, deque<ll>& right){
    cout << left.front() << '\n';
    left.pop_front();
    if (left.size() < right.size()) {
        left.push_back(right.front());
        right.pop_front();
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    ll n;
    cin >> n;
    char type;
    deque<ll> left, right;
    for (ll i = 0; i < n; ++i) {
        cin >> type;
        if (type == '+') {
            type_plus(left, right);
        }
        else if (type == '*') {
            type_multiply(left, right);
        }
        else {
            type_subtract(left, right);
        }
    }
}

