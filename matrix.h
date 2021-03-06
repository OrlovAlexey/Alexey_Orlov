#include <iostream>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>


using std::string;
using std::cin;
using std::cout;
using std::vector;
using std::max;
using std::min;
using std::ostream;
using std::istream;

class BigInteger{
private:
    static const int base = 10000;
    vector<int> num;
    bool sign = true;

public:
    BigInteger() {
        num.clear();
        num.push_back(0);
        sign = true;
    }
    BigInteger(const char* c) {
        string s(c);
        if (s.length() == 0) {
            num.clear();
            sign = true;
        }
        else {
            if (s[0] == '-') {
                sign = false;
                s = s.substr(1);
            }
            else {
                sign = true;
            }
            for (long long i = s.length(); i > 0; i -= 4){
                if (i < 4)
                    num.push_back(std::stoi(s.substr(0, i)));
                else
                    num.push_back(std::stoi(s.substr(i - 4, 4)));
            }
        }
        this->clearzeros();
    }
    BigInteger(int x) {
        if (x == 2 || x == 0) {
            sign = true;
            num.push_back(x);
        }
        else {
            if (x < 0) {
                x = -x;
                sign = false;
            }
            if (x == 0) {
                num.push_back(x);
            }

            while (x > 0) {
                num.push_back(x % base);
                x /= base;
            }

            this->clearzeros();
        }
    }
    BigInteger(string s) {
        if(s.length() == 0) {
            num.clear();
            sign = true;
        }
        else {
            if(s[0]=='-') {
                sign = false;
                s = s.substr(1);
            }
            else {
                sign = true;
            }
            for (long long i = s.length(); i > 0; i-=4){
                if (i < 4)
                    num.push_back(atoi(s.substr(0, i).c_str()));
                else
                    num.push_back(atoi(s.substr(i - 4, 4).c_str()));
            }
        }
        this->clearzeros();
    }


