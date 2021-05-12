#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <cmath>
#include <set>

struct Pool {
    std::vector<int8_t*> pull;
    size_t index, current_capacity;
    Pool(size_t index, size_t current_capacity) : index(index), current_capacity(current_capacity) {}
    void next_block(size_t chunkSize) {
        pull.push_back(reinterpret_cast<int8_t*>(::operator new(2 * current_capacity * chunkSize)));
        ++index;
        current_capacity *= 2;
    }
    ~Pool() {
        for (auto block : pull) {
            ::operator delete(block);
        }
    }
};

template <size_t chunkSize>
class FixedAllocator {
    std::shared_ptr<Pool> pool;
    size_t used;
    static const size_t starting_size = 100;
public:
    FixedAllocator() {
        pool = std::make_shared<Pool>(0, starting_size * chunkSize);
        pool->pull.push_back(reinterpret_cast<int8_t*>(::operator new(starting_size * chunkSize)));
        used = 0;
    }
    FixedAllocator& operator= (const FixedAllocator& fixed1) {
        if (this != &fixed1) {
            pool = fixed1.pool;
            used = fixed1.used;
        }
        return *this;
    }
    int8_t* allocate(size_t n) {
        size_t temp = used;
        used += n * chunkSize;
        if (used > pool->current_capacity) {
            pool->next_block(chunkSize);
            used = n * chunkSize;
            temp = 0;
        }
        return (pool->pull[pool->index] + temp);
    }
    void deallocate(int8_t*) {}
    ~FixedAllocator() = default;
};

template <typename T>
class FastAllocator {
public:
    FixedAllocator<4> fixed_alloc4;
    FixedAllocator<8> fixed_alloc8;
    typedef size_t     size_type;
    typedef ptrdiff_t  difference_type;
    typedef T*         pointer;
    typedef const T*   const_pointer;
    typedef T&         reference;
    typedef const T&   const_reference;
    typedef T          value_type;
    template<typename U>
    struct rebind {
        using other = FastAllocator<U>;
    };

    FastAllocator() noexcept {}

    FastAllocator(const FastAllocator& fastAllocator1) noexcept {
        fixed_alloc4 = fastAllocator1.fixed_alloc4;
        fixed_alloc8 = fastAllocator1.fixed_alloc8;
    }

    template <class U>
    FastAllocator (const FastAllocator<U>& fastAllocator1) noexcept {
        fixed_alloc4 = fastAllocator1.fixed_alloc4;
        fixed_alloc8 = fastAllocator1.fixed_alloc8;
    }

    FastAllocator& operator= (const FastAllocator<value_type>& fastAllocator1) {
        if (this != &fastAllocator1) {
            fixed_alloc4 = fastAllocator1.fixed_alloc4;
            fixed_alloc8 = fastAllocator1.fixed_alloc8;
        }
        return *this;
    }
    struct propagate_on_container_move_assignment : std::true_type {};

    pointer allocate(size_type n) {
        pointer ptr;
        size_t sz = n * sizeof(value_type);
        if (sz % 8 == 0) {
            ptr = reinterpret_cast<pointer>(fixed_alloc8.allocate(sz / 8));
        }
        else if (sz % 4 == 0) {
            ptr = reinterpret_cast<pointer>(fixed_alloc4.allocate(sz / 4));
        }
        else {
            ptr = reinterpret_cast<pointer>(::operator new(sz));
        }
        return ptr;
    }

    void deallocate(pointer ptr, size_type n) {
        if (!ptr) {
            return;
        }
        size_t sz = n * sizeof(value_type);
        if (sz % 8 == 0) {
            fixed_alloc8.deallocate(reinterpret_cast<int8_t*>(ptr));
        }
        else if (sz % 4 == 0) {
            fixed_alloc4.deallocate(reinterpret_cast<int8_t*>(ptr));
        }
        else {
            ::operator delete(ptr);
        }
    }

