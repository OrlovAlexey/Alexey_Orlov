#include <iostream>
#include <vector>

using std::vector;
using std::cin;
using std::cout;

//a,b,c?? Даже если хочется называть короткими именнами переменные, на входе они должны быть нормальными
// потом ссылками переименуешь. Ну или комментариями перед функцией описывай что происходит
vector<int> Merge(vector<int> &b, vector<int> &c, long long &count){
    vector<int> a;
    int i = 0;
    int j = 0;
    while(i < b.size() && j < c.size()) {
        if(b[i] <= c[j]) {
            a.push_back(b[i]);
            ++i;
        }
        else {
            a.push_back(c[j]);
            ++j;
            count += b.size() - i;
        }
    }
    while(i < b.size()) {
        a.push_back(b[i]);
        ++i;
    }
    while(j < c.size()) {
        a.push_back(c[j]);
        ++j;
    }
    return a;
}

vector<int> Merge_sort(vector<int> &a, long long &count){
    int size_a = a.size();
    if (size_a == 1) return a;

    vector<int> b(size_a/2);
    for(int i = 0; i < size_a/2; ++i){
        b[i] = a[i];
    }
    vector<int> c(size_a/2);// Создавать массивы для каждого рекурсивного вызова плохая идея, так как много памяти берёшь почём зря
    // можно было бы создать их заранее и переиспользовать для каждого вызова рекурсии. Создать в функции обёртке только, не в main
    for(int i = 0; i < size_a/2; ++i) {
        c[i] = a[size_a/2 + i];
    }
    b = Merge_sort(b, count);
    c = Merge_sort(c, count);
    return Merge(b, c, count);

}

int main() {
    freopen("inverse.in", "r", stdin);
    freopen("inverse.out", "w", stdout);
    int n;
    cin >> n;
    int k = 0;
    long long ans = 0;
    while(true){
        if((n >> k) == 1){
            break;
        }
        ++k;
    }
    int bias = (1 << (k+1)) - n;
    int r = 1 << (++k);
    vector<int> v(r);
    for(int i = bias; i < r; ++i) {
        cin >> v[i];
    }
    v = Merge_sort(v, ans);
    cout << ans << std::endl;
    return 0;
}