    BigInteger operator- () const {
        BigInteger copy = *this;
        copy.sign = !copy.sign;
        return copy;
    }
    BigInteger& operator+= (const BigInteger& b) {
        if (!this->sign) {
            if(!b.sign) {
                return (this->changessign() += (-b)).changessign();
            }
            else {
                return (this->changessign() -= b).changessign();
            }
        }// проверка на знаки
        else if (!b.sign) {
            *this -= (-b);
            return *this;
        }
        else {
            int over_digit = 0;
            size_t n1 = num.size();
            size_t n2 = b.num.size();
            for (size_t i = 0; i < max(n1, n2); ++i) {
                if (n1 <= i && i < n2) {
                    num.push_back(0);
                }// выравнивает число, к которому прибавляем

                int copy = num[i];
                if (n2 <= i && i < n1) {
                    num[i] = (num[i] + over_digit) % base;
                    over_digit = (copy + over_digit) / base;
                }
                else {
                    num[i] = (num[i] + b.num[i] + over_digit) % base;
                    over_digit = (copy + b.num[i] + over_digit) / base;
                }// прибавление

                if (i == max(n1, n2) - 1 && over_digit != 0) {
                    num.push_back(over_digit);
                }// конечный разряд
            }
            return *this;
        }
    }
    BigInteger& operator-= (const BigInteger& b) {
        if (!this->sign) {
            if(!b.sign) {
                return (this->changessign() -= (-b)).changessign();
            }
            else {
                return (this->changessign() += b).changessign();
            }
        }
        else if(!b.sign) {
            *this += (-b);
            return *this;
        }
        else {
            if (*this < b) {
                BigInteger temp = *this;
                *this = b;
                return (*this -= temp).changessign();
            }
            else {
                bool over_digit = 0;
                size_t n1 = num.size();
                size_t n2 = b.num.size();
                for (size_t i = 0; i < max(n1, n2); ++i) {
                    if (i < n2) {
                        num[i] -= static_cast<int>(over_digit) + b.num[i];
                    }
                    else {
                        num[i] -= static_cast<int>(over_digit);
                    }// уменьшение разряда

                    if (num[i] < 0) {
                        over_digit = true;
                        num[i] += base;
                    }
                    else {
                        over_digit = false;
                    }// проверка если разряд оказался отрицательным, прибавление к нему 10000 в этом случае®®
                }
                this->clearzeros();
                return *this;
            }
        }
    }
    bool operator== (const BigInteger& b) const {
        if (b.num.size() != num.size()) return false;
        for (size_t i = 0; i < num.size(); ++i) {
            if (num[i] != b.num[i]){
                return false;
            }
        }
        if (sign == b.sign) {
            return true;
        }
        else {
            return false;
        }
    }
    bool operator!= (const BigInteger& b) const {
        return !(*this == b);
    }
    bool operator<= (const BigInteger& b) const {
        return ((*this < b) || (*this == b));
    }
    bool operator>= (const BigInteger& b) const {
        return ((*this > b) || (*this == b));
    }
    BigInteger& operator++ () {
        *this += 1;
        return *this;
    }
    BigInteger operator++ (int) {
        BigInteger copy = *this;
        ++(*this);
        return copy;
    }
    BigInteger& operator-- () {
        *this -= 1;
        return *this;
    }
    BigInteger operator-- (int) {
        BigInteger copy = *this;
        --(*this);
        return copy;
    }
    BigInteger& operator*= (const BigInteger& b) {
        if ((sign && b.sign) || (!sign && !b.sign)) {
            sign = true;
        }
        else {
            sign =false;
        } // правильный знак

        BigInteger copy = *this;
        num.clear();
        num.resize(copy.num.size() + b.num.size() + 1);

        for(size_t i = 0; i < copy.num.size(); ++i){
            long long over_digit = 0;
            for (size_t j = 0; j < b.num.size() || over_digit != 0; ++j){
                long long result;
                if (j < b.num.size()) {
                    result = num[i+j] + copy.num[i] * b.num[j] + over_digit;
                }// умножение каждого разряда на каждый разряд
                else {
                    result = over_digit;
                }
                over_digit = result / base; // перенос разряда
                num[i+j] = result % base;
            }
        }
        clearzeros();
        return *this;
    }
    BigInteger& operator/= (const BigInteger& b) {
        if (b == 0) {
            cout << "Divided by zero" << '\n';
            return *this;
        }//проверка на деление на ноль
        if (b.num.size() > num.size()) {
            *this = 0;
            sign = true;
            return *this;
        }
        if ((sign && b.sign) || (!sign && !b.sign)) {
            sign = true;
        }
        else {
            sign = false;
        }//проверка на знак
        BigInteger copy = *this;
        num.clear();
        BigInteger remainder = 0;
        remainder.sign = true;
        num.resize(copy.num.size());

        for (int i = copy.num.size() - 1; i >= 0; --i) {
            remainder.pwr_4();
            remainder.num[0] = copy.num[i];
            remainder.clearzeros();
            int find = 0;
            int left = 0;
            int right = base;
            while (left <= right) {
                int middle = (left + right) / 2;
                BigInteger temp = b * middle;
                temp.sign = true;
                if (temp <= remainder) {
                    find = middle;
                    left = middle + 1;
                }
                else {
                    right = middle - 1;
                } // ищем число меньше или равно частичного остатка
            }
            num[i] = find % base;
            remainder -= (b >= 0 ? b * (find) : -(b * (find)));
        }// деление столбиком
        clearzeros();
        return *this;
    }
    BigInteger& operator%= (const BigInteger& b) {
        if (b == 2) {
            int rem = num[0] % 2;
            num.clear();
            num.push_back(rem);
            return *this;
        }
        *this = *this - (*this / b) * b;
        return *this;
    }

    string toString() const {
        string s = "";
        if (!sign) {
            s += "-";
        }
        if (num.size() == 1 && num[0]== 0) {
            s.clear();
        }
        if (num.size() != 0) {
            s += std::to_string(num[num.size() - 1]);
        }
        if (num.size() > 1) {
            for (int i = num.size() - 2; i >= 0; --i) {
                if (num[i] < 10) {
                    s += "000";
                } else if (num[i] < 100) {
                    s += "00";
                } else if (num[i] < 1000) {
                    s += "0";
                }
                s += std::to_string(num[i]);
            }
        }
        if (num.size() == 0){
            s.clear();
        }
        return s;
    }
    explicit operator bool() {
        return (*this != (BigInteger)0);
    }
    //    explicit operator int() {}

    //    friend BigInteger operator+ (const BigInteger&, const BigInteger&);
    friend BigInteger operator- (const BigInteger&, const BigInteger&);
    friend BigInteger operator* (const BigInteger&, const BigInteger&);
    friend BigInteger operator/ (const BigInteger&, const BigInteger&);
    friend BigInteger operator% (const BigInteger&, const BigInteger&);
    friend bool operator< (const BigInteger&, const BigInteger&);
    friend bool operator> (const BigInteger&, const BigInteger&);
    friend istream& operator>> (istream&, BigInteger&);
    friend string asDecimal_helper(const BigInteger&, const BigInteger&, long long);
//    friend ostream& operator<< (ostream&, const BigInteger&);
private:
    BigInteger& changessign() {
        sign = !sign;
        return *this;
    }// унарный минус без создания копии

