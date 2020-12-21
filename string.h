#include <iostream>
#include <cstring>
using namespace std;

class String {
private:
    size_t sz = 0;
    size_t capacity = 1;
    char* str = "";// nullptr при capacity = 1? Плохое поведение
    //исправил, вроде бы
public:
    String() = default; // конструктор по умолчанию(без параметров)

    String(const char* s){
        for(; s[sz] != '\0'; ++sz){} // strlen(s)
        capacity = 2 * sz;
        this->str = new char[capacity];
        std::copy(s, s + sz, str);
    }// конструктор с параметрами, нужно передать массив типа char, то есть строку

    String(size_t sz, char c = '\0'){
        this->sz = sz;
        capacity = 2 * sz;
        str = new char[capacity];
        memset(str, c, sz);
    }// конструктор с параметрами, нужно передать сколько символов и какой символ

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
        std::copy(s.str, s.str + s.sz, str);
    }// конструктор копирования

    //
    String(String&& s){
        this->sz = s.sz;
        capacity = 2 * sz;
        this->str = s.str;
        s.str = nullptr;
    }// конструктор перемещения
    //Неплохо, только не понятно зачем. Он по умолчанию как раз так и работает
    // А самое главное понимаешь ли ты зачем он существует
    //Наверно для копирования без копирования
    //чтобы сразу в память вставлять, хотя ладно, пока не знаю

    String& operator= (String s) {
        swap(s);
        return *this;
    }// оператор хорошего присваиваня

    void swap (String& s) {
        std::swap(capacity, s.capacity);
        std::swap(sz, s.sz);
        std::swap(str, s.str);
    }// своп

    String& operator+= (const char& c){
        this->push_back(c);
        return *this;
    }// оператор плюс-равно от символа

    String& operator+= (const String& s){
        if (s.sz + sz > capacity){
            capacity = 2 * (sz + s.sz);
            char* copy = str;
            str = new char[capacity];
            std::copy(copy, copy + sz, str);
            std::copy(s.str, s.str + s.sz, str + sz);
            delete[] copy;// А старый кто удалять будет?
            // удалил вроде
        }
        else {
            std::copy(s.str, s.str + s.sz, str + sz);
        }
        sz = sz + s.sz;
        return *this;
    }// оператор плюс-равно от строки

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
        for(int i = sz - 1; i >= 0; --i){// А последний символ не проверяешь? А вдруг subs из одного символа состоит, который как раз в 0 позиции? Доделай
            if (str[i] == subs[0]){// поправил
                bool mark = true;
                int k = i + 1;
                for(int j = 1; j < (int)subs.sz; ++j, ++k){
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
        String newStr(count);
//        for(size_t i = start; i < start + count; ++i){
//            newStr.push_back(str[i]);// Поочерёдный push_back? Можно ведь сразу на нужный размер создать и скопировать данные.
//        }
        newStr.clear();
        std::copy(str[start], str[start + count - 1], newStr.str);// так?
        return newStr;
    }//подстроки, начинающейся с индекса start и длины count

    size_t length() const{
        return sz;
    }// возвращает длину строки

    void push_back(const char& c){
        if (2 * sz >= capacity){
            capacity *= 2;
            char* copy = str;
            str = new char[capacity];
            std::copy(copy, copy + sz, str);
            delete[] copy;
            // А старую строку кто удалять будет?
        }
        if (str == nullptr) str = new char[capacity];
        str[sz] = c;
        sz++;
    }// добавляет символ в конец строки

    void pop_back(){
        // А почему код уменьшения размера закоментирован?
        if(4*sz <= capacity){// насколько я помню, с ним не заходило
            capacity /= 2;
            char* copy = new char[capacity];
            std::copy(str, str + sz, copy);
            str = copy;
        }
        sz--;
        str[sz] = '\0';
    }// удаляет последний символ из строки

    bool empty() const{
        // if true return true else return false....
        return (sz == 0);
    }// проверяет, пустая ли строка

    void clear(){
        str = "";// Зачем так
        sz = 0;// тогда так
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
// Три одинаковых оператора +? Компилятор сам сумеет скастовать, поправь
String operator+ (const String& a, const String& b){
    String copy = a;
    copy += b;
    return copy;
}// хорошая конкатенация

std::ostream& operator<< (std::ostream& out, const String& s){
    for(size_t i = 0; i < s.length(); ++i){
        out << s[i];
    }
    return out;
}// вывод в поток

std::istream& operator>>(std::istream& in, String& s){
    char c;
    s.clear();
    in.get(c);
    while(c != '\n' && c != ' ' && c != '\0' && c != '\t' && c != '\r'){// \t, \r
        s.push_back(c);
        if(!in.get(c))
            return in;
    }
    return in;
}