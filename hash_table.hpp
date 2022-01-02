// Copyright Davydov Nikolay 2021

#ifndef INCLUDE_HASH_TABLE_HPP_
#define INCLUDE_HASH_TABLE_HPP_

#include <vector>
#include <iostream>

enum NodeStatus {
    Free = 0,
    Deleted = -1,
    Used = 1
};

template<typename Key>
struct HashFunction {
    std::size_t operator()(const Key &key) const {
        return std::hash<Key>{}(key);
    }
};


template<typename Key, typename Value, typename HFunc = HashFunction<Key>,
        class = typename std::enable_if<std::is_default_constructible<Key>::value>::type,
        class = typename std::enable_if<std::is_default_constructible<Value>::value>::type>
class HashTable {
private:
    static constexpr double coefficient_resize = 0.5;
    static constexpr double coefficient_rehash = 0.2;
    static constexpr std::size_t default_size = 500;
    static constexpr std::size_t step = 23;

    std::size_t size_;
    std::size_t deleted_elements_;
    std::size_t capacity_;

    struct Node {
        Key key_;
        Value val_;
        int flag_;

        Node() : key_(), val_(), flag_(Free) {}

        explicit Node(Key key, Value val) : key_(key), val_(val), flag_(Used) {}
    };

    std::vector<Node> table_;
    HFunc hashFunc_;

public:
    explicit HashTable() : size_(0), deleted_elements_(0), capacity_(default_size),
                           table_(capacity_) {}

    ~HashTable() noexcept = default;

    void Add(const Key &key, const Value &val) {
        std::size_t hash = hashFunc_(key) % capacity_;
        while (table_[hash].flag_ == Used) {
            hash = (hash + step) % capacity_;
        }

        if (table_[hash].flag_ == Deleted) --deleted_elements_;
        else ++size_;

        table_[hash] = Node(key, val);

        if (size_ > coefficient_resize * capacity_) Resize();
    }


    void Delete(const Key &key) {
        std::size_t hash = hashFunc_(key) % capacity_;
        while (table_[hash].flag_ != Free && table_[hash].key_ != key) {
            hash = (hash + step) % capacity_;
        }

        if (table_[hash].flag_ == Free) return;

        table_[hash] = Node();
        table_[hash].flag_ = Deleted;

        ++deleted_elements_;
        if (deleted_elements_ > coefficient_rehash * capacity_) Rehash();
    }

    bool Find(const Key &key) const {
        std::size_t hash = hashFunc_(key) % capacity_;
        while (table_[hash].flag_ != Free && table_[hash].key_ != key) {
            hash = (hash + step) % capacity_;
        }

        if (table_[hash].flag_ == Free) return false;
        return true;
    }

    Value Get(const Key &key) const {
        std::size_t hash = hashFunc_(key) % capacity_;
        while (table_[hash].flag_ != Free && table_[hash].key_ != key) {
            hash = (hash + step) % capacity_;
        }

        if (table_[hash].flag_ == Free) throw std::runtime_error("No value");
        return table_[hash].val_;
    }

    [[nodiscard]] inline std::size_t size() const { return size_; }

    [[nodiscard]] inline std::size_t capacity() const { return capacity_; }

    [[nodiscard]] inline std::size_t used_elements() const { return size_ - deleted_elements_; }

private:
    void Resize() {
        std::vector<Node> new_table(table_);

        table_.clear();
        capacity_ *= 2;
        table_.resize(capacity_);
        for (size_t i = 0; i < new_table.size(); ++i) {
            if (new_table[i].key_ == Used) Add(new_table[i].key_, new_table[i].val_);
        }

        size_ -= deleted_elements_;
        deleted_elements_ = 0;
    }

    void Rehash() {
        std::vector<Node> new_table(table_);

        table_.clear();
        table_.resize(capacity_);
        for (size_t i = 0; i < capacity_; ++i) {
            if (new_table[i].key_ == Used) Add(new_table[i].key_, new_table[i].val_);
        }

        size_ -= deleted_elements_;
        deleted_elements_ = 0;
    }
};

#endif //INCLUDE_HASH_TABLE_HPP_
