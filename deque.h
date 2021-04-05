#include <iostream>
#include <vector>

using namespace std;

template <typename T>
class Deque{
    static const size_t base = 8;
    T** array;
    size_t sz;// number of elements
    size_t capacity;// current_array_size = capacity / base
    pair<size_t, size_t> front;// first = index in outer array, second = index in inner array (0 <= second <= base-1)
    pair<size_t, size_t> back;
public:
    Deque() {
        sz = 0;
        capacity = 8;
        array = new T*[1];
        array[0] = reinterpret_cast<T*>(new int8_t[base * sizeof(T)]);
        front = {0, 4};
        back = {0, 3};
    }

    Deque(int new_size, const T& element = T()) {
        sz = new_size;
        capacity = 3 * sz;
        array = new T*[capacity / base + 1];
        for (size_t i = 0; i < (sz / base); ++i) {
            array[(sz / base) + i] = reinterpret_cast<T*>(new int8_t[base * sizeof(T)]);
            for (size_t j = 0; j < base; ++j) {
                try {
                    new(array[(sz / base) + i] + j) T(element);
                } catch (...) {
                    throw;
                }
            }
        }// full buckets

        if (sz % base == 0) {
            front = {(2 * (sz / base) - 1), 0};
        }
        else {
            array[2 * (sz / base)] = reinterpret_cast<T*>(new int8_t[base * sizeof(T)]);
            for (size_t j = 0; j < (sz % base); ++j) {
                new(array[2 * (sz / base)] + (base - 1 - j)) T(element);
            }
            front = {2 * (sz / base), base - (sz % base)};
        }// last bucket
        back = {(sz / base), base - 1};
    }

    Deque(const Deque& another) {// new object
        sz = another.sz;
        capacity = 3 * sz;
        try {
            array = new T* [capacity / base + 1];
        } catch (...) {
            throw;
        }
        for(size_t i = another.front.first + 1; i >= another.back.first + 1; --i) {
            try {
                array[i - 1] = reinterpret_cast<T*>(new int8_t[base * sizeof(T)]);
            } catch (...) {
                this->~Deque();
                throw;
            }
            for (size_t j = (i - 1 == another.front.first ? another.front.second : 0);
                 j <= (i - 1 == another.back.first ? another.back.second : (base - 1)); ++j) {
                try {
                    new(array[i - 1] + j) T(another.array[i - 1][j]);
                } catch (...) {
                    this->~Deque();
                    throw;
                }
            }
        }
        front = another.front;
        back = another.back;
    }

    Deque& operator= (const Deque& deque1){// copying to old object
        if (this != &deque1) {
            Deque<T> copy(*this);

            for (size_t i = front.first; i + 1 > back.first; --i) {
                delete[] reinterpret_cast<int8_t*>(array[i]);
            }
            delete[] array;

            sz = deque1.sz;
            capacity = 3 * sz;
            array = new T*[capacity / base + 1];
            for(size_t i = deque1.front.first + 1; i >= deque1.back.first + 1; --i) {
                array[i - 1] = reinterpret_cast<T*>(new int8_t[base * sizeof(T)]);

                for (size_t j = (i - 1 == deque1.front.first ? deque1.front.second : 0);
                     j <= (i - 1 == deque1.back.first ? deque1.back.second : (base - 1)); ++j) {
                    try {
                        new(array[i - 1] + j) T(deque1.array[i - 1][j]);
                    } catch (...) {
                        // if constructor caught an error
                        for (; i <= deque1.front.first + 1; ++i) {
                            delete[] array[i - 1];
                        }
                        delete[] array;
                        sz = copy.sz;
                        capacity = copy.capacity;
                        front = copy.front;
                        back = copy.back;
                        array = new T* [capacity / base + 1];
                        for(size_t k = copy.front.first + 1; k >= copy.back.first + 1; --k) {
                            array[k - 1] = reinterpret_cast<T*>(new int8_t[base * sizeof(T)]);
                            for (size_t l = (k - 1 == copy.front.first ? copy.front.second : 0);
                                 l <= (k - 1 == copy.back.first ? copy.back.second : (base - 1)); ++l) {
                                new(array[k - 1] + l) T(copy.array[k - 1][l]);
                            }
                        }

                    }
                }
            }
            front = deque1.front;
            back = deque1.back;
        }
        return *this;
    }

