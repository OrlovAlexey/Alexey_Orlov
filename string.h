#include <iostream>
#include <cstring>
using namespace std;

class String {
private:
    size_t sz = 0;
    size_t capacity = 1;
    char* str = nullptr;
public:
    String() = default; // конструктор по умолчанию(без параметров)

    String(const char* s){
//        sz = strlen(s);
        for(; s[sz] != '\0'; ++sz){} // альтернатива пред строчке
        capacity = 2 * sz;
        this->str = new char[capacity];
//        for (size_t i = 0; i < sz; ++i){
//            this->str[i] = s[i];
//        }// неэффективно
//        memcpy(str, s, sz);
        std::copy(s, s + sz, str);
    }// конструктор с параметрами, нужно передать массив типа char, то есть строку

    String(size_t sz, char c = '\0'){
        this->sz = sz;
        capacity = 2 * sz;
        str = new char[capacity];
//        for (size_t i = 0; i < sz; ++i){
//            str[i] = c;
//        } //не эффективно
        memset(str, c, sz);
    }// конструктор с параметрами, нужно передать сколько символов и какой символ

    //String(int n) = delete; //запрещает инициализпцию от типа int

    String(std::initializer_list<char> lst){
        sz = lst.size();
        capacity = 2 * sz;
        str = new char[capacity];
        std::copy(lst.begin(), lst.end(), str);
    }// конструктор с параметрами, нужно передать символы по одному ( для записи String s{'a', 'b', 'c'}; )

    String(const String& s){
        sz = s.sz;
        capacity = 2 * sz;
        str = new char[capacity];
//        for(size_t i = 0; i < sz; ++i){
//            str[i] = s.str[i];
//        }// не эффективно
//        memcpy(str, s.str, sz);
        std::copy(s.str, s.str + s.sz, str);
    }// конструктор копирования

    //
    String(String&& s){
        this->sz = s.sz;
        capacity = 2 * sz;
        this->str = s.str;
        s.str = nullptr;
    }// конструктор перемещения
    //
//
//    String& operator= (const String& s) {
//        delete[] str;
//        sz = s.sz;
//        capacity = s.capacity;
//        str = new char[capacity];
//
//        std::copy(s.str, s.str + sz, str);
////        for(size_t i = 0; i < sz; ++i){
////            str[i] = s[i];
////        }// неэффективно
//        return *this;
//    }// оператор плохого присваиваня


    String& operator= (String s) {
        swap(s);
        return *this;
    }// оператор хорошего присваиваня

    void swap (String& s) {
        std::swap(capacity, s.capacity);
        std::swap(sz, s.sz);
        std::swap(str, s.str);
    }// своп

//    String& operator=(const String& s) = default;

    String& operator+= (const char& c){
        this->push_back(c);
        return *this;
    }// оператор плюс-равно от символа

    String& operator+= (const String& s){
//        for(size_t i = 0; i < s.sz; ++i){
//            this->push_back(s[i]);
//        }
        if (s.sz + sz > capacity){
            capacity = 2 * (sz + s.sz);
            char* copy = new char[capacity];
//            for(size_t i = 0; i < sz; ++i){
//                copy[i] = str[i];
//            }// неэффективно
//            memcpy(copy, str, sz);
            std::copy(str, str + sz, copy);
            std::copy(s.str, s.str + s.sz, copy + sz);
            str = copy;
        }
        else {
            std::copy(s.str, s.str + s.sz, str + sz);
        }
        sz = sz + s.sz;
        return *this;
    }// оператор плюс-равно от строки

    /*
     * String operator+ (const String& s) const{ // в параметры передается правый операнд
        String newStr;

        size_t thislength = this->sz;
        size_t otherlength = s.sz;
        newStr.sz = thislength + otherlength;

        newStr.str = new char[thislength + otherlength + 1];

        int i = 0;
        for(; i < thislength; ++i){
            newStr.str[i] = this->str[i];
        }

        for(int j = 0; j < otherlength; ++j, ++i){
            newStr.str[i] = s.str[j];
        }

        newStr.str[thislength + otherlength] = '\0';

        return newStr;
    }// плохая конкатенация
     */

    bool operator== (const String& s) const{ // в параметры передается правый операнд, константная ссылка чтобы не производить лишнее копирование
        if (s.sz != this->sz) return false;
        for(size_t i = 0; i < s.sz; ++i){
            if(this->str[i] != s.str[i]){
                return false;
            }
        }
        return true;
    }// оператор сравнения