    void clearzeros() {
        while (num.size() > 1 && num.back() == 0) {
            num.pop_back();
        }
        if (num.size() == 1 && num[0] == 0) {
            sign = true;
        }
    }// удаляет лишние нули в начале числа

    void pwr_4() {
        if (num.size() == 0) {
            num.push_back(0);
        }
        num.push_back(num[num.size()-1]);
        for (int j = num.size() - 2; j > 0; --j) {
            num[j] = num[j-1];
        }
        num[0] = 0;
    }// домножение на base bigInt'a(сдвиг влево)
public:
    bool sgn() const {
        return sign;
    } // возвращает знак

    BigInteger big_abs() const {
        BigInteger copy = *this;
        copy.sign = true;
        return copy;
    } // bigInteger модуль
};




istream& operator>> (istream& in, BigInteger& b) {
    char c;
    string s;
    in >> s;
    b.num.clear();
    if (s.length() == 0) {
        b.sign = true;
    }
    else {
        if (s[0] == '-') {
            b.sign = false;
            s = s.substr(1);
        }
        else {
            b.sign = true;
        }
        for (long long i = s.length(); i > 0; i-=4) {
            if (i < 4) {
                b.num.push_back(std::stoi(s.substr(0, i)));
            }
            else {
                b.num.push_back(std::stoi(s.substr(i - 4, 4)));
            }
        }
    }
    b.clearzeros();
    return in;
}

ostream& operator<< (ostream& out, const BigInteger& b) {
    out << b.toString();
    return out;
}

bool operator< (const BigInteger& a, const BigInteger& b) {
    if (!a.sign && b.sign) {
        return true;
    }
    if (a.sign && !b.sign) {
        return false;
    }
    if (a.sign && b.sign && (a.num.size() != b.num.size())) {
        return a.num.size() < b.num.size();
    }
    if (!a.sign && !b.sign && (a.num.size() != b.num.size())) {
        return a.num.size() > b.num.size();
    }
    for (int i = a.num.size() - 1; i >= 0; --i) {
        if ((a.num[i] != b.num[i]) && (a.sign && b.sign)) {
            return a.num[i] < b.num[i];
        }
        else if ((a.num[i] != b.num[i]) && (!a.sign && !b.sign)) {
            return a.num[i] > b.num[i];
        }
    }
    return false;
}

bool operator> (const BigInteger& a, const BigInteger& b) {
    if (a.sign && !b.sign) {
        return true;
    }
    if (!a.sign && b.sign) {
        return false;
    }
    if (a.sign && b.sign && (a.num.size() != b.num.size())) {
        return a.num.size() > b.num.size();
    }
    if (!a.sign && !b.sign && (a.num.size() != b.num.size())) {
        return a.num.size() < b.num.size();
    }
    for (int i = a.num.size() - 1; i >= 0; --i) {
        if ((a.num[i] != b.num[i]) && (a.sign && b.sign)) {
            return a.num[i] > b.num[i];
        } else if ((a.num[i] != b.num[i]) && (!a.sign && !b.sign)) {
            return a.num[i] < b.num[i];
        }
    }
    return false;
}

BigInteger operator+ (const BigInteger& a, const BigInteger& b) {
    BigInteger copy = a;
    copy += b;
    return copy;
}

BigInteger operator- (const BigInteger& a, const BigInteger& b) {
    BigInteger copy = a;
    copy -= b;
    return copy;
}

BigInteger operator* (const BigInteger& a, const BigInteger& b) {
    BigInteger copy = a;
    copy *= b;
    return copy;
}

BigInteger operator/ (const BigInteger& a,const BigInteger& b){
    BigInteger copy = a;
    copy /= b;
    return copy;
}

BigInteger operator% (const BigInteger& a,const BigInteger& b){
    BigInteger copy = a;
    copy %= b;
    return copy;
}


BigInteger operator""_bi(const char* x){
    return BigInteger(x);
}
BigInteger operator""_bi(unsigned long long x){
    return BigInteger(x);
}
BigInteger operator""_bi(char x){
    return BigInteger(x);
}