    template <typename... Args>
    void construct(pointer ptr, const Args&... args) {
        new(ptr) T(args...);
    }
    void destroy(pointer ptr) {
        ptr->~value_type();
    }
    ~FastAllocator() = default;
};

template <>
class FastAllocator<char*>{
public:
    typedef size_t         size_type;
    typedef ptrdiff_t      difference_type;
    typedef char**         pointer;
    typedef const char**   const_pointer;
    typedef char*&         reference;
    typedef const char*&   const_reference;
    typedef char*          value_type;
    template<typename U>
    struct rebind {
        using other = FastAllocator<U>;
    };

    FastAllocator() noexcept {}

    FastAllocator (const FastAllocator&) noexcept {}

    template <typename U>
    FastAllocator (const FastAllocator<U>&) noexcept {}

    FastAllocator& operator= (const FastAllocator<value_type>&) {
        return *this;
    }
    struct propagate_on_container_move_assignment : std::true_type {};

    pointer allocate(size_type n) {
        return reinterpret_cast<pointer>(::operator new(n * sizeof(value_type)));
    }

    void deallocate(pointer ptr, size_type n) {
        if (!ptr) {
            return;
        }
        ::operator delete(ptr, n * sizeof(value_type));
    }

    template <typename... Args>
    void construct(pointer ptr, const Args&... args) {
        new(ptr) value_type(args...);
    }
    void destroy(pointer ptr) {
        ptr->~value_type();
    }
    ~FastAllocator() = default;
};

template <class T, class U>
bool operator== (const FastAllocator<T>&, const FastAllocator<U>&) {
    return true;
}

template <class T, class U>
bool operator!= (const FastAllocator<T>&, const FastAllocator<U>&) {
    return false;
}


template <typename T, typename Allocator = std::allocator<T>>
class List {
    struct Node {
        T value;
        Node* next;
        Node* prev;
        Node() = delete;
        Node(const T& Value, Node* Next, Node* Prev) : value(Value), next(Next), prev(Prev) {}
        Node(T&& Value, Node* Next, Node* Prev) : value(std::move(Value)), next(Next), prev(Prev) {}
    };
    Node* first;
    Node* last;
    size_t sz;
    using NodeAllocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
    NodeAllocator alloc;
    using AllocTraits = std::allocator_traits<NodeAllocator>;

public:
    List() : alloc(NodeAllocator()) {
        sz = 0;
        first = AllocTraits::allocate(alloc, 1);
        last = AllocTraits::allocate(alloc, 1);
        first->next = last;
        last->prev = first;
    }
    explicit List(const Allocator& alloc1) : alloc(AllocTraits::select_on_container_copy_construction(alloc1)) {
        sz = 0;
        first = AllocTraits::allocate(alloc, 1);
        last = AllocTraits::allocate(alloc, 1);
        first->next = last;
        last->prev = first;
    }

    explicit List(size_t count, const T& value, const Allocator& alloc1 = Allocator()) : List(alloc1) {
        sz = count;
        Node* newnode;
        Node* prevnode = first;
        for (size_t i = 0; i < sz; ++i) {
            newnode = AllocTraits::allocate(alloc, 1);
            AllocTraits::construct(alloc, newnode, value, last, prevnode);
            prevnode->next = newnode;
            last->prev = newnode;
            prevnode = newnode;
        }
    }

    explicit List(size_t count, const Allocator& alloc1 = Allocator()) : List(alloc1) {
        sz = count;
        Node* newnode;
        Node* prevnode = first;
        for (size_t i = 0; i < sz; ++i) {
            newnode = AllocTraits::allocate(alloc, 1);
            AllocTraits::construct(alloc, newnode, last, prevnode);
            prevnode->next = newnode;
            last->prev = newnode;
            prevnode = newnode;
        }
    }

    NodeAllocator& get_allocator() {
        return alloc;
    }

