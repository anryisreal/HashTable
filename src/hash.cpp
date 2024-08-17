#include <vector>
#include <initializer_list>
#include <utility>
#include <iostream>

namespace std _GLIBCXX_VISIBILITY(default) {
    template <typename Type1, typename Type2>
    class HashTable {
    private:
        class Info {
            public:
                Type2 operator=(const Type1 _key) {
                    return i_value;
                }
                operator Type2() const {
                    
                }
            private:
                const Type1 i_key;
                Type2 i_value;
        };
        int h_capacity {0}; // Real size of the HashTable 
        int count {0}; // The number of items(Info) in the HashTable
        std::vector<Info> ht; // Array of pointers to on items(Info)  ---Our HashTable---
    public:
        HashTable() = default;
        HashTable(std::initializer_list<std::pair<const Type1, Type2>>&& ht) {
            this->count = ht.size();
            for (auto x: ht) {
                this->ht.push_back(x);
            }
            this->h_capacity = this->ht.capacity();
        }

        operator[] (Type1 _key) {
            for (auto pair: ht) {
                if (pair.first == _key) {

                }
            }
        }
        int size() {
            return count;
        }
        int capacity () {
            return h_capacity;
        }
        void insert() {

        }
        void print() {
            for (auto item: ht) {
                std::cout << item.first << " : " << item.second << std::endl;
            }
        }

    };
}


int main() {
    typedef std::HashTable <int, int> _HT;
    _HT a {{1, 2}, {3, 4}, {5, 6}};
    _HT b = a;
    b.print();
    std::cout << b.capacity();
    return 0;
}