string asDecimal_helper(const BigInteger& a, const BigInteger& b, long long precision) {
    BigInteger copy;
    copy.num.clear();
    copy.sign = true;
    BigInteger remainder = 0;
    remainder.sign = true;
    copy.num.resize(a.num.size());

    BigInteger dec_part = 0;
    dec_part.num.resize(precision / 4 + 1);

    for (long long i = a.num.size() - 1; i >= -(precision / 4 + 1); --i) {
        remainder.pwr_4();
        remainder.num[0] = (i >= 0 ? a.num[i] : 0);
        remainder.clearzeros();
        int find = 0;
        int left = 0;
        int right = 10000;
        while (left <= right) {
            int middle = (left + right) / 2;
            BigInteger temp = b * middle;
            temp.sign = true;
            if (temp <= remainder) {
                find = middle;
                left = middle + 1;
            }
            else {
                right = middle - 1;
            }
        }
        if (i >= 0) {
            copy.num[i] = find % 10000;
        }
        else {
            dec_part.num[i + (precision / 4 + 1)] = find % 10000;
        }
        remainder -= b * (find);
    }// деление столбиком с остатком
    copy.clearzeros();

    string s = "";
    s += copy.toString();
    s += '.';
    int n = dec_part.num[dec_part.num.size() - 1];
    if (n < 10) {
        s += "000";
    } else if (n < 100) {
        s += "00";
    } else if (n < 1000) {
        s += "0";
    }
    s += dec_part.toString();
    if (precision % 4 == 0) {
        s.pop_back(), s.pop_back(), s.pop_back(), s.pop_back();
    }
    else if (precision % 4 == 1) {
        s.pop_back(), s.pop_back(), s.pop_back();
    }
    else if (precision % 4 == 2) {
        s.pop_back(), s.pop_back();
    }
    else if (precision % 4 == 3) {
        s.pop_back();
    }
    return s;
}

BigInteger gcd(BigInteger a, BigInteger b) {
    if (a == 1) {
        return 1;
    }
    BigInteger t;
    while(b != 0) {
        if (a == 1) {
            return 1;
        }
        t = a;
        a = b;
        b = t % b;
    }
    return a;
}

class Rational{
public:
    BigInteger num;
    BigInteger den;
    bool sign = true;
    Rational() = default;
    Rational(const BigInteger& b1, const BigInteger& b2) {
        if(b1 != 0) sign = ((b1.sgn() && b2.sgn()) || (!b1.sgn() && !b2.sgn()));
        BigInteger g = gcd(b1.big_abs(), b2.big_abs());
        num = b1.big_abs() / g;
        den = b2.big_abs() / g;
    }
    Rational(const BigInteger& b) {
        if (b != 0) sign = b.sgn();
        num = b.big_abs();
        den = 1;
    }
    Rational(int a) {
        num = abs(a);
        den = 1;
        sign = (a >= 0);
    }
    Rational operator-() const {
        Rational copy = *this;
        copy.sign = !copy.sign;
        return copy;
    }
    Rational& operator+= (const Rational& r) {
        if (!sign) {
            if(!r.sign) {
                return (this->changessign() += (-r)).changessign();
            }
            else {
                return (this->changessign() -= r).changessign();
            }
        }// проверка на знаки
        else if (!r.sign) {
            *this -= (-r);
            return *this;
        }
        else {
            BigInteger n1 = (r.den * num + den * r.num).big_abs();
            BigInteger d1 = (den * r.den).big_abs();
            if (n1 == 0) {
                *this = 0;
                return *this;
            }
            BigInteger g1 = gcd(n1, d1);
            if (g1 != 0) {
                num = n1 / g1;
                den = d1 / g1;
            }
            return *this;
        }
    }
    Rational& operator-= (const Rational& r) {
        if (!this->sign) {
            if(!r.sign) {
                return (this->changessign() -= (-r)).changessign();
            }
            else {
                return (this->changessign() += r).changessign();
            }
        }
        else if(!r.sign) {
            *this += (-r);
            return *this;
        }
        else {
            if (*this < r) {
                Rational temp = *this;
                *this = r;
                return (*this -= temp).changessign();
            } else {
                BigInteger n1 = (r.den * num - den * r.num).big_abs();
                BigInteger d1 = (den * r.den).big_abs();
                if (n1 == 0) {
                    *this = 0;
                    return *this;
                }
                BigInteger g1 = gcd(n1, d1);
                if (g1 != 0) {
                    num = n1 / g1;
                    den = d1 / g1;
                }
                return *this;
            }
        }
    }
    bool operator== (const Rational& r) const {
        if (num == 0 && r.num == 0) {
            return true;
        }
        if (sign != r.sign) {
            return false;
        }
        if (num == r.num && den == r.den) {
            return true;
        }
        return false;
    }
    bool operator!= (const Rational& r) const {
        return !(*this == r);
    }
    bool operator<= (const Rational& r) const {
        return ((*this < r) || (*this == r));
    }
    bool operator>= (const Rational& r) const {
        return ((*this > r) || (*this == r));
    }
    Rational& operator*= (const Rational& r) {
        sign = ((sign && r.sign) || (!sign && !r.sign));
        num *= r.num;
        den *= r.den;
        num = num.big_abs();
        den = den.big_abs();
        if (num == 0) {
            *this = 0;
            return *this;
        }
        BigInteger g = gcd(num, den);
        if (g != 0) {
            num /= g;
            den /= g;
        }
        return *this;
    }
    Rational& operator/= (const Rational& r) {
        sign = ((sign && r.sign) || (!sign && !r.sign));
        num *= r.den.big_abs();
        den *= r.num.big_abs();
        num = num.big_abs();
        den = den.big_abs();
        if (num == 0) {
            *this = 0;
            return *this;
        }
        BigInteger g = gcd(num, den);
        if (g != 0) {
            num /= g;
            den /= g;
        }
        return *this;
    }
    string toString() const {
        string s = "";
        if (!sign) s += "-";
        if (den != 1) {
            s += num.toString();
            s += '/';
            s += den.toString();
        }
        else {
            s += num.toString();
        }
        if (num == 0) {
            s.clear();
            s += '0';
        }
        return s;
    }
    string asDecimal(size_t precision = 0) const {
        string s = "";
        if (!sign) s += "-";
        s += asDecimal_helper(num, den, precision);
        return s;
    }
    explicit operator double() {
        string s = this->asDecimal(64);
        double d = std::stod(s);
        return d;
    }