    List(const List& another) : List(AllocTraits::select_on_container_copy_construction(another.alloc)) {
        sz = another.sz;
        Node* newnode;
        Node* prevnode = first;
        Node* an_first = another.first;
        for (size_t i = 0; i < sz; ++i) {
            an_first = an_first->next;
            newnode = AllocTraits::allocate(alloc, 1);
            AllocTraits::construct(alloc, newnode, an_first->value, last, prevnode);
            prevnode->next = newnode;
            last->prev = newnode;
            prevnode = newnode;
        }
    }

    List(List&& another) : List(AllocTraits::select_on_container_copy_construction(std::move(another.alloc))) {
        sz = another.sz;
        Node* prevnode = first;
        Node* an_first = another.first;
        for (size_t i = 0; i < sz; ++i) {
            an_first = an_first->next;
            prevnode->next = an_first;
            an_first->prev = prevnode;
            an_first->next = last;
            last->prev = an_first;
            prevnode = an_first;
        }
        another.make_empty();
    }

    List& operator= (const List& list1) {
        if (this != &list1) {

            Node* ptr = first;
            Node* prev;
            for (size_t i = 0; i < sz; ++i) {
                ptr = ptr->next;
                prev = ptr->prev;
                AllocTraits::destroy(alloc, prev);
                AllocTraits::deallocate(alloc, prev, 1);
            }
            if (AllocTraits::propagate_on_container_copy_assignment::value) {
                alloc = list1.alloc;
            }
            sz = list1.sz;
            first = AllocTraits::allocate(alloc, 1);
            last = AllocTraits::allocate(alloc, 1);
            first->next = last;
            last->prev = first;
            Node* newnode;
            Node* prevnode = first;
            Node* an_first = list1.first;
            for (size_t i = 0; i < sz; ++i) {
                an_first = an_first->next;
                newnode = AllocTraits::allocate(alloc, 1);
                AllocTraits::construct(alloc, newnode, an_first->value, last, prevnode);
                prevnode->next = newnode;
                last->prev = newnode;
                prevnode = newnode;
            }
        }
        return *this;
    }

    List& operator= (List&& list1) noexcept {
        if (this != &list1) {
            Node* ptr = first;
            Node* prev;
            for (size_t i = 0; i < sz; ++i) {
                ptr = ptr->next;
                prev = ptr->prev;
                AllocTraits::destroy(alloc, prev);
                AllocTraits::deallocate(alloc, prev, 1);
            }
            if (AllocTraits::propagate_on_container_move_assignment(list1.alloc)) {
                alloc = std::move(list1.alloc);
            }
            sz = list1.sz;
            first = AllocTraits::allocate(alloc, 1);
            last = AllocTraits::allocate(alloc, 1);
            first->next = last;
            last->prev = first;

            Node* prevnode = first;
            Node* an_first = list1.first;
            for (size_t i = 0; i < sz; ++i) {
                an_first = an_first->next;
                prevnode->next = an_first;
                an_first->prev = prevnode;
                an_first->next = last;
                last->prev = an_first;
                prevnode = an_first;
            }
            list1.make_empty();
        }
        return *this;
    }

    size_t size() const {
        return sz;
    }

    bool empty() const {
        return (sz == 0);
    }

    void push_back(const T& value) {
        Node* newnode = AllocTraits::allocate(alloc, 1);
        AllocTraits::construct(alloc, newnode, value, last, last->prev);
        last->prev->next = newnode;
        last->prev = newnode;
        ++sz;
    }

    template <typename UniversalT>
    void push_back(UniversalT&& value) {
        Node* newnode = AllocTraits::allocate(alloc, 1);
        AllocTraits::construct(alloc, newnode, std::forward<UniversalT>(value), last, last->prev);
        last->prev->next = newnode;
        last->prev = newnode;
        ++sz;
    }

    template <typename UniversalT>
    void push_back(UniversalT&& value, size_t hash) {
        Node* newnode = AllocTraits::allocate(alloc, 1);
        Allocator().construct(&(newnode->value.first), std::forward<UniversalT>(value));
        newnode->value.second = hash;
        newnode->next = last;
        newnode->prev = last->prev;
        last->prev->next = newnode;
        last->prev = newnode;
        ++sz;
    }// конструирование ноды по частям

