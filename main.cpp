#include <iostream>
using namespace std;
int power(int x, int p){
    if (p == 0)
        return 1;
    else{
        for (int i=0; i<p; i++){
            x*=x;
        }
    }
    return x;
}
int main() {
    long double e = 2.7182818284590452353602875;
    long double a;
    int n;
    cin >> n;
    a = power(10, n);
    int b = e*a;
    long double c = b;
    c = c/a;
    cout << c << endl;
    return 0;
}