    bool operator!= (const String& s) const{
        return !(this->operator==(s));
    }// оператор неравенства

    char& operator[] (size_t index) {
        return str[index];
    }// возвращает i-ый символ, получение доступа к i-ому символу

    const char& operator[] (size_t index) const {
        return str[index];
    }// возвращает i-ый символ

    size_t find(const String& subs) const{
        for(size_t i = 0; i < sz; ++i){
            if (str[i] == subs[0]){
                bool mark = true;
                size_t k = i+1;
                for(size_t j = 1; j < subs.sz; ++j, ++k){
                    if (str[k] != subs[j]) {
                        mark = false;
                        break;
                    }
                }
                if(mark == true){
                    return i;
                }
            }
        }
        return sz;
    }// поиск первого вхождения подстроки subs

    size_t rfind(const String& subs) const{
        for(size_t i = sz - 1; i > 0; --i){
            if (str[i] == subs[0]){
                bool mark = true;
                size_t k = i+1;
                for(size_t j = 1; j < subs.sz; ++j, ++k){
                    if (str[k] != subs[j]) {
                        mark = false;
                        break;
                    }
                }
                if(mark == true){
                    return i;
                }
            }
        }
        return sz;
    }// поиск последнего вхождения подстроки subs

    String substr(const size_t& start, const size_t& count) const{
        String newStr;
        for(size_t i = start; i < start + count; ++i){
            newStr.push_back(str[i]);
        }
        return newStr;
    }// поиск подстроки, начинающейся с индекса start и длины count

    size_t length() const{
        return sz;
    }// возвращает длину строки

    void push_back(const char& c){
        if (2 * sz >= capacity){
            capacity *= 2;
            char* copy = new char[capacity];
//            for(size_t i = 0; i < sz; ++i){
//                copy[i] = str[i];
//            }// неэффективно
//            memcpy(copy, str, sz);
            std::copy(str, str + sz, copy);
            str = copy;
//            delete[] copy;
        }
        if (str == nullptr) str = new char[capacity];
        str[sz] = c;
        sz++;
    }// добавляет символ в конец строки

    void pop_back(){
//        if(4*sz <= capacity){
//            capacity /= 2;
//            char* copy = new char[capacity];
////            memcpy(copy, str, sz);
//            std::copy(str, str + sz, copy);
//            str = copy;
//        }
        sz--;
        str[sz] = '\0';
    }// удаляет последний символ из строки

    bool empty() const{
        if (sz == 0) return true;
        else return false;
    }// проверяет, пустая ли строка

    void clear(){
        str = nullptr;
        sz = 0;
        capacity = 1;
    }// очищает строку

    char front() const{
        return *str;
    }// возвращает первый символ в строке

    char& front(){
        return *str;
    }

    char back() const{
        return str[sz-1];
    }// возвращает последний символ в строке

    char& back() {
        return str[sz-1];
    }

    void print(){
        cout << str << '\n';
    }// вывод в поток(нет)

    ~String(){
        delete[] str;
    }// деструктор
};

String operator+ (const String& a, const String& b){
    String copy = a;
    copy += b;
    return copy;
}// хорошая конкатенация

String operator+ (const String& a, const char& b){
    String copy = a;
    copy += b;
    return copy;
}// конкатенация от строки и символа

String operator+ (const char& a, const String& b){
    String copy("");
    copy.push_back(a);
    copy += b;
    return copy;
}// конкатенация от символа и строки

std::ostream& operator<< (std::ostream& out, const String& s){
    for(size_t i = 0; i < s.length(); ++i){
        out << s[i];
    }
    return out;
}// вывод в поток

//std::istream& operator>> (std::istream& in, String& s){
//    char c;
//    s.clear();
//    do{
//        c = in.get();
//        if (c == '\0' || c == '\n') {
//            return in;
//        }
//        s.push_back(c);
//    }while(!c);
//    return in;
//}// ввод из потока

std::istream& operator>>(std::istream& in, String& s){
    char c;
    s.clear();
    in.get(c);
    while(c != '\n' && c != ' ' && c != '\0'){
        s.push_back(c);
        if(!in.get(c))
            return in;
    }
    return in;
}