    void push_front(const T& value) {
        Node* newnode = AllocTraits::allocate(alloc, 1);
        AllocTraits::construct(alloc, newnode, value, first->next, first);
        first->next->prev = newnode;
        first->next = newnode;
        ++sz;
    }

    template <typename UniversalT>
    void push_front(UniversalT&& value) {
        Node* newnode = AllocTraits::allocate(alloc, 1);
        AllocTraits::construct(alloc, newnode, std::forward<UniversalT>(value), first->next, first);
        first->next->prev = newnode;
        first->next = newnode;
        ++sz;
    }

    void pop_back() {
        Node* exnode = last->prev;
        exnode->prev->next = last;
        last->prev = exnode->prev;
        AllocTraits::destroy(alloc, exnode);
        AllocTraits::deallocate(alloc, exnode, 1);
        --sz;
    }

    void pop_front() {
        Node* exnode = first->next;
        exnode->next->prev = first;
        first->next = exnode->next;
        AllocTraits::destroy(alloc, exnode);
        AllocTraits::deallocate(alloc, exnode, 1);
        --sz;
    }

    template <bool IsConst = false>
    struct common_iterator : public std::iterator<std::bidirectional_iterator_tag, Node> {
        std::conditional_t<IsConst, const Node*, Node*> ptr;

        using value_type = std::conditional_t<IsConst, const T, T>;
        using pointer = std::conditional_t<IsConst, const T*, T*>;
        using reference = std::conditional_t<IsConst, const T&, T&>;

        common_iterator() : ptr(nullptr) {}
        common_iterator(std::conditional_t<IsConst, const Node*, Node*> Ptr) : ptr(Ptr) {}
        common_iterator(const common_iterator<false>& other) : ptr(other.ptr){}
        common_iterator& operator= (common_iterator it1) {
            ptr = it1.ptr;
            return *this;
        }
        common_iterator& operator++ () {
            ptr = ptr->next;
            return *this;
        }
        common_iterator operator++ (int) {
            common_iterator copy = *this;
            ++(*this);
            return copy;
        }
        common_iterator& operator-- () {
            ptr = ptr->prev;
            return *this;
        }
        common_iterator operator-- (int) {
            common_iterator copy = *this;
            --(*this);
            return copy;
        }
        bool operator== (const common_iterator<IsConst>& it1) const {
            return (ptr == it1.ptr);
        }
        bool operator!= (const common_iterator<IsConst>& it1) const {
            return !(*this == it1);
        }// (non-const == const) not works!!
        std::conditional_t<IsConst, const T&, T&> operator*() {
            return ptr->value;
        }
        std::conditional_t<IsConst, const T*, T*> operator->() {
            return &ptr->value;
        }
        friend common_iterator<false>;
    };

    using iterator = common_iterator<false>;
    using const_iterator = common_iterator<true>;

    iterator begin() {
        return iterator(first->next);
    }

    iterator end() {
        return iterator(last);
    }

    const_iterator begin() const {
        return const_iterator(const_cast<const Node*>(first->next));
    }

    const_iterator end() const {
        return const_iterator(const_cast<const Node*>(last));
    }

    const_iterator cbegin() const {
        return const_iterator(const_cast<const Node*>(first->next));
    }