    friend bool operator< (const Rational&, const Rational&);
    friend bool operator> (const Rational&, const Rational&);
    friend Rational operator+ (const Rational&, const Rational&);
    friend Rational operator- (const Rational&, const Rational&);
    friend Rational operator* (const Rational&, const Rational&);
    friend Rational operator/ (const Rational&, const Rational&);

    Rational& changessign() {
        sign = !sign;
        return *this;
    }// унарный минус без создания копии
};



bool operator< (const Rational& a, const Rational& b) {
    if (!a.sign && b.sign) {
        return true;
    }
    if (a.sign && !b.sign) {
        return false;
    }
    if (a.sign && b.sign) {
        return a.num * b.den < b.num * a.den;
    }
    if (!a.sign && !b.sign) {
        return a.num * b.den > b.num * a.den;
    }
    return false;
}

bool operator> (const Rational& a, const Rational& b) {
    return !(a <= b);
}

Rational operator+ (const Rational& r1, const Rational& r2) {
    Rational copy = r1;
    copy += r2;
    if (copy == 0) {
        copy.sign = true;
    }
    return copy;
}

Rational operator- (const Rational& r1, const Rational& r2) {
    Rational copy = r1;
    copy -= r2;
    if (copy == 0) {
        copy.sign = true;
    }
    return copy;
}

Rational operator* (const Rational& r1, const Rational& r2) {
    Rational copy = r1;
    copy *= r2;
    return copy;
}

Rational operator/ (const Rational& r1, const Rational& r2) {
    Rational copy = r1;
    copy /= r2;
    return copy;
}

istream& operator>> (istream& in, Rational& r) {
    BigInteger b;
    in >> b;
    r = Rational(b, 1);
    return in;
}

ostream& operator<< (ostream& out, const Rational& r) {
    out << r.toString();
    return out;
}






