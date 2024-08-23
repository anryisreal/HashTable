#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_
 
#include <initializer_list>
#include <utility>
#include <iostream>
#include <string>

namespace std {

    class HashException : public std::exception {
        protected:
            std::string msg;
        public:
            HashException(const char* er_msg) : msg{er_msg} {}
            HashException(const HashException& other) : msg{other.msg} {}
            const char* what () const noexcept override {
                return msg.c_str();
            }
    };

    class HE_IndexError : public HashException {
        private:
            void writemsg(int& capacity) noexcept {
                msg += std::to_string(capacity);
            }
        public:
            HE_IndexError(int capacity) : HashException("IndexError: index goes beyond the size limits: The requested index exceeds - ") {
                writemsg(capacity);
            }
    };

    class HE_SizeError: public HashException {
        public:
            HE_SizeError() : HashException("SizeError: There is no place to insert an element") { }
    };

    template <typename Type1, typename Type2, int(*UserHashFunc)(Type1 key)=nullptr>
    class HashTable {
    private:
        class Info {
            public:
                Info() : ht{nullptr}, isValue{false} { }
                Info(HashTable* ht, Type1 _key) : ht{ht}, i_key{_key}, isValue{false} { }
                Info(HashTable* ht, std::pair<const Type1, Type2> pair, int& index) : ht{ht}, i_key{pair.first}, i_value{pair.second}, index{index}, isValue{true} { }
                const Type2 operator=(Type2 value) {
                    ht->__changev(index, value);
                    if (!isValue) {
                        ht->__insert(std::pair(i_key, i_value));
                        isValue = true;
                    }
                    return i_value;
                }
                ~Info() = default;
                operator Type2() const {
                    try {
                        if (!isValue) {
                            throw HE_IndexError(this->ht->capacity()); 
                        }
                    }
                    catch(std::HE_IndexError& error) {
                        std::cout << error.what() << std::endl;
                        std::abort();
                    }
                    
                    return i_value;
                }
                Type1 first() const {
                    return i_key;
                }
                Type2 second() const {
                    return i_value;
                }
                HashTable* adress() const {
                    return ht;
                }
                void __adress(HashTable* other) {
                    ht = other;
                }
                void change(Type2 value) {
                    i_value = value;
                }
            private:
                HashTable* ht {nullptr};
                Type1 i_key;
                Type2 i_value;
                int index{-1};
                bool isValue{false};
        };
        int h_capacity {0}; // Real size of the HashTable 
        int count {0}; // The number of items(Info) in the HashTable
        Info* ht{nullptr}; // Array of pointers to on items(Info)  ---Our HashTable---
        int* keys{nullptr};
        enum CollisionType {
            search,
            insertion
        };
        const int _MAX_SIZE_ = 3000000; 


    public:
        HashTable() = default;
        HashTable(std::initializer_list<std::pair<const Type1, Type2>>&& ht) : HashTable() {
            this->h_capacity = ht.size();
            this->ht = new Info[h_capacity];
            this->keys = new int[h_capacity];
            for (auto pair: ht) {
                __insert(pair);
            }
        }
        ~HashTable() {
            delete[] ht;
            ht = nullptr;
            delete[] keys;
            keys = nullptr;
        }

        Info operator[] (Type1 _key) {

            int index = 0;
            if (!UserHashFunc) {
                index = HashFunction(_key);
            }
            else {
                index = UserHashFunc(_key);
            }
            HashCollision(index, _key, search);
            if (index == -1) {
                return Info(this, _key);
            }
            return ht[index];
        }
        const HashTable& operator=(const HashTable& other) {
            if (&other == this) {
                return *this;
            }

            h_capacity = other.h_capacity;
            count = other.count;
            delete[] ht;
            delete[] keys;
            ht = other.ht;
            keys = other.keys;
            for (int i = 0; i < count; i++) {
                ht[keys[i]].__adress(this);
            }
            
            return other;
        }

        int size() const {
            return count;
        }

        int capacity () const {
            return h_capacity;
        }
        
        void print() const {
            for (int i = 0; i < count; i++) {
                cout << ht[keys[i]].second() << endl;
            }
        }

        void __changev(int& index, Type2& value) {
            ht[index].change(value);
        }

        void __resize(int index) {
            try {
                if (index > _MAX_SIZE_) {
                    throw HE_SizeError();
                }
            }
            catch(HE_SizeError& error) {
                std::cout << error.what() << std::endl;
            }

            int size = h_capacity;
            if (index > h_capacity) {
                if (h_capacity * 2 > _MAX_SIZE_)  {
                    h_capacity = _MAX_SIZE_;
                }
                else {
                    h_capacity *= 2;
                }
                Info* newht = new Info[h_capacity];
                int* newkeys = new int[count];
                copy(ht, ht + size, newht);
                delete[] ht;
                delete[] keys;
                ht = newht;
                keys = newkeys;
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
            __resize(index);
            HashCollision(index, pair.first, insertion);
            this->ht[index] = Info(this, pair, index);
            keys[count] = index;
            count++;
        }

        int HashFunction(Type1 key) {
            // Current HashFunction working only to int, int16, int32, int64...

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
        
        void HashCollision(int& index, Type1 key, CollisionType type) {
            if (type == search && ht[index].adress() != this) {
                index = -1;
                return;
            }
            if (type == search && ht[index].adress() == this) {
                if (ht[index].first() == key) {
                    return;
                }
            }
            while (ht[index].adress() == this) {
                index += 10;
                if (type == search) {
                    if (index > h_capacity) {
                        index = -1;
                        return;
                    }
                }
                else {
                    __resize(index);
                }
            }
        }

    };
}   
#endif