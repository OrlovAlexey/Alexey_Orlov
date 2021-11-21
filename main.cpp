#include <iostream>
#include <complex>
#include <vector>

#define M_PI 3.14159265358979323846

using std::complex;
using std::vector;
using std::max;
using std::cin;
using std::cout;
using cmplx = complex<double>;

void fft(vector<cmplx>& a, bool is_invert) {
    int n = (int)a.size();
    if (n == 1) {
        return;
    }

    vector<cmplx> a_even(n / 2), a_odd(n / 2);
    for (int i = 0; 2 * i < n; ++i) {
        a_even[i] = a[2 * i];
        a_odd[i] = a[2 * i + 1];
    }
    fft(a_even, is_invert);
    fft(a_odd, is_invert);

    double angle = 2.0 * M_PI / n;
    if (is_invert) {
        angle *= -1;
    }
    cmplx w_n(1.0, 0.0),  w(cos(angle), sin(angle));
    for (int i = 0; i < (n / 2); ++i) {
        a[i] = a_even[i] + (w_n * a_odd[i]);
        a[i + (n / 2)] = a_even[i] - (w_n * a_odd[i]);
        if (is_invert) { // dividing on n = 2^k
            a[i] /= 2;
            a[i + (n / 2)] /= 2;
        }
        w_n *= w;
    }
}

void multiply(vector<cmplx>& a, vector<cmplx>& b, vector<int>& res) {
    size_t n = 1;
    while (n < max(a.size(), b.size())) {
        n *= 2;
    }
    n *= 2;
    a.resize(n), b.resize(n);

    fft(a, false),  fft(b, false);
    for (size_t i = 0; i < n; ++i) {
        a[i] *= b[i];
    }
    fft(a, true);

    res.resize(n);
    for (size_t i = 0; i < n; ++i) {
        if (a[i].real() + 0.5 > 0) {
            res[i] = (int)(a[i].real() + 0.5);
        }
        else {
            res[i] = (int)(a[i].real() - 0.5);
        }
    }
}

int main() {
    int n, m;
    cin >> n;
    vector<cmplx> a(n + 1);
    int temp;
    for (int i = 0; i <= n; ++i) {
        cin >> temp;
        a[i] = temp;
    }
    cin >> m;
    vector<cmplx> b(m + 1);
    for (int i = 0; i <= m; ++i) {
        cin >> temp;
        b[i] = temp;
    }
    vector<int> result;
    multiply(a, b, result);
    cout << n + m << " ";
    for(int i = 0; i <= n + m; ++i) {
        cout << result[i] << " ";
    }
    return 0;
}