    size_t size() const {
        return sz;
    }

    T& operator[] (size_t index){
        if (base - front.second > index) {
            return array[front.first][front.second + index];
        }
        index -= base - front.second;
        int d = index / base;
        return array[front.first - d - 1][index % base];
    }

    const T& operator[] (size_t index) const {
        if (base - front.second > index) {
            return array[front.first][front.second + index];
        }
        index -= base - front.second;
        int d = index / base;
        return array[front.first - d - 1][index % base];
    }

    T& at(size_t index){
        if (index < 0 || index >= sz) {
            throw std::out_of_range("Index is invalid.");
        }
        return this->operator[](index);
    }

    const T& at(size_t index) const {
        if (index < 0 && index >= sz) {
            throw std::out_of_range("Index is invalid.");
        }
        return this->operator[](index);
    }

    void push_back(const T& element) {
        if (back.second == base - 1) {
            while (back.first == 0) {
                try {
                    fixer();
                } catch (...) {
                    throw;
                }
            }
            --back.first;
            try {
                array[back.first] = reinterpret_cast<T*>(new int8_t[base * sizeof(T)]);
            } catch (...) {
                ++back.first;
                throw;
            }
        }
        try {
            new(array[back.first] + ((back.second + 1) % base)) T(element);
        } catch (...) {
            delete[] reinterpret_cast<int8_t*>(array[back.first]);
            ++back.first;
            throw;
        }
        back.second = (back.second + 1) % base;
        ++sz;
    }

    void push_front(const T& element) {
        if (front.second == 0) {
            if (front.first == capacity / base) {
                try {
                    fixer();
                } catch (...) {
                    throw;
                }
            }
            ++front.first;
            try {
                array[front.first] = reinterpret_cast<T*>(new int8_t[base * sizeof(T)]);
            } catch (...) {
                --front.first;
                throw;
            }
        }
        try {
            new(array[front.first] + ((front.second + base - 1) % base)) T(element);
        } catch (...) {
            delete[] reinterpret_cast<int8_t*>(array[front.first]);
            --front.first;
            throw;
        }
        front.second = (front.second + base - 1) % base;
        ++sz;
    }

    void pop_back() {
        if (sz == 0)
            return;
        if (back.second == 0) {
            delete[] reinterpret_cast<int8_t*>(array[back.first]);
            ++back.first;
        }
        back.second = (back.second + base - 1) % base;
        --sz;
    }

    void pop_front() {
        if (sz == 0)
            return;
        if (front.second == base - 1) {
            delete[] reinterpret_cast<int8_t*>(array[front.first]);
            --front.first;
        }
        front.second = (front.second + 1) % base;
        --sz;
    }

    template <bool IsConst = false>
    struct common_iterator : public std::iterator<std::random_access_iterator_tag, T> {
        pair<int, int> index;
        std::conditional_t<IsConst, const T**, T**> p;// указатель на начало дека
        common_iterator() : index({0, 0}), p(nullptr){}
        common_iterator(int index1, int index2, std::conditional_t<IsConst, const T**, T**> P) : index({index1, index2}), p(P) {}
        common_iterator(const common_iterator& it1) : index(it1.index), p(it1.p){}
        common_iterator& operator= (common_iterator it1) {
            index = it1.index;
            p = it1.p;
        }
        common_iterator& operator++ () {
            if (index.second == base - 1) {
                --index.first;
            }
            index.second = (index.second + 1) % base;
            return *this;
        }
        common_iterator operator++ (int) {
            common_iterator copy = *this;
            ++(*this);
            return copy;
        }
        common_iterator& operator-- () {
            if (index.second == 0) {
                ++index.first;
            }
            index.second = (index.second + base - 1) % base;
            return *this;
        }
        common_iterator operator-- (int) {
            common_iterator copy = *this;
            --(*this);
            return copy;
        }
        common_iterator operator+ (int a) {
            common_iterator copy = *this;
            copy.index.first -= (index.second + a) / base;
            copy.index.second = (index.second + a) % base;
            return copy;
        }
        common_iterator operator- (int a) {
            common_iterator copy = *this;
            if (a > index.second) {
                copy.index.first += (a + base - 1 - index.second) / base;
                copy.index.second = (base - (a + base - index.second) % base) % base;
            }
            else {
                copy.index.second = (index.second - a) % base;
            }
            return copy;
        }
        bool operator== (common_iterator it1) {
            return (index == it1.index && p == it1.p);
        }
        bool operator!= (common_iterator it1) {
            return !(*this == it1);
        }
        bool operator< (common_iterator it1) {
            if (index.first > it1.index.first) {
                return true;
            }
            else if (index.first == it1.index.first) {
                return (index.second < it1.index.second);
            }
            else {
                return false;
            }
        }
        bool operator<= (common_iterator it1) {
            return (*this < it1 || *this == it1);
        }
        bool operator> (common_iterator it1) {
            return !(*this <= it1);
        }
        bool operator>= (common_iterator it1) {
            return (*this > it1 || *this == it1);
        }
        int operator- (common_iterator it1) {
            return (index.second + (it1.index.first != index.first ? (it1.index.first - index.first - 1) : 0) * base + (base - it1.index.second));
        }
        std::conditional_t<IsConst, const T&, T&> operator*() {
            return p[index.first][index.second];
        }
        std::conditional_t<IsConst, const T*, T*> operator->() {
            return *(p + index.first) + index.second;
        }
    };