template <int N>
struct Finite {
    long long num;
    Finite() : num(0) {};
    Finite(long long n) {
        if (n >= 0) {
            num = n % N;
        }
        else {
            num = abs(n) % N;
            num = N - num;
        }
    }
    Finite<N> operator- () {
        Finite<N> copy = *this;
        copy.num = N - copy.num;
        return copy;
    }
    Finite<N>& operator++ () {
        num += 1;
        if (num == N) num = 0;
        return *this;
    }
    Finite<N> operator++ (int) {
        Finite<N> copy = *this;
        ++(*this);// ++ делается для this, а не для copy, иначе смысла нет
        return copy;
    }
    Finite<N>& operator-- () {
        num -= 1;
        if (num == -1) num = N-1;
        return *this;
    }
    Finite<N> operator-- (int) {
        Finite<N> copy = *this;
        --(*this);// Аналогично
        return copy;
    }
    Finite<N> operator+= (Finite<N> a) {
        num += a.num;
        num %= N;
        return *this;
    }
    Finite<N> operator-= (Finite<N> a) {
        num -= a.num;
        num = (num + N) % N;
        return *this;
    }
    Finite<N> operator*= (Finite<N> a) {
        num *= a.num;
        num %= N;
        return *this;
    }
    operator long long() {
        return num;
    }
};

template <int N>
bool operator== (Finite<N> a, Finite<N> b) {
    return (a.num == b.num);
}

template <int N>
bool operator!= (Finite<N> a, Finite<N> b) {
    return !(a == b);
}

template <int N>
Finite<N> operator+ (Finite<N> a, Finite<N> b) {
    return Finite<N>(a.num + b.num);
}

template <int N>
Finite<N> operator- (Finite<N> a, Finite<N> b) {
    return Finite<N>(a.num - b.num);
}

template <int N>
Finite<N> operator* (Finite<N> a, Finite<N> b) {
    return Finite<N>(a.num * b.num);
}

bool constexpr is_prime(int n) {// Арихтектурно можно было бы создать static переменную, куда записывать ответ после первого вызова и не вычислять
    // эту функцию в дальнейшем
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

long long bin_pow_mod(long long a, long long degree, int N) {
    if (degree == 0) {
        return 1;
    }
    if (degree % 2 != 0) {
        long long c = (bin_pow_mod(a, degree - 1, N) * a) % N;
        return c;
    }
    else {
        long long b = bin_pow_mod(a, degree / 2, N) % N;
        return (b * b) % N;
    }
}

template <int N>
Finite<N> Finiteinverse(Finite<N> a) {
    static_assert(is_prime(N), "Base is not prime!");
    assert(a.num != 0 && "No inverse for 0!");
    long long result = bin_pow_mod(a.num, N-2, N);
    result %= N;
    return Finite<N>(result);
}

template <int N>
Finite<N> operator/ (Finite<N> a, Finite<N> b) {
    static_assert(is_prime(N), "Base is not prime!");
    return a * Finiteinverse(b);
}

template <unsigned N, unsigned M, typename Field = Rational>
struct Matrix{
    vector<vector<Field>> mat;
    Matrix() {
        mat.resize(N);
        for (size_t i = 0; i < N; ++i) {
            mat[i].resize(M);
            for (size_t j = 0; j < M; ++j) {
                mat[i][j] = (Field)0;
            }
        }
    }
    template <typename F = int>
    Matrix(const vector<vector<F>>& vec) {
        mat.resize(vec.size());
        for (size_t i = 0; i < vec.size(); ++i) {
            for (size_t j = 0; j < vec[i].size(); ++j) {
                mat[i].resize(vec[i].size());
                mat[i][j] = vec[i][j];
            }
        }
    }
    Matrix<N, M, Field> operator+= (const Matrix<N, M, Field>& m) {
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < M; ++j) {
                mat[i][j] += m.mat[i][j];
            }
        }
        return *this;
    }
    Matrix<N, M, Field> operator-= (const Matrix<N, M, Field>& m) {
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < M; ++j) {
                mat[i][j] -= m.mat[i][j];
            }
        }
        return *this;
    }
    Matrix<M, N, Field> transposed() const {
        Matrix<M, N, Field> transposed;
        transposed.mat.resize(M);
        for (size_t i = 0; i < M; ++i) {
            transposed.mat[i].resize(N);
            for (size_t j = 0; j < N; ++j) {
                transposed.mat[i][j] = mat[j][i];
            }
        }
        return transposed;
    }
    size_t rank() const {
        size_t rank = min(N, M);
        vector<bool> line(M);
        Matrix<N, M, Field> copy = *this;
        for (size_t i = 0; i < N; ++i) {
            size_t j;
            for (j = 0; j < M; ++j)
                if (!line[j] && copy.mat[i][j] != Field(0)) {
                    break;
                }// находим лидера в строке i
            if (j == M) {
                --rank;
            }
            else {
                line[j] = true;
                Field temp = copy.mat[i][j];
                for (size_t m = j; m < M; ++m) {
                    copy.mat[i][m]= copy.mat[i][m] / temp;
                }
                for (size_t k = 0; k < N; ++k) {
                    if (k != j && copy.mat[i][j] != Field(0)) {
                        for (size_t p = j; p < M; ++p) {
                            copy.mat[k][p] -= copy.mat[i][p] * copy.mat[k][j];
                        }
                    }
                }
            }
        }
        return rank;
    }
    vector<Field> getRow(unsigned i) {
        return mat[i];
    }
    vector<Field> getColumn(unsigned j) {
        vector<Field> column(M);
        for (size_t i = 0; i < N; ++i) {
            column[i] = mat[i][j];
        }
        return column;
    }
    vector<Field>& operator[] (unsigned i) {
        return mat[i];
    }
    vector<Field> operator[] (unsigned i) const {
        return mat[i];
    }
};




