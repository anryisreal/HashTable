#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

#include <initializer_list>
#include <utility>
#include <iostream>
#include <memory>
#include "hashExcp.h"

namespace Hash {

    template <typename Type1, typename Type2, int(*UserHashFunc)(Type1 key)=nullptr>
    class UHashTable {
    private:
        class Info {
        public:
            Info() : isValue{false} { }
            Info(Type1 _key) : i_key{_key}, isValue{false} { }
            Info(std::pair<const Type1, Type2> pair, int index) 
                : i_key{pair.first}, i_value{pair.second}, index{index}, isValue{true} { }

            // Оператор присваивания значения
            const Type2 operator=(Type2 value) {
                i_value = value;
                isValue = true;
                return i_value;
            }

            // Оператор преобразования к Type2
            operator Type2() const {
                if (!isValue) {
                    throw HE_IndexError(0);  // Или передайте актуальную емкость
                }
                return i_value;
            }

            Type1 first() const { return i_key; }
            Type2 second() const { return i_value; }

            void change(Type2 value) {
                i_value = value;
            }

        private:
            Type1 i_key;
            Type2 i_value;
            int index{-1};
            bool isValue{false};
        };

        int h_capacity{1};         // Реальный размер хеш-таблицы
        int count{0};              // Количество элементов в хеш-таблице
        std::unique_ptr<Info[]> ht; // Массив элементов Info
        std::unique_ptr<int[]> keys; // Массив индексов ключей

        // Типы операций
        enum CollisionType {
            search,
            insertion
        };

        const int _MAX_SIZE_ = 3000000; // Максимальный размер хеш-таблицы

    public:
        UHashTable() : ht{std::make_unique<Info[]>(1)}, keys{std::make_unique<int[]>(1)} { }

        UHashTable(std::initializer_list<std::pair<const Type1, Type2>> init_list) : UHashTable() {
            h_capacity = init_list.size();
            ht = std::make_unique<Info[]>(h_capacity);
            keys = std::make_unique<int[]>(h_capacity);
            for (const auto& pair : init_list) {
                __insert(pair);
            }
        }

        UHashTable(const UHashTable& other)
            : h_capacity{other.h_capacity}, count{other.count}, 
              ht{std::make_unique<Info[]>(other.h_capacity)},
              keys{std::make_unique<int[]>(other.count)} 
        {
            std::copy(other.ht.get(), other.ht.get() + h_capacity, ht.get());
            std::copy(other.keys.get(), other.keys.get() + count, keys.get());
        }

        UHashTable& operator=(const UHashTable& other) {
            if (this == &other) return *this;
            
            h_capacity = other.h_capacity;
            count = other.count;
            ht = std::make_unique<Info[]>(h_capacity);
            keys = std::make_unique<int[]>(count);

            std::copy(other.ht.get(), other.ht.get() + h_capacity, ht.get());
            std::copy(other.keys.get(), other.keys.get() + count, keys.get());

            return *this;
        }

        Info operator[](Type1 _key) {
            if (!count) {
                throw HE_InitialError();
            }

            int index = 0;
            if (!UserHashFunc) {
                index = HashFunction(_key);
            } else {
                index = UserHashFunc(_key);
            }
            HashCollision(index, _key, search);
            if (index == -1) {
                return Info(_key);
            }
            return ht[index];
        }

        int size() const { return count; }

        int capacity() const { return h_capacity; }

        void print() const {
            for (int i = 0; i < count; i++) {
                std::cout << ht[keys[i]].second() << std::endl;
            }
        }

        void __changev(int& index, Type2& value) {
            ht[index].change(value);
        }

        void __resize(int index) {
            if (index > _MAX_SIZE_) {
                throw HE_SizeError();
            }

            int new_capacity = h_capacity * 2;
            if (new_capacity > _MAX_SIZE_) {
                new_capacity = _MAX_SIZE_;
            }

            auto new_ht = std::make_unique<Info[]>(new_capacity);
            auto new_keys = std::make_unique<int[]>(count);

            std::copy(ht.get(), ht.get() + h_capacity, new_ht.get());
            std::copy(keys.get(), keys.get() + count, new_keys.get());

            ht.swap(new_ht);
            keys.swap(new_keys);

            h_capacity = new_capacity;
        }

        void __insert(std::pair<const Type1, Type2> pair) {
            if (!h_capacity) {
                throw HE_InitialError();
            }

            int index = 0;
            if (!UserHashFunc) {
                index = HashFunction(pair.first);
            } else {
                index = UserHashFunc(pair.first);
            }

            if (count >= h_capacity) {
                __resize(index);
            }

            HashCollision(index, pair.first, insertion);
            ht[index] = Info(pair, index);
            keys[count] = index;
            count++;
        }

        int HashFunction(Type1 key) {
            float a = 143.67f;
            float b = 58.733f;
            int p = 13;
            int m = 5;
            return static_cast<int>((a * key + b)) % p % m;
        }

        void HashCollision(int& index, Type1 key, CollisionType type) {
            int original_index = index;
            int step = 1;

            while (ht[index].first() == key) {
                index = (original_index + step) % h_capacity;
                step++;

                if (step > h_capacity) {
                    throw HE_SizeError();
                }
            }
        }
    };
}
#endif
