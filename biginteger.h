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
        int sz = num.size();
        for (int i = 0; i < sz; ++i) {
            if (num[i] != b.num[i]){
                return false;
            }
        }
        return true;
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
        num.resize(copy.num.size() + b.num.size());

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
//            cout << "Divided by zero" << '\n';
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
        /*
        if (num.size() - b.num.size() < 2 || b.num.size() - num.size() < 2) {
            BigInteger quot = 0;
            for(BigInteger i = *this - b; i >= 0; i -= b) {
                quot++;
            }
            *this = quot;
            return *this;
        }// простой алгоритм
        */
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
    while (in.get(c)) {
        if (c == ' ' || c == '\0' || c == '\n') {
            break;
        }
        s += c;
    }
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
    if (b == 0) {
        return a;
    }
    else {
        return gcd(b, a % b);
    }
}

class Rational{
    BigInteger num;
    BigInteger den;
    bool sign = true;
public:
    Rational() = default;
    Rational(const BigInteger b1, const BigInteger b2) {
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
        if (copy != 0) copy.sign = !copy.sign;
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
        else { //
            BigInteger n1 = (r.den * num + den * r.num).big_abs();
            cout << n1 <<'\n';
            BigInteger d1 = (den * r.den).big_abs();
            cout << d1 <<'\n';

            BigInteger g1 = gcd(n1, d1);
            cout << g1 <<'\n';

            num = n1 / g1;
            cout << num <<'\n';

            den = d1 / g1;
            cout << den <<'\n';

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
                BigInteger g1 = gcd(n1, d1);
                num = n1 / g1;
                den = d1 / g1;
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
//        cout << num.sgn() << " " << num << " " << den.sgn() << " " << den <<'\n';
//        cout << r.num.sgn() << " " << r.num << " " << r.den.sgn() << " " << r.den <<'\n';

        num *= r.num;// 1 *= 1
        den *= r.den;// 200 *= 200
        BigInteger g = gcd(num, den);
        num /= g;
//        cout << den << '\n';
        den /= g;
//        cout << den << '\n';
        return *this;
    }
    Rational& operator/= (const Rational& r) {
        sign = ((sign && r.sign) || (!sign && !r.sign));
        num *= r.den;
        den *= r.num;
        BigInteger g = gcd(num, den);
        num /= g;
        den /= g;
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

//    void print() {
//        if (!sign) cout << "-";
//        if (den != 1) {
//            cout << num << "/" << den << '\n';
//        }
//        else {
//            cout << num <<'\n';
//        }
//    } // временный вывод

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
    if (!a.sign && !b.sign) { // 1 * 100 > 1 * 100
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