template <unsigned N, typename Field> // Довольно много копирования получается в даннм случае. У тебя уже используются static_assert, так что создание отдельного
// класса получается не логичным. Но ладно
// На самом деле можно было бы либо отнаследовать данный класс от предыдущего, либо написать teamplate <unsigned N, unsigned M = N, typename Field = Rational>
struct Matrix<N,N,Field>{
    vector<vector<Field>> mat;
    Matrix() {
        mat.resize(N);
        for (size_t i = 0; i < N; ++i) {
            mat[i].resize(N);
            for (size_t j = 0; j < N; ++j) {
                mat[i][j] = ((i == j) ? 1 : 0);
            }
        }
    }
    template <typename F = int>
    Matrix(const vector<vector<F>>& vec) {
        mat.resize(vec.size());
        for (size_t i = 0; i < vec.size(); ++i) {
            for (size_t j = 0; j < vec[i].size(); ++j) {
                mat[i].resize(vec[i].size());
                mat[i][j] = vec[i][j];
            }
        }
    }
    Matrix<N, N, Field> operator+= (const Matrix<N, N, Field>& m) {
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < N; ++j) {
                mat[i][j] += m.mat[i][j];
            }
        }
        return *this;
    }
    Matrix<N, N, Field> operator-= (const Matrix<N, N, Field>& m) {
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < N; ++j) {
                mat[i][j] -= m.mat[i][j];
            }
        }
        return *this;
    }
    Matrix<N, N, Field> transposed() const {
        Matrix<N, N, Field> transposed;
        transposed.mat.resize(N);
        for (size_t i = 0; i < N; ++i) {
            transposed.mat[i].resize(N);
            for (size_t j = 0; j < N; ++j) {
                transposed.mat[i][j] = mat[j][i];
            }
        }
        return transposed;
    }
    Matrix<N, N, Field> operator*= (const Matrix<N, N, Field>& m) {
        Matrix<N, N, Field> copy = *this;
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < N; ++j) {
                mat[i][j] = 0;
                for (size_t k = 0; k < N; ++k) {
                    mat[i][j] += copy.mat[i][k] * m.mat[k][j];
                }
            }
        }
        return *this;
    }
    Field det() const {
        Matrix<N, N, Field> copy = *this;
        Field temp;
        for (size_t k = 0; k < N; ++k) {
            for (size_t i = k + 1; i < N; ++i) {
                temp = -copy.mat[i][k] / copy.mat[k][k];
                for (size_t j = 0; j < N; ++j) {
                    copy.mat[i][j] += copy.mat[k][j] * temp;
                }
            }
        }
        Field det = 1;
        for (size_t i = 0; i < N; i++) {
            det = det * copy.mat[i][i];
        }
        return det;
    }
    size_t rank() const {
        size_t rank = N;
        vector<bool> line(N);
        Matrix<N, N, Field> copy = *this;
        for (size_t i = 0; i < N; ++i) {
            size_t j;
            for (j = 0; j < N; ++j)
                if (!line[j] && copy.mat[i][j] != Field(0)) {
                    break;
                }
            if (j == N) {
                --rank;
            }
            else {
                line[j] = true;
                Field temp = copy.mat[i][j];
                for (size_t m = j; m < N; ++m) {
                    copy.mat[i][m]= copy.mat[i][m] / temp;
                }
                for (size_t k = 0; k < N; ++k) {
                    if (k != j && copy.mat[i][j] != Field(0)) {
                        for (size_t p = j; p < N; ++p) {
                            copy.mat[k][p] -= copy.mat[i][p] * copy.mat[k][j];
                        }
                    }
                }
            }
        }
        return rank;
    }
    Field trace() {
        Field trace = 0;
        for (size_t i = 0; i < N; ++i) {
            trace += mat[i][i];
        }
        return trace;
    }
    void invert() {
        Matrix<N, N, Field> unit = Matrix<N, N, Field>();
        Field temp;
        for (size_t i = 0; i < N; ++i) {
            temp = mat[i][i];
            for (size_t j = 0; j < N; ++j) {
                mat[i][j] = mat[i][j] / temp;
                unit[i][j] = unit[i][j] / temp;
            }//2N^2
            for (size_t j = i + 1; j < N; ++j) {
                temp = mat[j][i];
                for (size_t k = 0; k < N; ++k) {
                    mat[j][k] -= (mat[i][k] * temp);
                    unit[j][k] -= (unit[i][k] * temp);
                }
            }//4N^3
        }
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < i; ++j) {
                temp = mat[j][i];
                for (size_t k = 0; k < N; ++k) {
                    mat[j][k] -= (mat[i][k] * temp);
                    unit[j][k] -= (unit[i][k] * temp);
                }
            }
        }//~4N^3
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < N; ++j) {
                mat[i][j] = unit[i][j];
            }
        }
    }
    Matrix<N, N, Field> inverted() const {
        Matrix<N, N, Field> copy = *this;
        copy.invert();
        return copy;
    }
    vector<Field> getRow(unsigned i) {
        return mat[i];
    }
    vector<Field> getColumn(unsigned j) {
        vector<Field> column(N);
        for (size_t i = 0; i < N; ++i) {
            column[i] = mat[i][j];
        }
        return column;
    }
    vector<Field>& operator[] (unsigned i) {
        return mat[i];
    }
    vector<Field> operator[] (unsigned i) const {
        return mat[i];
    }
};

