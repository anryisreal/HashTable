#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_
 
#include <vector>
#include <initializer_list>
#include <utility>
#include <iostream>
#include <set>

namespace std _GLIBCXX_VISIBILITY(default) {
    template <typename Type1, typename Type2, int(*UserHashFunc)(Type1 key)=nullptr>
    class HashTable {
    private:
        class Info {
            public:
                Info() : ht{nullptr} { }
                Info(HashTable* ht, Type1 _key) : ht{ht}, i_key{_key} { }
                Info(HashTable* ht, std::pair<const Type1, Type2> pair) : ht{ht}, i_key{pair.first}, i_value{pair.second}, isValue{true} { }
                Type2 operator=(const Type2 value) {
                    i_value = value;
                    if (!isValue) {
                        ht->__insert(std::pair(i_key, i_value));
                    }
                    return i_value;
                }
                operator Type2() const {
                    if (!isValue) {
                        throw ("IndexError"); 
                    }
                    return i_value;
                }
                Type1 first() {
                    return i_key;
                }
                Type2 second() {
                    return i_value;
                }
                bool isfull() {
                    return isValue;
                }
            private:
                HashTable* ht {nullptr};
                Type1 i_key;
                Type2 i_value;
                bool isValue{false};
        };
        int h_capacity {0}; // Real size of the HashTable 
        int count {0}; // The number of items(Info) in the HashTable
        Info* ht{nullptr}; // Array of pointers to on items(Info)  ---Our HashTable---
        std::set<int> keys;
        const int _MAX_SIZE_ = 3000000; 


    public:
        HashTable() = default;
        HashTable(std::initializer_list<std::pair<const Type1, Type2>>&& ht) {
            this->count = ht.size();
            this->ht = new Info[count];
            h_capacity = count;
            for (auto pair: ht) {
                __insert(pair);
            }
        }
        ~HashTable() {
            delete[] ht;
        }

        Info operator[] (Type1 _key) {
            for (auto& pair: ht) {
                if (pair.first() == _key) {
                    return pair;
                }
            }
            return Info(this, _key);
        }

        int size() {
            return count;
        }

        int capacity () {
            return h_capacity;
        }
        
        void print() {
            for (auto index: keys) {
                cout << ht[index] << endl;
            }
        }

        void __resize(int index) {
            int size = h_capacity;
            if (index > h_capacity) {
                if (h_capacity * 2 > _MAX_SIZE_)  {
                    h_capacity = _MAX_SIZE_;
                }
                else {
                    h_capacity *= 2;
                }
                Info* newht = new Info[h_capacity];
                copy(ht, ht + size, newht);
                delete[] ht;
                ht = newht;
            }
        }

        void __insert(std::pair<const Type1, Type2> pair) {
            int index = 0;
            if (!UserHashFunc) {
                index = HashFunction(pair.first);
            }
            else {
                index = UserHashFunc(pair.first);
            }
            cout << index << " " << h_capacity << endl;
            __resize(index);
            HashCollision(index, pair.first);
            this->ht[index] = Info(this, pair);
            keys.insert(index);
        }

        int HashFunction(Type1 key) {
            // --- CONST ---
            float a = 143.67;
            float b = 58.733;
            int p = 13;
            int m = 5;
            // --- CONST ---
            int index = 0;
            index = ((int)(a * key + b) % p) % m;

            return index;
        }
        void HashCollision(int& index, Type1 key) {
            while (ht[index].isfull() && ht[index].first() != key) {
                index += 10;
                __resize(index);
            }
        }

    };
}   
#endif