    using iterator = common_iterator<false>;
    using const_iterator = common_iterator<true>;

    iterator begin() {
        return iterator(front.first, front.second, array);
    }

    iterator end() {
        if (back.second == base - 1) {
            return iterator(back.first - 1, 0, array);
        }
        return iterator(back.first, back.second + 1, array);
    }

    const_iterator begin() const {
        return const_iterator(front.first, front.second, const_cast<const T**>(array));
    }

    const_iterator end() const {
        if (back.second == base - 1) {
            return const_iterator(back.first - 1, 0, const_cast<const T**>(array));
        }
        return const_iterator(back.first, back.second + 1, const_cast<const T**>(array));
    }

    const_iterator cbegin() const {
        return const_iterator(front.first, front.second, const_cast<const T**>(array));
    }

    const_iterator cend() const {
        if (back.second == base - 1) {
            return const_iterator(back.first - 1, 0, const_cast<const T**>(array));
        }
        return const_iterator(back.first, back.second + 1, const_cast<const T**>(array));
    }

    template <bool IsConst>
    struct common_reverse_iterator : public std::iterator<std::random_access_iterator_tag, T> {
        pair<int, int> index;
        std::conditional_t<IsConst, const T**, T**> p;
        common_reverse_iterator() : index({0, 0}), p(nullptr){}
        common_reverse_iterator(int index1, int index2, T** P) : index({index1, index2}), p(P) {}
        common_reverse_iterator(const common_reverse_iterator& it1) : index(it1.index), p(it1.p){}
        common_reverse_iterator& operator= (common_reverse_iterator it1) {
            index = it1.index;
            p = it1.p;
        }
        common_reverse_iterator& operator++ () {
            if (index.second == 0) {
                ++index.first;
            }
            index.second = (index.second + base - 1) % base;
            return *this;
        }
        common_reverse_iterator operator++ (int) {
            common_reverse_iterator copy = *this;
            ++(*this);
            return copy;
        }
        common_reverse_iterator& operator-- () {
            if (index.second == base - 1) {
                --index.first;
            }
            index.second = (index.second + 1) % base;
            return *this;
        }
        common_reverse_iterator operator-- (int) {
            common_reverse_iterator copy = *this;
            --(*this);
            return copy;
        }
        common_reverse_iterator operator+ (int a) {
            common_iterator copy = *this;
            if (a > index.second) {
                copy.index.first += (a + base - 1 - index.second) / base;
                copy.index.second = (base - (a + base - index.second) % base) % base;
            }
            else {
                copy.index.second = (index.second - a) % base;
            }
            return copy;
        }
        common_reverse_iterator operator- (int a) {
            common_iterator copy = *this;
            copy.index.first -= (index.second + a) / base;
            copy.index.second = (index.second + a) % base;
            return copy;
        }
        bool operator== (common_reverse_iterator it1) {
            return (index == it1.index && p == it1.p);
        }
        bool operator!= (common_reverse_iterator it1) {
            return !(*this == it1);
        }
        bool operator< (common_reverse_iterator it1) {
            if (index.first < it1.index.first) {
                return true;
            }
            else if (index.first == it1.index.first) {
                return (index.second > it1.index.second);
            }
            else {
                return false;
            }
        }
        bool operator<= (common_reverse_iterator it1) {
            return (*this < it1 || *this == it1);
        }
        bool operator> (common_reverse_iterator it1) {
            return !(*this <= it1);
        }
        bool operator>= (common_reverse_iterator it1) {
            return (*this > it1 || *this == it1);
        }
        int operator- (common_reverse_iterator it1) {
            return (it1.index.second + (it1.index.first != index.first ? (index.first - it1.index.first - 1) : 0) * base + (base - index.second));
        }
        std::conditional_t<IsConst, const T&, T&> operator*() const {
            return p[index.first][index.second];
        }
        std::conditional_t<IsConst, const T*, T*> operator->() const {
            return *p;
        }
    };