    const_iterator cend() const {
        return const_iterator(const_cast<const Node*>(last));
    }

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    reverse_iterator rbegin() {
        return reverse_iterator(last);
    }
    reverse_iterator rend() {
        return reverse_iterator(first->next);
    }
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(const_cast<const Node*>(last));
    }
    const_reverse_iterator rend() const {
        return const_reverse_iterator(const_cast<const Node*>(first->next));
    }
    const_reverse_iterator crbegin() {
        return const_reverse_iterator(const_cast<const Node*>(last));
    }
    const_reverse_iterator crend() {
        return const_reverse_iterator(const_cast<const Node*>(first->next));
    }

    iterator insert(const_iterator it, const T& value) {
        iterator find(const_cast<Node*>(it.ptr));
        Node* newnode = AllocTraits::allocate(alloc, 1);
        AllocTraits::construct(alloc, newnode, value, find.ptr, find.ptr->prev);
        find.ptr->prev->next = newnode;
        find.ptr->prev = newnode;
        ++sz;
        return iterator(newnode);
    }

    iterator insert_back(const_iterator it) {
        Node* ptr = const_cast<Node*>(it.ptr);
        ptr->next = last;
        ptr->prev = last->prev;
        last->prev->next = ptr;
        last->prev = ptr;
        ++sz;
        return iterator(ptr);
    }

    iterator erase(const_iterator it) {
        iterator find(const_cast<Node*>(it.ptr));
        Node* result = find.ptr->next;
        find.ptr->prev->next = find.ptr->next;
        find.ptr->next->prev = find.ptr->prev;
        AllocTraits::destroy(alloc, find.ptr);
        AllocTraits::deallocate(alloc, find.ptr, 1);
        --sz;
        return iterator(result);
    }

    void print() {
        Node* p = first->next;
        for (size_t i = 0; i < sz; ++i) {
            std::cout << p->value;
            p = p->next;
        }
    }// print all elements in list

    ~List() {
        Node* ptr = first->next;
        Node* prev;
        for (size_t i = 0; i < sz; ++i) {
            ptr = ptr->next;
            prev = ptr->prev;
            AllocTraits::destroy(alloc, prev);
            AllocTraits::deallocate(alloc, prev, 1);
        }
        AllocTraits::deallocate(alloc, first, 1);
        AllocTraits::deallocate(alloc, last, 1);
    }

    void make_empty() {
        sz = 0;
        first->next = last;
        last->prev = first;
    }

    template <typename... Args>
    iterator makeNode(Args&&... args) {
        Node* node = AllocTraits::allocate(alloc, 1);
        Allocator().construct(&(node->value.first), std::forward<Args>(args)...);
        return iterator(node);
    }

    void deleteNode(iterator it) {
        Node* node = it.ptr;
        Allocator().destroy(&(node->value.first));
        AllocTraits::deallocate(alloc, node, 1);
    }
};


using std::pair;
using std::vector;

template <typename Key, typename Value, typename Hash = std::hash<Key>, typename Equal = std::equal_to<Key>, typename Alloc = std::allocator<std::pair<const Key, Value>>>
class UnorderedMap{
private:
    size_t sz;
    size_t bucket_n;
    using NodeType = pair<const Key, Value>;
    using hashedNodeType = pair<NodeType, size_t>;
    vector<List<hashedNodeType, Alloc>> hashTable;
    Hash hasher;
    Equal key_equal;
    constexpr static const float static_max_load_factor = 1.0;
    std::set<size_t> pointers;
public:

    UnorderedMap() : sz(), bucket_n(1), hashTable(1), hasher(), key_equal(), pointers() {}

    explicit UnorderedMap(size_t bucket_count, const Hash& hash = Hash(), const Equal& equal = Equal(), const Alloc& alloc = Alloc() ) : sz(), bucket_n(bucket_count), hasher(hash), key_equal(equal), pointers() {
        hashTable(bucket_count, alloc);
    }

    UnorderedMap(const UnorderedMap& other) {
        sz = other.sz;
        bucket_n = other.bucket_n;
        hasher = other.hasher;
        key_equal = other.key_equal;
        for (auto& chain : other.hashTable) {
            hashTable.push_back(chain);
        }
        pointers = other.pointers;
    }

    UnorderedMap(UnorderedMap&& other)  noexcept {
        sz = other.sz;
        other.sz = 0;
        bucket_n = other.bucket_n;
        other.bucket_n = 1;
        hasher = std::move(other.hasher);
        key_equal = std::move(other.key_equal);
        hashTable = std::move(other.hashTable);
        pointers = std::move(other.pointers);
    }

