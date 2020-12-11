#include <iostream>
#include <string>

using std::string;
using std::cin;
using std::cout;
using std::pair;

typedef long long ll;


class splay_tree {
private:
    splay_tree *left = nullptr, *right = nullptr;
    ll key = 0, sum = 0;
    bool empty = true;
    void splay(ll, bool);
    pair<splay_tree, splay_tree> split(ll);
public:
    void set_sum();
    void new_vertex(ll);
    bool exists(ll);
    void add(ll);
    ll get_sum(ll, ll);
    ll find_max();
    ll next_or_equal(ll);
    void merge(splay_tree);
    void small_left_rotate();
    void small_right_rotate();
    ~splay_tree();
};

void splay_tree::splay(ll x, bool is_root = true) {

    if (left != nullptr && x < key) {
        left->splay(x, false);
    }
    if (right != nullptr && x > key) {
        right->splay(x, false);
    }

    if (!empty && x != key) {

        if (is_root) {
            if ((right != nullptr) && (x == right->key)) {
                small_left_rotate();
            }
            if ((left != nullptr) && (x == left->key)) {
                small_right_rotate();
            }
        }

        if ((left != nullptr) && (left->left != nullptr) && (left->left->key == x)) {
            small_right_rotate();
            small_right_rotate();
        }// zig-zig right

        if ((right != nullptr) && (right->right != nullptr) && (right->right->key == x)) {
            small_left_rotate();
            small_left_rotate();
        }// zig-zig left

        if ((left != nullptr) && (left->right != nullptr) && (left->right->key == x)) {
            left->small_left_rotate();
            small_right_rotate();
        }// zig-zag left-right

        if (right != nullptr && right->left != nullptr && right->left->key == x){
            right->small_right_rotate();
            small_left_rotate();
        }// zig-zag right-left
    }
}

pair<splay_tree, splay_tree> splay_tree::split(ll x) {
    ll pivot = next_or_equal(x);
    pair<splay_tree, splay_tree> result;

    if (pivot == 1000000001) {
        splay(find_max(),true);
        result.first = *this;
        result.second = *(new splay_tree());
    }
    else {
        splay(pivot, true);
        if (left != nullptr) {
            result.first = *left;
        }
        else {
            result.first = *(new splay_tree());
        }
        result.second = *this;
        if(!result.second.empty) {
            result.second.left = new splay_tree();
        }
        result.second.set_sum();
    }
    return result;
}

void splay_tree::merge(splay_tree tree) {
    splay(find_max(), true);
    *right = tree;
    this->set_sum();
}

void splay_tree::set_sum() {
    if(empty) {
        return;
    }
    sum = key;
    sum += left->sum;
    sum += right->sum;
}

void splay_tree::new_vertex(ll x) {
    empty = false;
    key = x;
    sum = key;
    left = new splay_tree();
    right = new splay_tree();
}

bool splay_tree::exists(ll x) {
    if (empty) {
        return false;
    }
    if (x > key) {
        return right->exists(x);
    }
    if (x < key) {
        return left->exists(x);
    }
    return true;
}

void splay_tree::add(ll x){
    if (x == 0) {
        return;
    }
    pair<splay_tree, splay_tree> splitted_tree = split(x);
    if (!splitted_tree.second.empty && splitted_tree.second.key == x) {
        *this = splitted_tree.first;
        merge(splitted_tree.second);
        splay(x);
    }
    else {
        new_vertex(x);
        *left = splitted_tree.first;
        *right = splitted_tree.second;
        set_sum();
    }
}

ll splay_tree::get_sum(ll l, ll r){
    ll result = 0;
    result += split(r + 1).first.sum;
    result -= split(l).first.sum;
    return result;
}

ll splay_tree::find_max(){
    if(!right || right->empty) {
        return key;
    }
    return right->find_max();
}

ll splay_tree::next_or_equal(ll x){
    if(empty) {
        return 1000000001;
    }
    if(x > key) {
        if(right->empty) {
            return 1000000001;
        }
        else {
            return right->next_or_equal(x);
        }
    }
    else {
        if(left->empty) {
            return key;
        }
        else {
            return std::min(left->next_or_equal(x), key);
        }
    }
}

void splay_tree::small_left_rotate() {
    splay_tree root = *right;
    *right = *root.left;
    *root.left = *this;
    *this = root;
    left->set_sum();
    set_sum();
}

void splay_tree::small_right_rotate() {
    splay_tree root = *left;
    *left = *root.right;
    *root.right = *this;
    *this = root;
    right->set_sum();
    set_sum();
}

splay_tree::~splay_tree() {
    delete left;
    delete right;
}


int main() {
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    splay_tree tree;
    ll n;
    string s;
    bool flag = false;
    ll y;
    ll x, l, r;

    cin >> n;
    for (ll i = 0; i < n; ++i) {
        cin >> s;
        if (s[0] == '+') {
            cin >> x;
            if (flag) {
                x = (x + y) % 1000000000;
                flag = false;
            }
            tree.add(x);
        }
        if (s[0] == '?') {
            cin >> l >> r;
            flag = true;
            y = tree.get_sum(l, r);
            cout << y << '\n';
        }
    }

    return 0;
}
