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
    bool is_positive = true;// is_positive
    //так sign понятнее
public:
    BigInteger() {
        num.clear();
        num.push_back(0);
        is_positive = true;
    }

    BigInteger(string s) {
      if (s.length() == 0) {
        num.clear(); // int не должен быть пустым, даже в таких условиях, 0 нужно добавить
        num.push_back(0);
        is_positive = true;
      } else {
        if (s[0] == '-') {
          is_positive = false;
          s = s.substr(1);
        } else {
          is_positive = true;
        }
        for (long long i = s.length(); i > 0; i -= 4) {
          if (i < 4)
            num.push_back(atoi(s.substr(0, i).c_str()));
          else
            num.push_back(atoi(s.substr(i - 4, 4).c_str()));
        }
      }
      this->clearzeros();
    }

    BigInteger(int x) {
        if (x < 0) {
            x = -x;
            is_positive = false;
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

    BigInteger operator- () const {
        BigInteger copy = *this;
        copy.is_positive = !copy.is_positive;
        return copy;
    }
    BigInteger& operator+= (const BigInteger& b) {
        if (!this->is_positive) {
            if(!b.is_positive) {
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
                    } else {
                        num[i] = (num[i] + b.num[i] + over_digit) % base;
                        over_digit = (copy + b.num[i] + over_digit) / base;
                    }// прибавление

                    if (i == max(n1, n2) - 1 && over_digit != 0) {
                        num.push_back(over_digit);
                    }// конечный разряд
                }
                return *this;
            }
            else {
                return (this->changessign() -= b).changessign();
            }
        }// проверка на знаки
        else if (!b.is_positive) {
            //*this -= (-b);// И снова, копирование запрещено. Можно сделать sing=!is_positive, а потом *this -= b, в последствии изменив знак ещё раз (если останется отрицательный,
            // значит должен быть положительный и наоборот)
            is_positive = !is_positive;// поправил
            *this -= b;
            is_positive = !is_positive;
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
        if (!this->is_positive) {
            if(!b.is_positive) {
              if (*this > b) {
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
            else {
                return (this->changessign() += b).changessign();
            }
        }
        else if(!b.is_positive) {
            is_positive = !is_positive;
            *this += b;// И здесь так же
            is_positive = !is_positive;
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
        if ((is_positive && b.is_positive) || (!is_positive && !b.is_positive)) {
            is_positive = true;
        }
        else {
            is_positive = false;
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
//            cout << "Divided by zero" << '\n';
            return *this;
        }//проверка на деление на ноль
        if (b.num.size() > num.size()) {
            *this = 0;
            is_positive = true;
            return *this;
        }
        if ((is_positive && b.is_positive) || (!is_positive && !b.is_positive)) {
            is_positive = true;
        }
        else {
            is_positive = false;
        }//проверка на знак
        BigInteger copy = *this;
        num.clear();
        BigInteger remainder = 0;
        remainder.is_positive = true;
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
                BigInteger temp = (b * middle);
                temp.is_positive = true;
                if (temp <= remainder) {
                    find = middle;
                    left = middle + 1;
                }
                else {
                    right = middle - 1;
                } // ищем число меньше или равно частичного остатка
            }
            num[i] = find % base;
            remainder -= (b >= 0 ? (b * (find)) : -(b * (find)));
        }// деление столбиком
        clearzeros();
        return *this;
    }
    BigInteger& operator%= (const BigInteger& b) {
        BigInteger copy = *this;
        num.clear();
        BigInteger remainder = 0;
        remainder.is_positive = true;
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
                BigInteger temp = (b * middle);
                temp.is_positive = true;
                if (temp <= remainder) {
                    find = middle;
                    left = middle + 1;
                }
                else {
                    right = middle - 1;
                } // ищем число меньше или равно частичного остатка
            }
            num[i] = find % base;
            remainder -= (b >= 0 ? (b * (find)) : -(b * (find)));
        }
        *this = remainder;
        clearzeros();
        return *this;
    }

    string toString() const {
        string s = "";
        if (!is_positive) {
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

    // Эти операторы при правильном написании не должны быть friend
    // у меня /=, %= написаны через -, *, /
    // но это можно пофиксить
    // но в <, >, >> используются знаки и не только
    friend bool operator< (const BigInteger&, const BigInteger&);
    friend bool operator> (const BigInteger&, const BigInteger&);
    friend istream& operator>> (istream&, BigInteger&);
    friend string asDecimal_helper(const BigInteger&, const BigInteger&, long long);
private:
    BigInteger& changessign() {
        is_positive = !is_positive;
        return *this;
    }// унарный минус без создания копии

    void clearzeros() {
        while (num.size() > 1 && num.back() == 0) {
            num.pop_back();
        }
        if (num.size() == 1 && num[0] == 0) {
            is_positive = true;
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

    BigInteger operator* (int a) const {// добавил умножение bigint на int для /=
        BigInteger copy = *this;
        copy.num.clear();
        copy.num.resize(num.size() + 1);
        int over_digit = 0;
        for (int i = 0; i < (int)num.size() || over_digit != 0; ++i) {
            int temp;
            if (i < (int)num.size()) {
                temp = num[i] * a + over_digit;
            }
            else {
                temp = over_digit;
            }
            over_digit = temp / base;
            copy.num[i] = temp % base;
        }
        copy.clearzeros();
        return copy;
    }
public:
    bool sgn() const {// То же самое делает i<0
        return is_positive;//да, но это же быстрее
    } // возвращает знак

    BigInteger big_abs() const {
        BigInteger copy = *this;
        copy.is_positive = true;
        return copy;
    } // bigInteger модуль
};

istream& operator>> (istream& in, BigInteger& b) {
    string s;
    in >> s;//упростил
    b.num.clear();
    if (s.length() == 0) {
        b.is_positive = true;
    }
    else {
        if (s[0] == '-') {
            b.is_positive = false;
            s = s.substr(1);
        }
        else {
            b.is_positive = true;
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
    if (!a.is_positive && b.is_positive) {
        return true;
    }
    if (a.is_positive && !b.is_positive) {
        return false;
    }
    if (a.is_positive && b.is_positive && (a.num.size() != b.num.size())) {
        return a.num.size() < b.num.size();
    }
    if (!a.is_positive && !b.is_positive && (a.num.size() != b.num.size())) {
        return a.num.size() > b.num.size();
    }
    for (int i = a.num.size() - 1; i >= 0; --i) {
        if ((a.num[i] != b.num[i]) && (a.is_positive && b.is_positive)) {
            return a.num[i] < b.num[i];
        }
        else if ((a.num[i] != b.num[i]) && (!a.is_positive && !b.is_positive)) {
            return a.num[i] > b.num[i];
        }
    }
    return false;
}

bool operator> (const BigInteger& a, const BigInteger& b) {
    if (a.is_positive && !b.is_positive) {
        return true;
    }
    if (!a.is_positive && b.is_positive) {
        return false;
    }
    if (a.is_positive && b.is_positive && (a.num.size() != b.num.size())) {
        return a.num.size() > b.num.size();
    }
    if (!a.is_positive && !b.is_positive && (a.num.size() != b.num.size())) {
        return a.num.size() < b.num.size();
    }
    for (int i = a.num.size() - 1; i >= 0; --i) {
        if ((a.num[i] != b.num[i]) && (a.is_positive && b.is_positive)) {
            return a.num[i] > b.num[i];
        } else if ((a.num[i] != b.num[i]) && (!a.is_positive && !b.is_positive)) {
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

BigInteger operator/ (const BigInteger& a, const BigInteger& b){
    BigInteger copy = a;
    copy /= b;
    return copy;
}

BigInteger operator% (const BigInteger& a, const BigInteger& b){
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

BigInteger pow(BigInteger a, long long degree) {
    if (degree == 0) {
        return 1;
    }
    if (degree % 2 != 0) {
        return pow(a, degree - 1) * a;
    }
    else {
        BigInteger b = pow(a, degree / 2);
        return (b * b);
    }
}

// Зачем так сложно то, a*pow(10,precision)/b тебе в помощь
string asDecimal_helper(const BigInteger& a, const BigInteger& b, long long precision) {
    BigInteger copy;
    copy = a / b;
    copy.is_positive = true;
    BigInteger remainder;
    remainder = (a * pow(BigInteger(10), precision)) / b;
    remainder.is_positive = true;

    string s = "";
    s += copy.toString();
    s += '.';
    int n = remainder.num[remainder.num.size() - 1];
    if (n < 10) {
        s += "000";
    } else if (n < 100) {
        s += "00";
    } else if (n < 1000) {
        s += "0";
    }
    s += remainder.toString();
    if (precision == 0) s.pop_back();
    return s;
}

class Rational{
    BigInteger num;
    BigInteger den;
    bool is_positive = true;
public:
    Rational() = default;

    Rational(const BigInteger b1, const BigInteger b2) {
        if(b1 != 0) is_positive = ((b1.sgn() && b2.sgn()) || (!b1.sgn() && !b2.sgn()));
        BigInteger g = gcd(b1.big_abs(), b2.big_abs());
        num = b1.big_abs() / g;
        den = b2.big_abs() / g;
    }

    Rational(const BigInteger& b) {
        if (b != 0) is_positive = b.sgn();
        num = b.big_abs();
        den = 1;
    }
    Rational(int a) {
        num = abs(a);
        den = 1;
        is_positive = (a >= 0);
    }
    Rational operator-() const {
        Rational copy = *this;
        if (copy != 0) copy.is_positive = !copy.is_positive;
        return copy;
    }

    Rational& operator+= (const Rational& r) {
        if (!is_positive) {
            if(!r.is_positive) {
                BigInteger n1 = (r.den * num + den * r.num).big_abs();
                BigInteger d1 = (den * r.den).big_abs();
                BigInteger g1 = gcd(n1, d1);
                num = n1 / g1;
                den = d1 / g1;
                return *this;
            }
            else {
                return (this->changessign() -= r).changessign();
            }
        }// проверка на знаки
        else if (!r.is_positive) {
            is_positive = !is_positive;
            *this -= r;
            is_positive = !is_positive;
            return *this;
        }
        else {
            BigInteger n1 = (r.den * num + den * r.num).big_abs();
            BigInteger d1 = (den * r.den).big_abs();
            BigInteger g1 = gcd(n1, d1);
            num = n1 / g1;
            den = d1 / g1;
            return *this;
        }
    }
    Rational& operator-= (const Rational& r) {
        if (!this->is_positive) {
            if(!r.is_positive) {
                if (*this > r) {
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
            else {
                return (this->changessign() += r).changessign();
            }
        }
        else if(!r.is_positive) {
            is_positive = !is_positive;
            *this += r;
            is_positive = !is_positive;
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
        if (is_positive != r.is_positive) {
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
        is_positive = ((is_positive && r.is_positive) || (!is_positive && !r.is_positive));
        num *= r.num;
        den *= r.den;
        BigInteger g = gcd(num.big_abs(), den.big_abs());
        num /= g;
        den /= g;
        return *this;
    }
    Rational& operator/= (const Rational& r) {
        is_positive = ((is_positive && r.is_positive) || (!is_positive && !r.is_positive));
        num *= r.den;
        den *= r.num;
        BigInteger g = gcd(num.big_abs(), den.big_abs());
        num /= g;
        den /= g;
        return *this;
    }
    string toString() const {
        string s = "";
        if (!is_positive) s += "-";
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
        if (!is_positive) s += "-";
        s += asDecimal_helper(num, den, precision);
        return s;
    }
    explicit operator double() {
        string s = this->asDecimal(64);
        double d = std::stod(s);
        return d;
    }
    // Почему все friend то?
    // в += и -= используются
    friend bool operator< (const Rational&, const Rational&);
    friend bool operator> (const Rational&, const Rational&);
private:
    Rational& changessign() {
        is_positive = !is_positive;
        return *this;
    }// унарный минус без создания копии


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
};



bool operator< (const Rational& a, const Rational& b) {
    if (!a.is_positive && b.is_positive) {
        return true;
    }
    if (a.is_positive && !b.is_positive) {
        return false;
    }
    if (a.is_positive && b.is_positive) {
        return a.num * b.den < b.num * a.den;
    }
    if (!a.is_positive && !b.is_positive) {
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
    return copy;
}

Rational operator- (const Rational& r1, const Rational& r2) {
    Rational copy = r1;
    copy -= r2;
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