    UnorderedMap& operator= (const UnorderedMap& umap1) {
        if (this != &umap1) {
            sz = umap1.sz;
            bucket_n = umap1.bucket_n;
            hasher = umap1.hasher;
            key_equal = umap1.key_equal;
            hashTable = umap1.hashTable;
            pointers = umap1.pointers;
        }
        return *this;
    }

    UnorderedMap& operator= (UnorderedMap&& umap1)  noexcept {
        if (this != &umap1) {
            sz = umap1.sz; umap1.sz = 0;
            bucket_n = umap1.bucket_n; umap1.bucket_n = 1;
            hasher = std::move(umap1.hasher);
            key_equal = std::move(umap1.key_equal);
            hashTable = std::move(umap1.hashTable);
            pointers = std::move(umap1.pointers);
        }
        return *this;
    }

    Value& operator[] (const Key& key) {
        size_t hashValue = hasher(key) % bucket_n;
        auto& chain = hashTable[hashValue];
        for (auto it = chain.begin(); it != chain.end(); ++it) {
            if (key_equal(it->first.first, key)) {
                return it->first.second;
            }
        }
        auto pair_it = insert({key, Value()});
        return ((pair_it.first)->second);
    }

//    Value& operator[] (Key&& key) {
//
//    }

    Value& at(const Key& key) {
        size_t hashValue = hasher(key) % bucket_n;
        auto& chain = hashTable[hashValue];
        for (auto it = chain.begin(); it != chain.end(); ++it) {
            if (key_equal(it->first.first, key)) {
                return it->first.second;
            }
        }
        throw std::out_of_range("No such key in map.");
    }

    const Value& at(const Key& key) const {
        size_t hashValue = hasher(key) % bucket_n;
        auto& chain = hashTable[hashValue];
        for (auto it = chain.begin(); it != chain.end(); ++it) {
            if (key_equal(it->first->first, key)) {
                return it->first->second;
            }
        }
        throw std::out_of_range("No such key in map.");
    }

    size_t size() const {
        return sz;
    }

    template <bool IsConst = false>
    struct CommonIterator : public std::iterator<std::forward_iterator_tag, hashedNodeType> {
        using ListIter = typename List<hashedNodeType, Alloc>::iterator;
        using ConstListIter = typename List<hashedNodeType, Alloc>::const_iterator;
        std::conditional_t<IsConst, ConstListIter, ListIter> inner_iter;
        using SetIter = typename std::set<size_t>::iterator;
        using ConstSetIter = typename std::set<size_t>::const_iterator;
        std::conditional_t<IsConst, ConstSetIter, SetIter> outer_iter;
        std::conditional_t<IsConst, const UnorderedMap*, UnorderedMap*> owner;

        using value_type = std::conditional_t<IsConst, const NodeType, NodeType>;
        using pointer = std::conditional_t<IsConst, const NodeType*, NodeType*>;
        using reference = std::conditional_t<IsConst, const NodeType&, NodeType&>;

        CommonIterator() : inner_iter(), outer_iter(), owner(nullptr) {}

        CommonIterator(std::conditional_t<IsConst, ConstListIter, ListIter> iter, std::conditional_t<IsConst, ConstSetIter, SetIter> index, std::conditional_t<IsConst, const UnorderedMap*, UnorderedMap*> owner) : inner_iter(iter), outer_iter(index), owner(owner) {}

        CommonIterator(const CommonIterator<false> &other) : inner_iter(other.inner_iter), outer_iter(other.outer_iter), owner(other.owner) {}

        CommonIterator& operator= (CommonIterator it1) {
            inner_iter = it1.inner_iter;
            outer_iter = it1.outer_iter;
            owner = it1.owner;
            return *this;
        }