template <size_t N, typename Field = Rational>
using SquareMatrix = Matrix<N, N, Field>;

// вот такое правильное бы было наследование?
// В целом да, хотя тут можно скорее написать через using, так как перегружать нечего)
//template <unsigned N, typename Field = Rational>
//struct SquareMatrix: Matrix<N,N,Field>{
//    SquareMatrix(){
//    }
//};
//

template <unsigned N, unsigned M, typename Field = Rational>
bool operator== (const Matrix<N, M, Field>& m1, const Matrix<N, M, Field>& m2) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            if (m1.mat[i][j] != m2.mat[i][j]) {
                return false;
            }
        }
    }
    return true;
}

template <unsigned N, unsigned M, typename Field = Rational>
bool operator!= (const Matrix<N, M, Field>& m1, const Matrix<N, M, Field>& m2) {
    return !(m1 == m2);
}

template <unsigned N, unsigned M, typename Field = Rational>
Matrix<N, M, Field> operator+ (const Matrix<N, M, Field>& m1, const Matrix<N, M, Field>& m2) {
    Matrix<N, M, Field> copy = m1;
    copy += m2;
    return copy;
}

template <unsigned N, unsigned M, typename Field = Rational>
Matrix<N, M, Field> operator- (const Matrix<N, M, Field>& m1, const Matrix<N, M, Field>& m2) {
    Matrix<N, M, Field> copy = m1;
    copy -= m2;
    return copy;
}

template <unsigned N, unsigned M, unsigned K, typename Field = Rational>
Matrix<N, K, Field> operator* (const Matrix<N, M, Field>& m1, const Matrix<M, K, Field>& m2) {
    Matrix<N, K, Field> m3;
    m3.mat.resize(N);
    for (size_t i = 0; i < N; ++i) {
        m3.mat[i].resize(K);
        for (size_t j = 0; j < K; ++j) {
            m3.mat[i][j] = 0;
            for (size_t k = 0; k < M; ++k) {
                m3.mat[i][j] += m1.mat[i][k] * m2.mat[k][j];
            }
        }
    }
    return m3;
}

template <unsigned N, unsigned M, typename Field = Rational>
Matrix<N, M, Field> operator* (const Matrix<N, M, Field>& m, Field a) {
    Matrix<N, M, Field> copy = m;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            copy.mat[i][j] *= a;
        }
    }
    return copy;
}

template <unsigned N, unsigned M, typename Field = Rational>
Matrix<N, M, Field> operator* (Field a, const Matrix<N, M, Field>& m) {
    Matrix<N, M, Field> copy = m;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            copy.mat[i][j] *= a;
        }
    }
    return copy;
}