    using reverse_iterator = common_reverse_iterator<false>;
    using const_reverse_iterator = common_reverse_iterator<true>;

    reverse_iterator rbegin() {
        if (front.second == 0) {
            return reverse_iterator(front.first + 1, base - 1, array);
        }
        return reverse_iterator(front.first, front.second - 1, array);
    }

    reverse_iterator rend() {
        return reverse_iterator(back.first, back.second, array);
    }

    const_reverse_iterator crbegin() const {
        if (front.second == 0) {
            return const_reverse_iterator(front.first + 1, base - 1, array);
        }
        return const_reverse_iterator(front.first, front.second - 1, array);
    }

    const_reverse_iterator crend() const {
        return const_reverse_iterator(back.first, back.second, array);
    }

    void insert(iterator it, const T& element) {
        iterator it2 = this->end();
        --it2;
        T last = *it2;
        --it2;
        for (Deque<T>::iterator it1 = it2; it1 + 1 != it; --it1) {
            try {
                new(array[(it1 + 1).index.first] + (it1 + 1).index.second) T(array[it1.index.first][it1.index.second]);
            } catch (...) {
                ++it1;
                for (; it1 != it2 + 1; ++it1) {
                    new(array[it1.index.first] + it1.index.second) T(array[(it1 + 1).index.first][(it1 + 1).index.second]);
                }// перекладываем абратно
                new(array[back.first] + back.second) T(last);
                throw;
            }
        }
        new(array[it.index.first] + it.index.second) T(element);
        this->push_back(last);
    }

    void erase(iterator it) {
        T copy_element = *it;
        for (Deque<T>::iterator it1 = it; it1 + 1 != this->end(); ++it1) {
            try {
                new(array[it1.index.first] + it1.index.second) T(array[(it1 + 1).index.first][(it1 + 1).index.second]);
            } catch (...) {
                --it1;
                for (; it1 != it; --it1) {
                    new(array[(it1 + 1).index.first] + (it1 + 1).index.second) T(array[it1.index.first][it1.index.second]);
                }
                *it1 = copy_element;
                throw;
            }
        }
        this->pop_back();
    }

    ~Deque() {
        for (size_t i = front.first + 1; i >= back.first + 1; --i) {
            delete[] array[i - 1];
        }
        delete[] array;
    }

    void print() {
        for(size_t i = front.first + 1; i >= back.first + 1; --i) {
            for (size_t j = (i - 1 == front.first ? front.second : 0);
                 j <= (i - 1 == back.first ? back.second : (base - 1)); ++j) {
                cout << array[i - 1][j] << " ";
            }
            cout << '\n';
        }
    }// for debug

    void fixer() {
        capacity *= 3;
        T** copy = new T*[capacity / base + 1];
        size_t old_front_first = front.first;
        front.first = (capacity / base + 1) / 2 + (old_front_first - back.first + 1 > 2 ? old_front_first - back.first - 1 : 0) / 2 +
                      (old_front_first - back.first + 1 > 2 ? old_front_first - back.first - 1 : 0) % 2 +
                      (old_front_first == back.first ? 0 : 1);// recounting indices
        back.first = (capacity / base + 1) / 2 - (old_front_first - back.first + 1 > 2 ? old_front_first - back.first - 1 : 0) / 2;
        for(size_t i = front.first + 1; i >= back.first + 1; --i) {
            copy[i - 1] = array[old_front_first];
            --old_front_first;
        }
        delete[] array;
        array = copy;
    }// make extension of the array (capacity -> 3*capacity)

};