        CommonIterator& operator++ () {
            inner_iter++;
            if (inner_iter == owner->hashTable[*outer_iter].end()) {
                ++outer_iter;
                if (outer_iter == owner->pointers.end()) {
                    return *this;
                }
                inner_iter = owner->hashTable[*outer_iter].begin();
            }
            return *this;
        }

        CommonIterator operator++ (int) {
            CommonIterator copy = *this;
            ++(*this);
            return copy;
        }

        bool operator== (const CommonIterator<IsConst>& it1) const {
            return (outer_iter == it1.outer_iter && inner_iter == it1.inner_iter && owner == it1.owner);
        }

        bool operator!= (const CommonIterator<IsConst>& it1) const {
            return !(*this == it1);
        }

        reference operator* () {
            return inner_iter->first;
        }

        pointer operator-> () {
            return &(inner_iter->first);
        }

        friend CommonIterator<false>;
    };
    using Iterator = CommonIterator<false>;
    using ConstIterator = CommonIterator<true>;

    Iterator begin() {
        if (hashTable.begin() == hashTable.end())
            return end();
        return Iterator(hashTable[*pointers.begin()].begin(), pointers.begin(), this);
    }

    Iterator end() {
        if (hashTable.begin() == hashTable.end())
            return Iterator(nullptr, pointers.end(), this);
        return Iterator(hashTable[*(--pointers.end())].end(), pointers.end(), this);
    }

    ConstIterator begin() const {
        return ConstIterator(hashTable[*pointers.begin()].begin(), pointers.begin(), this);
    }

    ConstIterator end() const {
        return ConstIterator(hashTable[*(--pointers.end())].end(), pointers.end(), this);
    }

    ConstIterator cbegin() const {
        return ConstIterator(hashTable[*pointers.begin()].begin(), pointers.begin(), this);
    }

    ConstIterator cend() const {
        return ConstIterator(hashTable[*(--pointers.end())].end(), pointers.end(), this);
    }

    pair<Iterator, bool> insert(const NodeType& pair_value) {
        auto& key = pair_value.first;
        auto& value = pair_value.second;
        if (load_factor() >= max_load_factor()) {
            rehash(2 * bucket_n);
        }

        size_t hashValue = hasher(key);
        auto& chain = hashTable[hashValue % bucket_n];
        for (auto it = chain.begin(); it != chain.end(); ++it) {
            if (key_equal(it->first.first, key)) {
                return {Iterator(it, pointers.find(hashValue % bucket_n), this), false};
            }
        }
        chain.push_back(std::make_pair(std::make_pair(key, value), hashValue));
        ++sz;
        auto temp_it = pointers.insert(hashValue % bucket_n);
        return {Iterator(--chain.end(), temp_it.first, this), true};
    }

    template <typename NodeTypeT>
    auto insert(NodeTypeT&& pair_value) -> pair<Iterator, bool> {
        if (load_factor() >= max_load_factor()) {
            rehash(2 * bucket_n);
        }

        size_t hashValue = hasher(std::forward<const Key>(pair_value.first));
        auto& chain = hashTable[hashValue % bucket_n];
        for (auto it = chain.begin(); it != chain.end(); ++it) {
            if (key_equal(std::forward<const Key>(it->first.first), std::forward<const Key>(pair_value.first))) {
                return {Iterator(it, pointers.find(hashValue % bucket_n), this), false};
            }
        }
        chain.push_back(std::forward<NodeTypeT>(pair_value), hashValue);
        ++sz;
        auto temp_it = pointers.insert(hashValue % bucket_n);
        return {Iterator(--chain.end(), temp_it.first, this), true};
    }

    auto insert(NodeType&& x) {
        return insert<>(x);
    }

    template <class InputIt>
    void insert(InputIt first, InputIt last) {
        for (InputIt iter = first; iter != last; ++iter) {
            auto temp = *iter;
            insert(temp);
        }
    }

    template <class... Args>
    pair<Iterator, bool> emplace(Args&&... args) {
        if (load_factor() >= max_load_factor()) {
            rehash(2 * bucket_n);
        }

        typename List<hashedNodeType, Alloc>::iterator list_it = hashTable[0].makeNode(std::forward<Args>(args)...);
        size_t hashValue = hasher(list_it->first.first);
        list_it->second = hashValue;

        auto &chain = hashTable[hashValue % bucket_n];
        for (auto it = chain.begin(); it != chain.end(); ++it) {
            if (key_equal(it->first.first, list_it->first.first)) {
                hashTable[0].deleteNode(list_it);
                return {Iterator(it, pointers.find(hashValue % bucket_n), this), false};
            }
        }
        chain.insert_back(list_it);
        ++sz;
        auto temp_it = pointers.insert(hashValue % bucket_n);
        return {Iterator(--chain.end(), temp_it.first, this), true};
    }

    Iterator erase(Iterator iter) {
        auto copy = iter;
        ++copy;
        size_t hashValue = hasher(iter->first);
        --sz;
        hashTable[hashValue % bucket_n].erase(iter.inner_iter);
        if (hashTable[hashValue % bucket_n].empty()) {
            pointers.erase(hashValue % bucket_n);
        }
        return copy;
    }

    Iterator erase(ConstIterator iter) {
        auto copy = iter;
        copy++;
        size_t hashValue = hasher(iter->first);
        --sz;
        hashTable[hashValue % bucket_n].erase(iter.inner_iter);
        if (hashTable[hashValue % bucket_n].empty()) {
            pointers.erase(hashValue % bucket_n);
        }
        return copy;
    }

    template <class InputIt>
    Iterator erase(InputIt first, InputIt last) {
        Iterator iter;
        for (iter = first; iter != last;) {
            iter = erase(iter);
        }
        return iter;
    }

    Iterator find(const Key& key) {
        size_t hashValue = hasher(key);
        auto& chain = hashTable[hashValue % bucket_n];
        for (auto it = chain.begin(); it != chain.end(); ++it) {
            if (key_equal(it->first.first, key)) {
                return Iterator(it, pointers.find(hashValue % bucket_n), this);
            }
        }
        return end();
    }

    ConstIterator find(const Key& key) const {
        size_t hashValue = hasher(key);
        auto& chain = hashTable[hashValue % bucket_n];
        for (auto it = chain.begin(); it != chain.end(); ++it) {
            if (key_equal(it->first.first, key)) {
                return Iterator(it, pointers.find(hashValue % bucket_n), this);
            }
        }
        return end();
    }

    void rehash(size_t count) {
        size_t old_bucket_n = bucket_n;
        bucket_n = count;
        vector<List<hashedNodeType, Alloc>> new_hashTable(bucket_n);
        std::set<size_t> new_pointers;
        size_t hashValue;
        for (size_t i = 0; i < old_bucket_n; ++i) {
            auto it = hashTable[i].begin();
            auto it_copy = it;
            for (; it != hashTable[i].end(); it = it_copy) {
                it_copy = it;
                it_copy++;
                hashValue = it->second;
                new_hashTable[hashValue % bucket_n].insert_back(it);
                new_pointers.insert(hashValue % bucket_n);
            }
            hashTable[i].make_empty();
        }
        hashTable = std::move(new_hashTable);
        pointers = std::move(new_pointers);
    }

    void reserve(size_t count) {
        if (count <= bucket_n)
            return;
        rehash(std::ceil(static_cast<float>(count) / max_load_factor()));
    }

    float load_factor() const {
        return (static_cast<float>(sz) / static_cast<float>(bucket_n));
    }

    float max_load_factor() const {
        return static_max_load_factor;
    }

    size_t max_size() const {
        return bucket_n;
    }

    ~UnorderedMap() = default;

    void print() {
        for (auto& chain : hashTable) {
            for (auto& element : chain) {
                std::cout << element.first.first << " " << element.first.second << '\n';
            }
        }
    }
};